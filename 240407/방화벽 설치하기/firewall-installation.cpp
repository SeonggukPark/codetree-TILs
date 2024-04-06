#include <iostream>
#include <cstring>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
#define pii pair<int, int>
using namespace std;
constexpr size_t MAX_SIZE = 8;
int n, m, pos_idx, fire_idx, rst;
int grid[MAX_SIZE][MAX_SIZE];
pii pos[MAX_SIZE * MAX_SIZE];
pii fire[MAX_SIZE + 2];
int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

void init(){
    memset(grid, 0, sizeof(grid));
    pos_idx = fire_idx = 0;
    rst = -1;
}

void input(){
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
            if(grid[i][j] == 0) pos[pos_idx++] = {i, j};
            else if(grid[i][j] == 2) fire[fire_idx++] = {i, j};
        }
    }
}

void bfs(int x, int y){
    for(int i = 0; i < 4; i++){
        int nx = x + dx[i], ny = y + dy[i];

        if(nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
        if(grid[nx][ny] == 0){
            grid[nx][ny] = 2;
            bfs(nx, ny);
        }
    }
}


void run(){
    int ori_grid[MAX_SIZE][MAX_SIZE];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ori_grid[i][j] = grid[i][j];
        }
    }


    for (int i = 0; i < pos_idx; ++i) {
        for (int j = i + 1; j < pos_idx; ++j) {
            for (int k = j + 1; k < pos_idx; ++k) {
                grid[pos[i].first][pos[i].second] = 1;
                grid[pos[j].first][pos[j].second] = 1;
                grid[pos[k].first][pos[k].second] = 1;

                for(int x = 0; x < fire_idx; x++){
                    bfs(fire[x].first, fire[x].second);
                }

                int tmp = 0;

                for (int x = 0; x < n; ++x) {
                    for (int y = 0; y < m; ++y) {
                        if(grid[x][y] == 0) tmp++;
                    }
                }

                if(rst < tmp) rst = tmp;

                for (int x = 0; x < n; ++x) {
                    for (int y = 0; y < m; ++y) {
                        grid[x][y] = ori_grid[x][y];
                    }
                }
            }
        }
    }
}


int main() {
    fastio
    init();
    input();
    run();

    cout << rst << endl;

    return 0;
}