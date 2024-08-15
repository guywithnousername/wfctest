#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstdlib>
#include <random>
#include <algorithm>

using namespace std;

pair<int, int> dirs[8] = {{-1, 0}, {1, 0}, {-1, 1}, {1, 1}, 
{0, 1}, {-1, -1}, {0, -1}, {1, -1}};


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
        if (b.first == '\0') continue;
        v.push_back({tem, b.first});
        tem += b.second;
    }
    int choice = rand(0, a['\0']);
    auto it = v.rbegin();
    for (; it != v.rend(); it ++) {
        if (choice >= (*it).first) {
            return (*it).second;
        }
    }
    it --;
    return (*it).second;
}
vector<vector<char>> wfc(vector<vector<map<char, int>>> grid, map<char, vector<map<char, int>>> rules, int n2, int m2) {
   // lists tiles with specific entropy
    map<int, vector<pair<int, int>>> ent;
    pair<int, int> lE = {-1, -1}; // leastEntropy
    bool flag = true;
    // get entropy by checking how many choices there are. 
    for (int i = 0; i < n2; i ++) {
        for (int j = 0; j < m2; j ++) {
            if (grid[i][j].size() == 1) {
                return {};
            }
            if (grid[i][j].size() == 2) continue;
            ent[grid[i][j].size()].push_back({j, i});
            flag = false;
        }
    }
    // for (int i = 0; i < n2; i ++) {
    //     for (int j = 0; j < m2; j ++) {
    //         cout << "\033[0m";
    //         if (grid[i][j].size() > 2) {
    //             cout << "\033[1;31m";
    //         } 
    //         cout << "[";
    //         for (char t : tiles) {
    //             if (grid[i][j].count(t)) {
    //                 cout << t;
    //             } else cout << " ";
    //         }
    //         cout << "]";
    //     }
    //     cout << "\n";
    // }
    // cout << "\033[0m-----\n";
    if (flag) {
        // print out
        vector<vector<char>> ans (n2, vector<char> (m2));
        for (int i = 0; i < n2; i ++) {
            for (int j = 0; j < m2; j ++) {
                auto it = grid[i][j].begin();
                advance(it, 1);
                ans[i][j] = (*(it)).first;
            }
        }
        return ans;
    }
    // pick a random tile with the lowest entropy (excluding tiles with 1 choice)
    vector<pair<int, int>> least = (*ent.begin()).second;

    lE = least[rand(0, least.size() - 1)];
    int cx = lE.first;
    int cy = lE.second;
    // finds and applies a random possibility
    set<char> no;
    while (1) {
        char poss = choose(grid[cy][cx]);
        while (rules.find(poss) == rules.end() && !no.count(poss)) {
            poss = choose(grid[cy][cx]);
        }
        auto copy = grid;
        copy[cy][cx] = map<char, int> {{poss, 1}, {'\0', 1}};
        for (int dir = 0; dir < 8; dir ++) {
            int x = dirs[dir].first;
            int y = dirs[dir].second;
            if (cy + y < 0 || cy + y >= n2 || cx + x < 0 || cx + x >= m2) continue;
            map<char, int> a = rules[poss][dir];
            map<char, int> b = grid[cy + y][cx + x];
            map<char, int> ins = {};
            int sz = 0;
            for (pair<char, int> tem : a) {
                if (tem.first == '\0') continue;
                if (b.find(tem.first) != b.end()) {
                    ins[tem.first] = tem.second;
                    sz += tem.second;
                }
            }
            ins['\0'] = sz;
            copy[cy + y][cx + x] = ins;
        }
        vector<vector<char>> res = wfc(copy, rules, n2, m2);
        if (res.size() < 1) {
            no.insert(poss);
            if (no.size() >= grid[cy][cx].size() - 1) {
                return {};
            }
        } else {
            return res;
        }
    }
    return {};
}

vector<char> gettiles(vector<vector<char>> grid) {
    vector<char> tiles = {};
    int n = grid.size(), m = grid[0].size();
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            if (find(tiles.begin(), tiles.end(), grid[i][j]) == tiles.end()) {
                tiles.push_back(grid[i][j]);
            }
        }
    }
    return tiles;
}

map<char, vector<map<char, int>>> getrules(vector<vector<char>> grid) {
    map<char, vector<map<char, int>>> rules;
    vector<char> tiles = gettiles(grid);
    // get rules
    for (char tile : tiles) {
        map<char, int> tem;
        tem['\0'] = 0; // no one can use this, so it is the total. 
        rules[tile] = vector<map<char, int>> (8, tem);
    }
    for (int i = 0; i < grid.size(); i ++) {
        for (int j = 0; j < grid[0].size(); j ++) {
            for (int dir = 0; dir < 8; dir ++) {
                int x = dirs[dir].first;
                int y = dirs[dir].second;
                if (x == 0 && y == 0) continue;
                if (j + x < 0 || j + x >= grid[0].size()) continue;
                if (i + y < 0 || i + y >= grid.size()) continue;
                char tile = grid[i][j];
                char type = grid[i + y][j + x];
                rules[tile][dir][type] ++;
                rules[tile][dir]['\0'] ++;
            }
        }
    }
    return rules;
}

vector<vector<char>> generate(vector<vector<char>> grid, int n2, int m2) {
    vector<vector<char>> ans (n2, vector<char> (m2));
    vector<char> tiles = gettiles(grid);
    map<char, vector<map<char, int>>> rules = getrules(grid);
    // creates arguments and uses the function.
    map<char, int> s; 
    for (char a : tiles) {
        s[a] = 1;
        s['\0'] ++;
    }
    vector<vector<map<char, int>>> start (n2, vector<map<char, int>> (m2, s));
    return wfc(start, rules, n2, m2);
}

int main() {
    int n, m; // input
    int n2, m2; // output
    int nums; 
    cin >> n >> m >> n2 >> m2 >> nums;
    vector<vector<char>> grid (n, vector<char> (m));
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            cin >> grid[i][j];
            
        }
    }
    cout << "A\n";
    vector<vector<char>> ans = generate(grid, n2, m2);
    for (int it = 0; it < nums; it ++) {
        for (int i = 0; i < n2; i ++) {
            for (int j = 0; j < m2; j ++) {
                cout << "\033[0m";
                if (ans[i][j] == '*') cout << "\033[33m";
                if (ans[i][j] == '~') cout << "\033[34m";
                if (ans[i][j] == '#') cout << "\033[1;32m";
                cout << ans[i][j];
            }
            cout << "\n";
        }
        cout << "----\n";
    }
}