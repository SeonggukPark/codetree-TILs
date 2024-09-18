#include <iostream>
#include <deque>
#include <queue>
#include <cstring>
#define pii pair<int, int>
#define endl '\n'
#define x first
#define y second
using namespace std;
int grid[5][5], rot_grid[5][5];

struct Node{
    int val, deg, r, c;
};

struct cmp{
    bool operator()(const Node & a, const Node & b){
        if(a.val != b.val) return a.val < b.val;
        if(a.deg != b.deg) return a.deg > b.deg;
        if(a.c != b.c) return a.c > b.c;
        return a.r > b.r;
    }
};

void rotate(pii pos, int deg, bool is_original){
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            rot_grid[i][j] = grid[i][j];
        }
    }

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            switch (deg) {
                case 1:
                    rot_grid[pos.x + i][pos.y + j] = grid[pos.x - j][pos.y + i];
                    break;
                case 2:
                    rot_grid[pos.x + i][pos.y + j] = grid[pos.x - i][pos.y - j];
                    break;
                case 3:
                    rot_grid[pos.x + i][pos.y + j] = grid[pos.x + j][pos.y - i];
                    break;
                default:
                    cout << "error occured.." << endl;
                    break;
            }
        }
    }

    /*
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            cout << rot_grid[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
     */

    if(is_original){
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                grid[i][j] = rot_grid[i][j];
            }
        }
    }
}

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
int bfs(bool is_original){
    bool visited[5][5];
    memset(visited, false, sizeof(visited));
    int acc = 0;
    queue<pii> q, seq;

    if(!is_original){
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if(visited[i][j]) continue;
                visited[i][j] = true;
                q.push({i, j});
                int tmp_acc = 1;

                while(!q.empty()){
                    auto top = q.front();
                    q.pop();

                    for (int k = 0; k < 4; ++k) {
                        int nx = top.x + dx[k], ny = top.y + dy[k];
                        if(nx < 0 || ny < 0 || nx >= 5 || ny >= 5) continue;
                        if(visited[nx][ny] || rot_grid[nx][ny] != rot_grid[top.x][top.y]) continue;
                        visited[nx][ny] = true;
                        q.push({nx, ny});
                        tmp_acc++;
                    }
                }

                if(tmp_acc >= 3) acc += tmp_acc;
            }
        }

        return acc;
    }

    else{
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                // cout << "i, j: " << i << ' ' << j << endl;
                seq = {};
                if(visited[i][j]) continue;
                visited[i][j] = true;
                q.emplace(i, j), seq.emplace(i, j);
                int tmp_acc = 1;

                while(!q.empty()){
                    auto top = q.front();
                    q.pop();

                    for (int k = 0; k < 4; ++k) {
                        int nx = top.x + dx[k], ny = top.y + dy[k];
                        if(nx < 0 || ny < 0 || nx >= 5 || ny >= 5) continue;
                        if(visited[nx][ny] || grid[nx][ny] != grid[top.x][top.y]) continue;
                        visited[nx][ny] = true;

                        // cout << "nx, ny: " << nx << ' ' << ny << endl;
                        q.emplace(nx, ny), seq.emplace(nx, ny);
                        tmp_acc++;
                    }
                }

                if(tmp_acc >= 3) {
                    acc += tmp_acc;
                    while(!seq.empty()){
                        auto top = seq.front();
                        // cout << top.x << ' ' << top.y << endl;
                        seq.pop();
                        grid[top.x][top.y] = 0;
                    }
                }
            }
        }
        return acc;
    }
}

void solve(){
    int K, M;
    cin >> K >> M;
    deque<int> deq(M);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            cin >> grid[i][j];
        }
    }

    for (int i = 0; i < M; ++i) {
        cin >> deq[i];
    }


    priority_queue<Node, vector<Node>, cmp> pq;

    for (int i = 0; i < K; ++i) {
        // 1. 탐사 진행
        int cur_val, rst = 0;
        pii pos = {1, 1};
        pq = {};

        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                pii cur_pos = {pos.x + r, pos.y + c};
                for (int deg = 1; deg <= 3; ++deg) {
                    // cout << "r, c, deg, val: " << cur_pos.x << ' ' << cur_pos.y << ' ' << deg << endl;
                    rotate(cur_pos, deg, false);
                    cur_val = bfs(false);
                    // cout << "cur_val: " << cur_val << endl;
                    pq.push({cur_val, deg, cur_pos.x, cur_pos.y});
                }
            }
        }

        Node max_node = pq.top();
        if(max_node.val == 0) break;

       // cout << "max_node: " << max_node.val << ' ' <<max_node.r << ' ' <<max_node.c << ' ' << max_node.deg << endl;

        // 2. 유물 획득
        // 실제로 격자 반영
        rotate({max_node.r, max_node.c}, max_node.deg, true);

        while(true) {
            cur_val = bfs(true);
            if(cur_val == 0) break;
            rst += cur_val;

            for (int c = 0; c < 5; ++c) {
                for (int r = 4; r >= 0; --r) {
                    if (grid[r][c] == 0) {
                        grid[r][c] = deq.front();
                        deq.pop_front();
                    }
                }
            }
        }

        // 해당 턴 유물 가치 총합 출력
        cout << rst << ' ';
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();

    return 0;
}