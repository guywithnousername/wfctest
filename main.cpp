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
map<char, vector<map<char, int>>> rules;
pair<int, int> dirs[8] = {{-1, 0}, {1, 0}, {-1, 1}, {1, 1}, 
{0, 1}, {-1, -1}, {0, -1}, {1, -1}};

int n, m; // input
int n2, m2; // output
vector<vector<char>> ans;
int nums; // number of grids created

int rand(int st, int mx) { // random [st, mx]
    std::random_device rd;
    std::mt19937 gen(rd());
    uniform_int_distribution<> distrib (st, mx);
    return distrib(gen);
}

char choose(map<char, int> a) {
    vector<pair<int, char>> v (a.size());
    int tem = 1;
    for (pair<char, int> b : a) {
        v.push_back({tem, b.first});
        tem += b.second;
    }
    int choice = rand(0, a['\0']);
    for (auto it = v.rbegin(); it != v.rend(); it ++) {
        if (choice >= (*it).first) {
            return (*it).second;
        }
    }
}

void wfc(vector<vector<map<char, int>>> grid) {
    if (nums <= 0) return; 

   // lists tiles with specific entropy
    map<int, vector<pair<int, int>>> ent;
    pair<int, int> lE = {-1, -1}; // leastEntropy
    bool flag = true;
    // get entropy by checking how many choices there are. 
    for (int i = 0; i < n2; i ++) {
        for (int j = 0; j < m2; j ++) {
            if (grid[i][j].size() == 1) return;
            if (grid[i][j].size() == 2) continue;
            ent[grid[i][j].size()].push_back({j, i});
            flag = false;
        }
    }
    cout << "Recorded ents\n";
    if (flag) {
        // print out
        for (int i = 0; i < n2; i ++) {
            for (int j = 0; j < m2; j ++) {
                ans[i][j] = (*(grid[i][j].begin())).first;
                cout << ans[i][j] ;
            }
            cout << "\n";
        }
        cout << "----\n";
        nums --;
        return;
    }
    cout << "Checked flag\n";
    // pick a random tile with the lowest entropy (excluding tiles with 1 choice)
    vector<pair<int, int>> least = (*ent.begin()).second;

    lE = least[rand(0, least.size() - 1)];
    int cx = lE.first;
    int cy = lE.second;
    cout << "Found cx cy\n";
    // finds and applies a random possibility
    /* **********************************************
    STILL NEEDS CHANGES HERE 
    - get way to use random to get choice
    - get way to check if rejection happens and stop
    it from choosing the same choice.
    - remove nums variable
    */
    char poss = choose(grid[cy][cx]);
    auto copy = grid;
    copy[cy][cx] = map<char, int> {{poss, 1}};
    cout << "Got random choice\n";
    for (int dir = 0; dir < 8; dir ++) {
        int x = dirs[dir].first;
        int y = dirs[dir].second;
        if (cy + y < 0 || cy + y >= n2 || cx + x < 0 || cx + x >= m2) continue;
        cout << "x y " << x << " " << y << "\n";
        map<char, int> a = rules[poss][dir];
        map<char, int> b = grid[cy + y][cx + x];
        map<char, int> ins = {};
        for (pair<char, int> tem : a) {
            if (b.find(tem.first) != b.end()) {
                ins[tem.first] = tem.second;
            }
        }
        cout << "merged a and b\n";
        copy[cy + y][cx + x] = ins;
        cout << "Set ins\n";
    }
    cout << "Created copy\n";
    // wfc(copy);
    // **********************************************
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
        map<char, int> tem;
        tem['\0'] = 0; // no one can use this, so it is the total. 
        rules[tile] = vector<map<char, int>> (8, tem);
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
                rules[tile][dir][type] ++;
                rules[tile][dir]['\0'] ++;
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
   map<char, int> s; 
   for (char a : tiles) {
    s[a] = 1;
    s['\0'] ++;
   }
   vector<vector<map<char, int>>> start (10, vector<map<char, int>> (10, s));
   wfc(start);
}