#include <iostream>
#include <cstring>
#include <queue>
#include <unordered_set>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_N = 16;
constexpr size_t MAX_M = 31;
int N, M, cur_t;
int grid[MAX_N][MAX_N];
int dx[4] = {-1, 0, 0, 1}; // 상, 좌, 우, 하 순 (1번 우선순위)
int dy[4] = {0, -1, 1, 0};

struct Pos{
    int x, y;
};

Pos person[MAX_M], conv[MAX_M]; // 1 ~ M번 사람 위치
unordered_set<int> alive, still_alive;

void init(){
    memset(grid, 0, sizeof(grid));
    memset(conv, 0, sizeof(conv));
    alive.clear();
    still_alive.clear();
    cur_t = 0;
}

void input(){
    cin >> N >> M;

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            cin >> grid[i][j];
        }
    }

    for (int i = 1; i <= M; ++i) {
        cin >> conv[i].x >> conv[i].y;
    }
}

bool visited[MAX_N][MAX_N];

void phase_1() {
    // alive 순환하면서 희망 편의점 까지 최단 거리 방향 이동
    queue<pair<Pos, int>> q;

    for (auto p: alive) {
        memset(visited, false, sizeof(visited));
        q = {};
        int per_x = person[p].x, per_y = person[p].y;

        for(int i = 0; i < 4; i++){
            int nx = per_x + dx[i], ny = per_y + dy[i];

            if(nx < 1 || ny < 1 || nx > N || ny > N || grid[nx][ny] == -1)  continue;

            visited[nx][ny] = true;
            q.push({{nx, ny}, i});
        }


        while(!q.empty()){
            auto target = q.front();
            q.pop();
            int x = target.first.x, y = target.first.y;

            if(x == conv[p].x && y == conv[p].y){ // 해당 지역이 희망 편의점일 경우
                person[p].x += dx[target.second], person[p].y += dy[target.second]; // 사람 이동
                break;
            }

            for(int i = 0; i < 4; i++){
                int nx = x + dx[i], ny = y + dy[i];
                if(nx < 1 || ny < 1 || nx > N || ny > N || visited[nx][ny] || grid[nx][ny] == -1)  continue;

                visited[nx][ny] = true;
                q.push({{nx, ny}, target.second});
            }
        }
    }
}

void phase_2(){
    still_alive = {};

    // alive 순환하면서 현재 위치가 희망 편의점이랑 일치하면 해당 위치 lock
    for(auto p : alive){
        if(person[p].x == conv[p].x && person[p].y == conv[p].y){
            grid[conv[p].x][conv[p].y] = -1;
            continue;
        }
        still_alive.insert(p);
    }

    // alive에서 해당 사람 제거
    alive = still_alive;
}


void phase_3(){
    // bfs로 들어가는 사람 희망 편의점 기준 최단 거리 베이스캠프 찾아서 들어가기
    queue<pair<Pos, int>> q;
    memset(visited, false, sizeof(visited));
    int conv_x = conv[cur_t].x, conv_y = conv[cur_t].y;
    q.push({{conv_x, conv_y}, 0});

    while(!q.empty()){
        auto target = q.front();
        q.pop();
        int x = target.first.x, y = target.first.y;

        if(grid[x][y] == 1){ // 해당 지역이 베이스캠프 일 경우

            while(!q.empty()){
                auto comp = q.front();
                q.pop();

                // 같은 depth 애들 다 뽑아 보기
                if(comp.second != target.second) break;

                // row 비교
                if(target.first.x > comp.first.x){
                    x = comp.first.x, y = comp.first.y;
                }

                // col 비교
                if(target.first.x == comp.first.x && target.first.y > comp.first.y){
                    x = comp.first.x, y = comp.first.y;
                }
            }

            grid[x][y] = -1; // 해당 베이스 캠프는 이제 못 지남
            person[cur_t].x = x, person[cur_t].y = y;
            break;
        }

        for(int i = 0; i < 4; i++){
            int nx = x + dx[i], ny = y + dy[i];
            if(nx < 1 || ny < 1 || nx > N || ny > N || visited[nx][ny] || grid[nx][ny] == -1)  continue;

            visited[nx][ny] = true;
            q.push({{nx, ny}, target.second + 1});
        }
    }

    // alive에 cur_t 번 사람 넣기
    alive.insert(cur_t);
}

void run(){
    while(cur_t < M) { // M초 까지
        cur_t++;
        phase_1();
        phase_2();
        phase_3();
    }

    while(!alive.empty()){
        cur_t++;
        phase_1();
        phase_2();
    }

    cout << cur_t;
}


int main() {
    fastio
    // freopen("input.txt", "r", stdin);
    init();
    input();
    run();
    return 0;
}