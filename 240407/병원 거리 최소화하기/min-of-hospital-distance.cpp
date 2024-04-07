#include <iostream>
#include <cstring>
#include <vector>
#include <set>
using namespace std;
constexpr size_t MAX_N = 50;
int n, m, rst;
int grid[MAX_N][MAX_N];

vector<pair<int, int> > house, chicken;
set<int> s;

void init(){
    memset(grid, 0, sizeof(grid));
    house.clear();
    chicken.clear();
    s = {};
    rst = 1000000;
}

void input(){
    cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];

            if(grid[i][j] == 1) house.emplace_back(i, j);
            else if(grid[i][j] == 2) chicken.emplace_back(i, j);
        }
    }
}

void dfs(int idx, int depth){
    if(depth == m){
        int add = 0;

        for(auto h : house){
            int cnt = 1000000;
            for(auto c : s){
                cnt = min(cnt, abs(h.first - chicken[c].first) + abs(h.second - chicken[c].second));
            }

            add += cnt;
        }

        rst = min(rst, add);
        return;
    }


    for(int i = idx + 1; i < chicken.size(); i++){
        s.insert(i);
        dfs(i, depth + 1);
        s.erase(i);
    }
}

void run(){
    dfs(-1, 0);
}

int main() {
    init();
    input();
    run();

    cout << rst << endl;
    return 0;
}