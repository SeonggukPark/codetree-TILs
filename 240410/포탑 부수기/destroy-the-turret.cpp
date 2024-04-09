#include <iostream>
#include <cstring>
#include <iomanip>
#include <queue>
#include <stack>
#define pii pair<int, int>
using namespace std;
constexpr size_t MAX_SIZE = 10;
int n, m, k, tower_cnt;
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
    tower_cnt = 0;
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
            cout << setw(2) << grid[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void phase_1(){ // 1. 공격자, 피해자 선정
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

    is_connect[at.x][at.y] = is_connect[df.x][df.y] = true;
}

struct info{
    int x, y, dep;
    stack<pos> trace;
};

int min_dist;
bool visited[MAX_SIZE][MAX_SIZE];
stack<pos> trace;

bool raser(){ // 공격 성공시 true 반환
    memset(visited, false, sizeof(visited));
    trace = {};
    bool flag = false;
    min_dist = -1;

    queue<info> q;
    q.push({at.x, at.y, 0, trace});

    while(!q.empty()){
        auto cur = q.front();
        q.pop();

        if(cur.x == df.x && cur.y == df.y){
            min_dist = cur.dep;
            flag = true;

            cur.trace.pop();
            trace = cur.trace;

            // cout << "min_dist: " << min_dist << endl;
            break;
        }

        for(int i = 0; i < 4; i++){
            int nx = (cur.x + dx[i]) % n, ny = (cur.y + dy[i]) % m;
            if(nx < 0 || ny < 0 || grid[nx][ny] == 0) continue;
            cur.trace.push({nx, ny});
            q.push({nx, ny, cur.dep + 1, cur.trace});
            cur.trace.pop();
        }
    }

    if(!flag) return false;

    // 경로 확보 시 공격
    while(!trace.empty()){
        grid[trace.top().x][trace.top().y] -= grid[at.x][at.y] / 2;
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

        grid[nx][ny] -= grid[at.x][at.y] / 2;
    }
}

void phase_2(){ // 2. 공격자의 공격
    grid[df.x][df.y] -= grid[at.x][at.y];

    bool rst = raser();

    // 레이저 공격 실패 시 폭탄 공격
    if(!rst) bomb();
}

void phase_3(){ // 3. 포탑 파괴
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if(grid[i][j] <= 0){
                grid[i][j] = 0;
                tower_cnt--;
            }
        }
    }
}

void phase_4(){ // 4. 포탑 정비
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            // 무관했으며 부서지지 않은 포탑 공격력 1 증가
            if(!is_connect[i][j] && grid[i][j] != 0) grid[i][j]++;
        }
    }
}

void run(){
    // 부서지지 않은 포탑 1개 이상일 경우 k번 반복
    while(k-- && tower_cnt > 1){
        // 정비 대상 선택 위한 배열 초기화
        memset(is_connect, false, sizeof(is_connect));

        phase_1(); // 1. 공격자, 피해자 선정
        phase_2(); // 2. 공격자의 공격
        phase_3(); // 3. 포탑 파괴
        phase_4(); // 4. 포탑 정비
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
    init();
    input();
    // traverse();
    run();

    return 0;
}