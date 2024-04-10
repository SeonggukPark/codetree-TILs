#include <iostream>
#include <cstring>
#include <iomanip>
#include <queue>
#include <stack>
#define pii pair<int, int>
using namespace std;
constexpr size_t MAX_SIZE = 10;
int n, m, k, tower_cnt, time_cnt;
int grid[MAX_SIZE][MAX_SIZE];
int grid_time[MAX_SIZE][MAX_SIZE];
bool is_connect[MAX_SIZE][MAX_SIZE];

int dx[4] = {0, 1, 0, -1}; // 우, 하, 좌, 상 순
int dy[4] = {1, 0, -1, 0};

int bdx[8] = {0, 0, 1, 1, 1, -1, -1, -1};
int bdy[8] = {1, -1, 1, 0 ,-1, 1, 0, -1};

struct pos{
    int x, y;
};

pos at, df;

struct Tower{
    int x, y, damage, attack_t;
};

struct cmp_a{
    bool operator()(const Tower & a, const Tower & b){
         if(a.damage != b.damage) return a.damage > b.damage;
         if(a.attack_t != b.attack_t) return a.attack_t < b.attack_t;
         if(a.x + a.y != b.x + b.y) return a.x + a.y < b.x + b.y;
         return a.y < b.y;
    }
};

struct cmp_d{
    bool operator()(const Tower & a, const Tower & b){
        if(a.damage != b.damage) return a.damage < b.damage;
        if(a.attack_t != b.attack_t) return a.attack_t > b.attack_t;
        if(a.x + a.y != b.x + b.y) return a.x + a.y > b.x + b.y;
        return a.y > b.y;
    }
};

priority_queue<Tower, vector<Tower>, cmp_a> pq_a;
priority_queue<Tower, vector<Tower>, cmp_d> pq_d;

void init(){
    memset(grid, 0, sizeof(grid));
    memset(grid_time, 0, sizeof(grid_time));
    memset(is_connect, false, sizeof(is_connect));
    tower_cnt = 0, time_cnt = 0;
}

void input(){
    cin >> n >> m >> k;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
            if(grid[i][j] != 0) tower_cnt++;
        }
    }
}

void traverse(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << setw(4) << grid[i][j] << ' ';
        }
        cout << endl;
    }
    cout << "tower cnt: " << tower_cnt << endl;

}

void phase_1(){ // 1. 공격자, 피해자 선정
    pq_a = {};
    pq_d = {};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if(grid[i][j] != 0) {
                pq_a.push({i, j, grid[i][j], grid_time[i][j]});
                pq_d.push({i, j, grid[i][j], grid_time[i][j]});
            }
        }
    }

    at = {pq_a.top().x, pq_a.top().y}, df = {pq_d.top().x, pq_d.top().y};

    // 공격자 공격력 올려주기
    grid[at.x][at.y] += (n + m);

    grid_time[at.x][at.y] = time_cnt;

    is_connect[at.x][at.y] = is_connect[df.x][df.y] = true;

    // cout << "at, df: " << at.x << ' ' << at.y << ' ' << df.x << ' ' << df.y << endl;
}

struct info{
    int x, y, dep;
    stack<pos> trace;
};

bool visited[MAX_SIZE][MAX_SIZE];
stack<pos> trace;

bool raser(){ // 공격 성공시 true 반환
    memset(visited, false, sizeof(visited));
    trace = {};
    bool flag = false;

    queue<info> q;
    q.push({at.x, at.y, 0, trace});
    visited[at.x][at.y] = true;

    while(!q.empty()){
        auto cur = q.front();
        q.pop();

        if(cur.x == df.x && cur.y == df.y){
            // cout << "arrive: " << cur.x << ' ' << cur.y << endl;
            flag = true;

            if(!cur.trace.empty()) cur.trace.pop();
            trace = cur.trace;
            break;
        }

        for(int i = 0; i < 4; i++){
            int nx = (cur.x + dx[i] + n) % n, ny = (cur.y + dy[i] + m) % m;
            // cout << "nx, ny: " << nx << ' ' << ny << endl;
            if(nx < 0 || ny < 0 || grid[nx][ny] == 0 || visited[nx][ny]) continue;
            // cout << "alive nx, ny: " << nx << ' ' << ny << endl;

            cur.trace.push({nx, ny});
            visited[nx][ny] = true;
            q.push({nx, ny, cur.dep + 1, cur.trace});
            cur.trace.pop();
        }
    }


    if(!flag) return false;

    // 경로 확보 시 공격
    while(!trace.empty()){
        // cout << trace.top().x << ' ' << trace.top().y << endl;

        grid[trace.top().x][trace.top().y] -= (grid[at.x][at.y] / 2);

        if(grid[trace.top().x][trace.top().y] <= 0){
            grid[trace.top().x][trace.top().y] = 0;
            tower_cnt--;
        }

        is_connect[trace.top().x][trace.top().y] = true;
        trace.pop();
    }

    return true;
}

void bomb(){
    // 폭탄 공격
    for(int i = 0; i < 8; i++){
        int nx = (df.x + bdx[i]) % n, ny = (df.y + bdy[i]) % m;
        if(nx < 0 || ny < 0 || grid[nx][ny] == 0) continue;
        if(at.x == nx && at.y == ny) continue; // 공격자는 영향 X

        grid[nx][ny] -= (grid[at.x][at.y] / 2);

        if(grid[nx][ny] <= 0){
            grid[nx][ny] = 0;
            tower_cnt--;
        }

        is_connect[nx][ny] = true;
    }
}

void phase_2(){ // 2. 공격자의 공격
    bool rst = raser();

    // 레이저 공격 실패 시 폭탄 공격
    if(!rst) {
        // cout << "attck with bomb.. " << endl;
        bomb();
    }

    grid[df.x][df.y] -= grid[at.x][at.y];

    if(grid[df.x][df.y] <= 0){
        grid[df.x][df.y] = 0;
        tower_cnt--;
    }
}

void phase_3(){ // 3. 포탑 정비
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            // 무관했으며 부서지지 않은 포탑 공격력 1 증가
            if(!is_connect[i][j] && grid[i][j] != 0) grid[i][j]++;
        }
    }
}

void run(){
    // 부서지지 않은 포탑 1개 이상일 경우 k번 반복
    while(k--){
        time_cnt++;

        // 정비 대상 선택 위한 배열 초기화
        memset(is_connect, false, sizeof(is_connect));

        phase_1(); // 1. 공격자, 피해자 선정
        phase_2(); // 2. 공격자의 공격

        if(tower_cnt <= 1) break;

        phase_3(); // 3. 포탑 정비

        // traverse();
    }

    int rst = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            rst = max(rst, grid[i][j]);
        }
    }
    cout << rst;
}

int main() {
    //freopen("input.txt", "r", stdin);
    init();
    input();
    // traverse();
    run();

    return 0;
}