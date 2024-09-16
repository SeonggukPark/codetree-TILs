#include <iostream>
#include <cstring>
#include <queue>
using namespace std;
constexpr size_t MAX_SIZE = 71;
int grid[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE], is_exit[MAX_SIZE][MAX_SIZE];
int r, c, k, rst, gol_c, gol_d;
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

struct Pos{
    int r, c;
};

Pos south[3] = {{2, 0}, {1, 1}, {1, -1}};
Pos west[5] = {{-1, -1}, {0, -2}, {1, -1}, {1, -2}, {2, -1}};
Pos east[5] = {{-1, 1}, {0, 2}, {1, 1}, {1, 2}, {2, 1}};

bool is_movable(const Pos & cur, int dir){
    switch (dir) {
        case 0: // south
            for(auto & i : south){
                int nr = cur.r + i.r, nc = cur.c + i.c;
                if(grid[nr][nc] != 0) return false;
            }
            break;

        case 1: // west
            for(auto & i : west) {
                int nr = cur.r + i.r, nc = cur.c + i.c;
                if (grid[nr][nc] != 0) return false;
            }
            break;

        case 2: // east
            for(auto & i : east) {
                int nr = cur.r + i.r, nc = cur.c + i.c;
                if (grid[nr][nc] != 0) return false;
            }
            break;
        default:
            cout << "exception error.. " << endl;
            break;
    }
    return true;
}


void solve(){
    memset(grid, 0, sizeof(grid));
    memset(is_exit, false, sizeof(is_exit));

    rst = 0;
    cin >> r >> c >> k;

    for (int i = 1; i <= k; ++i) {
        cin >> gol_c >> gol_d;
        Pos cur = {-1, gol_c}; // r, c
        bool is_going = true;

        while(is_going){
            if(cur.r <= r - 2 && is_movable(cur, 0)){
                cur.r++;
                continue;
            }

            if(cur.r <= r - 2 && cur.c >= 3 && is_movable(cur, 1)){
                cur.r++, cur.c--;
                gol_d = (gol_d + 3) % 4;
                continue;
            }

            if(cur.r <= r - 2 && cur.c <= c - 2 && is_movable(cur, 2)){
                cur.r++, cur.c++;
                gol_d = (gol_d + 1) % 4;
                continue;
            }

            is_going = false;
        }

        if(cur.r <= 1){
            memset(grid, 0, sizeof(grid));
            memset(is_exit, false, sizeof(is_exit));
            continue;
        }

        grid[cur.r][cur.c] = i;
        for (int j = 0; j < 4; ++j) {
            grid[cur.r + dr[j]][cur.c + dc[j]] = i;
        }
        is_exit[cur.r + dr[gol_d]][cur.c + dc[gol_d]] = true;

        memset(visited, false, sizeof(visited));
        queue<Pos> q;
        q.push({cur.r, cur.c});
        int max_r = cur.r;
        visited[cur.r][cur.c] = true;

        while(!q.empty()){
            cur = q.front();
            q.pop();
            if(max_r < cur.r) max_r = cur.r;

            for (int j = 1; j < 4; ++j) {
                int nr = cur.r + dr[j], nc = cur.c + dc[j];

                if(nr > r || nr < 1 || nc > c || nc < 1) continue;
                if(grid[nr][nc] == 0 || visited[nr][nc]) continue;
                if(grid[nr][nc] == grid[cur.r][cur.c]){
                    q.push({nr, nc});
                    visited[nr][nc] = true;
                    continue;
                }

                if(is_exit[cur.r][cur.c]){
                    q.push({nr, nc});
                    visited[nr][nc] = true;
                }
            }
        }

        rst += max_r;
    }

    cout << rst;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}