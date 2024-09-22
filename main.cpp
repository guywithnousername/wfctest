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

map<char, vector<map<char, int>>> cinrules(const char* path) {
    FILE *fp = freopen(path, "r", stdin);
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
int BSIDE = 3; // side length of a part
int DIST = 1; // length between parts

int main() {
    auto rules = cinrules("map/minimap.txt");
    vector<vector<char>> map = getans(HEIGHT, WIDTH, rules);
    vector<vector<char>> ans = vector<vector<char>> (HEIGHT * (BSIDE + DIST) - DIST, 
    vector<char> (WIDTH * (BSIDE + DIST) - DIST)); 
    cout << "minimap:\n";
    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            cout << map[i][j];
        }
        cout << "\n";
    }
    cout << "----\n";
    for (int i = 0; i < ans.size(); i += BSIDE + DIST) {
        for (int j = 0; j < ans[0].size(); j += BSIDE + DIST) {
            // pass (might not be looping properly)
        }
    }
}