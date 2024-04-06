#include <iostream>
#include <cstring>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_SIZE = 50;
int n, m, cnt;
int sx, sy, d;
int map[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];

int dx[4] = {-1, 0, 1, 0}; // N, E, S, W
int dy[4] = {0, 1, 0, -1};

void init(){
    memset(map, 0, sizeof(map));
    memset(visited, false, sizeof(visited));
    cnt = 1;
}

void input(){
    cin >> n >> m;
    cin >> sx >> sy >> d; // d: 0 ~ 3 -> N, E, S, W
    visited[sx][sy] = true;

    for(int i = 0; i < n; ++i){
        for (int j = 0; j < m; ++j) {
            cin >> map[i][j];
        }
    }
}

void run(int x, int y, int dir){
    while(true) {
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[(dir + 3 - i) % 4], ny = y + dy[(dir + 3 - i) % 4];

            if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
            if (visited[nx][ny] || map[nx][ny]) continue;

            visited[nx][ny] = true;
            cnt++;
            x = nx, y = ny, dir = (dir + 3 - i) % 4, i = -1;
        }

        if(map[x - dx[dir]][y - dy[dir]]) break;
        x -= dx[dir], y -= dy[dir];
    }
}


int main() {
    fastio
    init();
    input();
    run(sx, sy, d);
    cout << cnt << endl;

    return 0;
}