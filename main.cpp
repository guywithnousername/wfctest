#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstdlib>
#include <random>
#include <algorithm>

#include "wfc.h"

using namespace std;

map<char, vector<map<char, int>>> cinrules(string path) {
    const char* cpath = path.c_str();
    FILE *fp = freopen(cpath, "r", stdin);
    int n, m; cin >> n >> m;
    vector<vector<char>> grid (n, vector<char> (m));
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            cin >> grid[i][j];
        }
    }
    fclose(fp);
    return getrules(grid);
}

int HEIGHT = 3, WIDTH = 3;
int BSIDE = 3; // side length of a part
int DIST = 1; // length between parts

vector<vector<char>> stitch(vector<vector<char>> ans, map<char, vector<map<char, int>>> rules, int x1, int y1, int x2, int y2) {
    // currently not using x1, y1, x2, y2
    map<char, vector<map<char, int>>> all = cinrules("map/all.txt");
    cout << "-----\n";
    for (int y = 0; y < HEIGHT; y ++) {
        for (int x = 0; x < WIDTH; x ++) {
            cout << "x y " << x << " " << y << "\n";
            int sy = max(0, (y - 1) * (BSIDE + DIST));
            int sx = max(0, (x - 1) * (BSIDE + DIST));
            int ey = min(int(ans.size() - 1), (y + 1) * (BSIDE + DIST));
            int ex = min(int(ans[0].size() - 1), (x + 1) * (BSIDE + DIST));
            for (int i = sy; i <= ey; i ++) {
                for (int j = sx; j <= ex; j ++) {
                    cout << ans[i][j];
                }
                cout << "\n";
            }
            cout << "new:\n";
            int distx = (ex - sx + 1), disty = (ey - sy + 1);
            vector<vector<map<char, int>>> old (disty, vector<map<char, int>> (distx));
            for (int i = sy; i <= ey; i ++) {
                for (int j = sx; j <= ex; j ++) {
                    int tem = ans[i][j];
                    old[i - sy][j - sy] = {{tem, 1}, {'\0', 1}};
                }
            }
            vector<vector<char>> next = getans(disty, distx, all, old);
            for (int i = 0; i < disty; i ++) {
                for (int j = 0; j < distx; j ++) {
                    cout << next[i][j];
                }
                cout << "\n";
            }
            cout << "-----\n";
        }
    }
    return ans;
}

int main() {
    map<char, map<char, vector<map<char, int>>>> ruleset;
    map<char, vector<map<char, int>>> rules = cinrules("map/minimap.txt");
    vector<vector<char>> minim = getans(HEIGHT, WIDTH, rules);
    vector<vector<char>> ans = vector<vector<char>> (HEIGHT * (BSIDE + DIST), 
    vector<char> (WIDTH * (BSIDE + DIST), ' ')); 
    cout << "minimap:\n";
    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            cout << minim[i][j];
        }
        cout << "\n";
    }
    cout << "----\n";
    for (char biome : gettiles(rules)) {
        ruleset[biome] = cinrules(("map/types/" + string(1, biome) + ".txt"));
    }
    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            // cout << "A\n";
            int c = (BSIDE + DIST) * i;
            int d = (BSIDE + DIST) * j;
            vector<vector<char>> cur = getans(BSIDE, BSIDE, ruleset[minim[i][j]]);
            for (int y = 0; y < BSIDE; y ++) {
                for (int x = 0; x < BSIDE; x ++) {
                    // cout << "xy " << d + x << " " << c + y << "\n";
                    ans[c + y][d + x] = cur[y][x];
                }
            }
        }
    }
    for (int i = 0; i < ans.size(); i ++) {
        for (int j = 0; j < ans[0].size(); j ++) {
            cout << ans[i][j];
        }
        cout << "\n";
    }
    stitch(ans, rules, 0, 0, WIDTH, HEIGHT);
}