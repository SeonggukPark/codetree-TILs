#include <iostream>
#include <cstring>
#define endl '\n'
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_N = 500;
int n, m, max_sum;
int grid[MAX_N][MAX_N];
int visited[MAX_N][MAX_N];
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};


void init(){
    memset(grid, 0, sizeof(grid));
    memset(visited, 0, sizeof(visited));
    max_sum = 0;
}

void input(){
    cin >> n >> m;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            cin >> grid[i][j];
        }
    }
}

void dfs(int x, int y, int depth, int add) {
    if (depth == 4) {
        if (add > max_sum) max_sum = add;
        return;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];

        if (1 <= nx && nx <= n && 1 <= ny && ny <= m) {
            if(visited[nx][ny]) continue;
            visited[nx][ny] = 1;
            dfs(nx, ny, depth + 1, add + grid[nx][ny]);
            visited[nx][ny] = 0;
        }
    }
}


void run(){
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            visited[i][j] = 1;
            dfs(i, j, 1, grid[i][j]);
            visited[i][j] = 0;
        }
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            int t = grid[i][j];
            int mini = 1001;

            for (int k = 0; k < 4; k++) {
                if(mini > grid[i + dx[k]][j + dy[k]]) mini = grid[i + dx[k]][j + dy[k]];
                t += grid[i + dx[k]][j + dy[k]];
            }

            if(max_sum < t - mini) max_sum = t - mini;
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