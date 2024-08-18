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