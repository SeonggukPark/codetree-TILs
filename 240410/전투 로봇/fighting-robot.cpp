#include <iostream>
#include <cstring>
#include <queue>
#define pii pair<int, int>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;

constexpr size_t MAX_N = 20;
int n, cnt;
int grid[MAX_N][MAX_N];
int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

struct Robot{
    int x, y, lev, kill;
};

struct Monster{
    int x, y, dist;
};

struct cmp{
    bool operator()(const Monster & a, const Monster & b){
        if(a.dist != b.dist) return a.dist > b.dist;
        if(a.x != b.x) return a.x > b.x;
        return a.y > b.y;
    }
};

Robot robot;
vector<pii> monster[7];
priority_queue<Monster, vector<Monster>, cmp> pq;

void init(){
    memset(grid, 0, sizeof(grid));
    cnt = 0;
    robot = {0, 0, 2, 0};

    for (int i = 1; i < 7; ++i) {
        monster[i].clear();
    }
}

void input(){
    cin >> n;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];

            if(grid[i][j] == 9) {
                grid[i][j] = 0;
                robot.x = i, robot.y = j;
            }

            if(grid[i][j] >= 1 && grid[i][j] <= 6){
                monster[grid[i][j]].emplace_back(i, j);
            }
        }
    }
}

int min_dist;
bool visited[MAX_N][MAX_N];
pii d;

struct pos{
    int x, y, dep;
};

queue<pos> q;

void bfs(){
    q = {};
    q.push({robot.x, robot.y, 0});
    visited[robot.x][robot.y] = true;

    while(!q.empty()){
        auto cur = q.front();
        q.pop();

        if(d.first == cur.x && d.second == cur.y){
            min_dist = cur.dep;
            break;
        }

        for(int i = 0; i < 4; i++){
            int nx = cur.x + dx[i], ny = cur.y + dy[i];


            if(nx < 0 || ny < 0 || nx >= n || ny >= n || visited[nx][ny]) continue;
            if(grid[nx][ny] > robot.lev) continue;

            visited[nx][ny] = true;
            q.push({nx, ny, cur.dep + 1});
        }
    }
}

void calc_dist(pii target){
    memset(visited, 0, sizeof(visited));
    min_dist = -1;
    d = target;
    bfs();
}

void fill_pq(){
    pq = {};
    int lev = robot.lev;
    if(lev > 7) lev = 7;

    for(int i = 1; i < lev; i++){
        for(auto k : monster[i]){
            if(grid[k.first][k.second] != 0){
                // 거리 계산 ~ dist
                calc_dist(k);

                // 길 없는 경우
                if(min_dist == -1) continue;

                pq.push({k.first, k.second, min_dist});
            }
        }
    }
}

void run(){
    while(true){
        fill_pq();

        // 더 이상 공격할 대상 없으면 종료
        if(pq.empty()) break;

        // 이동 & 제거
        auto target = pq.top();

        cnt += target.dist;
        robot.x = target.x, robot.y = target.y, robot.kill++;
        grid[robot.x][robot.y] = 0;

        if(robot.kill == robot.lev){
            robot.lev++, robot.kill = 0;
        }
    }

    cout << cnt;
}

int main() {
    fastio
    init();
    input();
    run();
    return 0;
}