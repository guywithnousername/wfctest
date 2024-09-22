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
    freopen("types/b.txt", "r", stdin);
    int n, m; // input
    int n2 = 5, m2 = 5; // output
    cin >> n >> m;
    vector<vector<char>> grid (n, vector<char> (m));
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            cin >> grid[i][j];
        }
    }
    vector<vector<char>> ans = generate(grid, n2, m2);
    for (int i = 0; i < n2; i ++) {
        for (int j = 0; j < m2; j ++) {
            cout << ans[i][j];
        }
        cout << "\n";
    }
    cout << "----\n";
}