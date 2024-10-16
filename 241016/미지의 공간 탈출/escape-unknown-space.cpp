#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define pii pair<int, int>
#define x first
#define y second
#define endl '\n'

struct DIM_3{
    int dim, r, c;
};

struct FIRE{
    int r, c, d, v;
};

constexpr size_t MAX_N = 20, MAX_M = 10;
int N, M, F, tot_turn;
int grid_2[MAX_N][MAX_N], grid_3[5][MAX_M][MAX_M];
int visit_2[MAX_N][MAX_N], fire_map[MAX_N][MAX_N], visit_3[5][MAX_M][MAX_M];
int dx[4] = {0, 0, 1, -1}; // 동, 서, 남, 북
int dy[4] = {1, -1, 0, 0};

vector<FIRE> fires;
DIM_3 cur_3, exit_3;
pii cur_2, exit_2;

queue<DIM_3> q_3;
queue<pii> q_2;

// ----------------------------------------------------------------

bool is_valid_2(int nx, int ny){
    if (nx < 0 || ny < 0 || nx >= N || ny >= N) return false;
    return true;
}

bool is_valid_3(int nx, int ny){
    if (nx < 0 || ny < 0 || nx >= M || ny >= M) return false;
    return true;
}

void init(){
    tot_turn = 0;
    pii m_br, m_tl = {-1, -1};
    cin >> N >> M >> F;
    for (int i = 0; i < N; ++i) {
        fill(visit_2[i], visit_2[i] + N, -1);
        fill(fire_map[i], fire_map[i] + N, -1);
        for (int j = 0; j < N; ++j) {
            cin >> grid_2[i][j];

            // 산의 좌상단 좌표, 우하단 좌표 구하기
            if(grid_2[i][j] == 3 && m_tl == make_pair(-1, -1)) m_tl = {i, j}, m_br = {i + M - 1, j + M - 1};
            // 2차원 탈출 좌표 구하기
            if(grid_2[i][j] == 4) exit_2 = {i, j};
        }
    }

    for(int dim = 0; dim < 5; ++dim) {
        for (int i = 0; i < M; ++i) {
            fill(visit_3[dim][i], visit_3[dim][i] + M, -1);
            for (int j = 0; j < M; ++j) {
                cin >> grid_3[dim][i][j];
                if(grid_3[dim][i][j] == 2) cur_3 = {dim, i, j};
            }
        }
    }

    fires.clear();
    fires.resize(F);
    for (int i = 0; i < F; ++i) {
        cin >> fires[i].r >> fires[i].c >> fires[i].d >> fires[i].v;
    }

    // 2차원 시작 좌표 구하기 (cur_2)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if(grid_2[i][j] == 3){
                for(int k = 0; k < 4; ++k){
                    int nx = i + dx[k], ny = j + dy[k];
                    if(!is_valid_2(nx, ny)) continue;
                    if(grid_2[nx][ny] == 0) cur_2 = {nx, ny};
                }
            }
        }
    }

    // 3차원 탈출 좌표 구하기 (exit_3)
    if(cur_2.x > m_br.x) exit_3 = {2, M - 1, cur_2.y - m_tl.y}; // 남
    if(cur_2.x < m_tl.x) exit_3 = {3, M - 1, M - 1 - (cur_2.y - m_tl.y)}; // 북
    if(cur_2.y > m_br.y) exit_3 = {0, M - 1, M - 1 - (cur_2.x - m_tl.x)}; // 동
    if(cur_2.y < m_tl.y) exit_3 = {1, M - 1, cur_2.x - m_tl.x}; // 서
}

DIM_3 change_dim(int dim, int row, int col){
    DIM_3 tmp = {dim, row, col};

    switch (dim) {
        case 0: // 동
            if(row >= M) tmp = {-1, -1, -1}; // ERROR
            if(row < 0) tmp = {4, M - 1 - col, M - 1}; // 윗
            if(col >= M) tmp = {3, row, 0}; // 북
            if(col < 0) tmp = {2, row, M - 1}; // 남
            break;

        case 1: // 서
            if(row >= M) tmp = {-1, -1, -1}; // ERROR
            if(row < 0) tmp = {4, col, 0}; // 윗
            if(col >= M) tmp = {2, row, 0}; // 남
            if(col < 0) tmp = {3, row, M - 1}; // 북
            break;

        case 2: // 남
            if(row >= M) tmp = {-1, -1, -1}; // ERROR
            if(row < 0) tmp = {4, M - 1, col}; // 윗
            if(col >= M) tmp = {0, row, 0}; // 동
            if(col < 0) tmp = {1, row, M - 1}; // 서
            break;

        case 3: // 북
            if(row >= M) tmp = {-1, -1, -1}; // ERROR
            if(row < 0) tmp = {4, 0, M - 1 - col}; // 윗
            if(col >= M) tmp = {1, row, 0}; // 서
            if(col < 0) tmp = {0, row, M - 1}; // 동
            break;

        case 4: // 윗
            if(row >= M) tmp = {2, 0, col}; // 남
            if(row < 0) tmp = {3, 0, M - 1 - col}; // 북
            if(col >= M) tmp = {0, 0, M - 1 - row}; // 동
            if(col < 0) tmp = {1, 0, row}; // 서
            break;
    }
    return tmp;
}

void escape_3dim(){
    q_3 = {};
    q_3.push(cur_3);
    visit_3[cur_3.dim][cur_3.r][cur_3.c] = 0;

    DIM_3 tmp;
    while(!q_3.empty()){
        DIM_3 top = q_3.front(); q_3.pop();

        if(top.dim == exit_3.dim && top.r == exit_3.r && top.c == exit_3.c){
            tot_turn = visit_3[top.dim][top.r][top.c];
            return;
        }

        for(int dir = 0; dir < 4; ++dir){
            int nx = top.r + dx[dir], ny = top.c + dy[dir];
            if(!is_valid_3(nx, ny)) tmp = change_dim(top.dim, nx, ny);
            else tmp = {top.dim, nx, ny};
            if(grid_3[tmp.dim][tmp.r][tmp.c] == 1) continue;
            // cout << "tmp: " << tmp.dim << ' ' << tmp.r << ' ' << tmp.c << endl;

            if(tmp.dim == -1 || visit_3[tmp.dim][tmp.r][tmp.c] >= 0) continue;
            visit_3[tmp.dim][tmp.r][tmp.c] = visit_3[top.dim][top.r][top.c] + 1;
            q_3.push(tmp);
        }
    }
    tot_turn = -1;
}

void spread_fire(){
    for(FIRE fire : fires){
        int cur_t = 0, nx = fire.r, ny = fire.c, dir = fire.d, aff = fire.v;
        while(true){
            if(!is_valid_2(nx, ny) || grid_2[nx][ny] != 0) break;
            if((fire_map[nx][ny] != -1 && fire_map[nx][ny] > cur_t) || fire_map[nx][ny] == -1){
                fire_map[nx][ny] = cur_t;
            }
            cur_t += aff, nx += dx[dir], ny += dy[dir];
        }
    }
}

void escape_2dim(){
    tot_turn++;
    q_2 = {};
    q_2.push({cur_2.x, cur_2.y});
    visit_2[cur_2.x][cur_2.y] = tot_turn;

    while(!q_2.empty()){
        pii top = q_2.front(); q_2.pop();
        if(top == exit_2) {
            tot_turn = visit_2[top.x][top.y];
            return;
        }

        for(int dir = 0; dir < 4; ++dir){
            int nx = top.x + dx[dir], ny = top.y + dy[dir];
            if(!is_valid_2(nx, ny) || visit_2[nx][ny] != -1) continue;
            if(fire_map[nx][ny] != -1 && fire_map[nx][ny] <= visit_2[top.x][top.y] + 1) continue;
            if(grid_2[nx][ny] == 1 || grid_2[nx][ny] == 3) continue;

            visit_2[nx][ny] = visit_2[top.x][top.y] + 1;
            q_2.push({nx, ny});
        }
    }
    tot_turn = -1;
}

void solve(){
    escape_3dim(); // 3차원 탈출 시도
    if(tot_turn == -1) return; // 큐브 탈출 못한 경우 바로 리턴
    spread_fire(); // 불 확산
    escape_2dim(); // 2차원 탈출 시도
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // freopen("input.txt", "r", stdin);
    init();
    solve();
    cout << tot_turn;
    return 0;
}