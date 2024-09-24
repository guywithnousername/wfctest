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

int HEIGHT = 3, WIDTH = 4;
int BSIDE = 5; // side length of a part
int DIST = 3; // length between parts

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
}