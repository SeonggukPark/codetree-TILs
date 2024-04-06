#include <iostream>
#include <cstring>
#include <set>
#define endl '\n'
#define pii pair<int, int>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

using namespace std;
constexpr size_t MAX_N = 200;
int n, m, max_sum;
int grid[MAX_N][MAX_N];

void init(){
    memset(grid, 0, sizeof(grid));
    max_sum = 0;
}

void input(){
    cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
        }
    }
}

void dfs(int depth, set<pii> s, int cnt){
    if(depth == 4){
        if(cnt > max_sum) max_sum = cnt;
        return;
    }

    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    for(auto pos : s){
        for(int i = 0; i < 4; i++){
            int nx = pos.first + dx[i], ny = pos.second + dy[i];

            if(0 <= nx && nx < n && 0 <= ny && ny < m){
                if(s.find({nx, ny}) == s.end()){
                    s.insert({nx, ny});
                    dfs(depth + 1, s, cnt + grid[nx][ny]);
                    s.erase({nx, ny});
                }
            }
        }
    }
}

void run(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            set<pii> s;
            s.insert({i, j});
            dfs(1, s, grid[i][j]);
        }
    }
}


int main() {
    fastio
    init();
    input();
    run();

    cout << max_sum << endl;
    return 0;
}