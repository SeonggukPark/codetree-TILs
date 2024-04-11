#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;
constexpr size_t MAX_N = 20;
int n, m, k, c, rst;
int grid[MAX_N][MAX_N];
int sur_num[MAX_N][MAX_N];
int killer[MAX_N][MAX_N];
int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

void init(){
    memset(grid, 0, sizeof(grid));
    memset(sur_num, 0, sizeof(sur_num));
    memset(killer, false, sizeof(killer));
    rst = 0;
}

void input(){
    cin >> n >> m >> k >> c; // n: 격자 크기, m: 진행 년 수, k: 확산 범위, c: 제초제 잔여 년 수
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j]; // 빈칸 0, 벽 -1, 나무 1 ~ 100
        }
    }
}

void traverse(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(3) << grid[i][j] << ' '; // 빈칸 0, 벽 -1, 나무 1 ~ 100
        }
        cout << endl;
    }
    cout << endl;
}

void Grow(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(grid[i][j] <= 0) continue;
            for (int l = 0; l < 4; ++l) {
                int nx = i + dx[l], ny = j + dy[l];
                if(nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
                if(grid[nx][ny] > 0) {
                    grid[i][j]++;
                }
            }
        }
    }
}

int add_grid[MAX_N][MAX_N];
void Extend(){
    memset(add_grid, 0, sizeof(add_grid));
    memset(sur_num, 0, sizeof(sur_num));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(grid[i][j] <= 0) continue;

            // 주변 빈칸 count
            for (int l = 0; l < 4; ++l) {
                int nx = i + dx[l], ny = j + dy[l];
                if(nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
                if(grid[nx][ny] == 0 && killer[nx][ny] == 0) sur_num[i][j]++;
            }

            // 나무 / 개수 만큼 spread
            for (int l = 0; l < 4; ++l) {
                int nx = i + dx[l], ny = j + dy[l];
                if(nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
                if(grid[nx][ny] == 0 && killer[nx][ny] == 0) add_grid[nx][ny] += grid[i][j] / sur_num[i][j];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            grid[i][j] += add_grid[i][j];
        }
    }
}

int ddx[4] = {1, 1, -1, -1};
int ddy[4] = {1, -1, 1, -1};

void Kill(){
    memset(add_grid, 0, sizeof(add_grid));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(grid[i][j] <= 0) continue;
            add_grid[i][j] += grid[i][j];

            for (int l = 0; l < 4; ++l) {
                int nx = i + ddx[l], ny = j + ddy[l], cnt = 0;

                while(cnt < k) {
                    if (nx < 0 || ny < 0 || nx >= n || ny >= n || grid[nx][ny] <= 0) break;
                    add_grid[nx][ny] += grid[i][j];
                    // cout << "i, j, nx, ny: " << i << ' ' << j << ' ' << nx << ' ' << ny << endl;

                    nx += ddx[l], ny += ddy[l], cnt++;
                }
            }

            // traverse total kill
            /*
            for (int x = 0; x < n; ++x) {
                for (int l = 0; l < n; ++l) {
                    cout << setw(3) << add_grid[x][l] << ' ';
                }
                cout << endl;
            }
            cout << endl;
        */
        }

    }

    // 살포 위치 선정
    int tar_x, tar_y, cnt = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (add_grid[i][j] >= cnt) {
                if (add_grid[i][j] == cnt && tar_x < i) continue;
                if (add_grid[i][j] == cnt && tar_x == i && tar_y < j) continue;

                cnt = add_grid[i][j], tar_x = i, tar_y = j;
            }
        }
    }

        rst += cnt;

    // 이전 살충제 효과 감소
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(killer[i][j] > 0) killer[i][j]--;
        }
    }

    // 살포
    grid[tar_x][tar_y] = 0;
    killer[tar_x][tar_y] = c;

    for (int i = 0; i < 4; ++i) {
        int nx = tar_x + ddx[i], ny = tar_y + ddy[i], rpt = 0;
        while (rpt < k) {
            if (nx < 0 || ny < 0 || nx >= n || ny >= n) break;

            grid[nx][ny] = 0;
            killer[nx][ny] = c;

            nx += ddx[i], ny += ddy[i], rpt++;
        }
    }
}


void run(){
    while(m--){
        Grow();
        // traverse();
        Extend();
        // traverse();
        Kill();
        // traverse();
    }

    cout << rst;
}

int main() {
    // freopen("input.txt", "r", stdin);
    init();
    input();
    run();
    return 0;
}