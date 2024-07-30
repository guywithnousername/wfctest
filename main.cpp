#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstdlib>
#include <random>
#include <algorithm>

using namespace std;

vector<vector<char>> grid;
vector<char> tiles = {};
map<char, vector<set<char>>> rules;
pair<int, int> dirs[8] = {{-1, 0}, {1, 0}, {-1, 1}, {1, 1}, 
{0, 1}, {-1, -1}, {0, -1}, {1, -1}};

int n, m; // input
int n2, m2; // output
vector<vector<char>> ans;
int nums; // number of grids created

void wfc(vector<vector<set<char>>> grid) {
    if (nums <= 0) return;
    /*
    pseudocode
    ------------
    check if there is a tile with no available choices
        if so, return
    try to find a least entropy tile that has multiple choices
        if so:
            loop through all the possible choices:
                create a copy of grid 
                set the tile in that copy to the choice
                narrow down choices in surrounding tiles
                wfs(copy)
        if not, set to ans and return 
    */

   // lists tiles with specific entropy
    map<int, vector<pair<int, int>>> ent;
    pair<int, int> lE = {-1, -1}; // leastEntropy
    bool flag = true;
    // get entropy by checking how many choices there are. 
    for (int i = 0; i < n2; i ++) {
        for (int j = 0; j < m2; j ++) {
            if (grid[i][j].size() == 0) return;
            if (grid[i][j].size() == 1) continue;
            ent[grid[i][j].size()].push_back({j, i});
            flag = false;
        }
    }

    if (flag) {
        // print out
        for (int i = 0; i < n2; i ++) {
            for (int j = 0; j < m2; j ++) {
                ans[i][j] = *(grid[i][j].begin());
                cout << ans[i][j] ;
            }
            cout << "\n";
        }
        cout << "----\n";
        nums --;
        return;
    }

    // pick a random tile with the lowest entropy (excluding tiles with 1 choice)
    vector<pair<int, int>> least = (*ent.begin()).second;
    // copied of cppreference lol https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    uniform_int_distribution<> distrib (0, least.size() - 1);
    lE = least[distrib(gen)];
    int cx = lE.first;
    int cy = lE.second;

    // applies a random choice and changes 
    // the grid based on it. 
    for (char poss : grid[cy][cx]) {
        auto copy = grid;
        copy[cy][cx] = set<char> {poss};
        for (int dir = 0; dir < 8; dir ++) {
            int x = dirs[dir].first;
            int y = dirs[dir].second;
            if (cy + y < 0 || cy + y >= n2 || cx + x < 0 || cx + x >= m2) continue;
            set<char> a = rules[poss][dir];
            set<char> ins = {};
            set_intersection(grid[cy + y][cx + x].begin(), grid[cy + y][cx + x].end(),
            a.begin(), a.end(), inserter(ins, ins.begin()));
            copy[cy + y][cx + x] = ins;
        }
        wfc(copy);
    }
    return;
}

int main() {
    // input 
    cin >> n >> m >> n2 >> m2 >> nums;
    grid = vector<vector<char>> (n, vector<char> (m));
    ans = vector<vector<char>> (n2, vector<char> (m2));
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            cin >> grid[i][j];
            if (find(tiles.begin(), tiles.end(), grid[i][j]) == tiles.end()) {
                tiles.push_back(grid[i][j]);
            }
        }
    }

    // get rules
    for (char tile : tiles) {
        rules[tile] = vector<set<char>> (8, set<char> {});
    }
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            for (int dir = 0; dir < 8; dir ++) {
                int x = dirs[dir].first;
                int y = dirs[dir].second;
                if (x == 0 && y == 0) continue;
                if (j + x < 0 || j + x >= m) continue;
                if (i + y < 0 || i + y >= n) continue;
                char tile = grid[i][j];
                char type = grid[i + y][j + x];
                rules[tile][dir].insert(type);
            }
        }
    }

    // print out rules
    /*
    for (auto a : rules) {
        cout << a.first << "\n";
        for (int dir = 0; dir < 8; dir ++) {
            cout << "   xy " << dirs[dir].first <<
            " " << dirs[dir].second << "\n      ";
            for (char i : a.second[dir]) cout << i << " ";
            cout << "\n";
        }
    }
    */

   // creates arguments and uses the function.
   set<char> s; 
   for (char a : tiles) s.insert(a);
   vector<vector<set<char>>> start (10, vector<set<char>> (10, s));
   wfc(start);
}