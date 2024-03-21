#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

typedef pair<int, int> pii;

struct cmp{
    bool operator()(const pair<int, pair<int, int>> & a, const pair<int, pair<int, int>> & b){
        if(a.first != b.first) return a.first > b.first;
        if(a.second.first != b.second.first) return a.second.first > b.second.first;
        return a.second.second > b.second.second;
    }
};

// 변수 선언
constexpr size_t MAX_N = 11;
constexpr size_t MAX_M = 11;
int N, M, K; // N: 미로 크기, M: 참가자 수, K: 게임 시간
int cur_time; // 현재 시간
int maze[MAX_N][MAX_N]; // 현재 미로 상태
set<int> s, g; // 현재 미로에 남아있는 인원 관리
int dist_sum; // 이동 거리 합
pii escape; // 출구 좌표
priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, cmp> min_dist; // 회전 크기 지정 위한 pq
pii mans[MAX_M];

// 초기화 함수
void init(){
    memset(maze, 0, sizeof(maze));
    dist_sum = 0;
    s.clear();
    cur_time = 1;
}

void input(){
    int x , y;
    cin >> N >> M >> K;
    // maze, mans, escape 저장

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> maze[i][j];
        }
    }

    for (int i = 0; i < M; ++i) {
        cin >> x >> y;
        maze[x - 1][y - 1] = i + 10;
        mans[i] = {x - 1, y - 1};
        s.insert(i);
    }

    cin >> escape.first >> escape.second;
    escape.first -= 1, escape.second -= 1;
    maze[escape.first][escape.second] = -1;
}


// map 회전
void clock_wise(pair<int, pair<int, int>> mini){
    pii pos = mini.second;
    pii lu_corner;

    // 회전 반경, 좌상단 좌표 구하기
    pos.first > escape.first ?
            lu_corner.first = pos.first - mini.first : lu_corner.first = escape.first - mini.first;

    pos.second > escape.second ?
            lu_corner.second = pos.second - mini.first : lu_corner.second = escape.second - mini.first;

    if(lu_corner.first < 0) lu_corner.first = 0;
    if(lu_corner.second < 0) lu_corner.second = 0;

    int tmp[MAX_N][MAX_N] = {0};

    for(int i = 0; i <= mini.first; i++){
        for (int j = 0; j <= mini.first; ++j) {
            tmp[i][j] = maze[lu_corner.first + (mini.first - j)][lu_corner.second + i];
        }
    }


    for(int i = 0; i <= mini.first; i++){
        for (int j = 0; j <= mini.first; ++j) {
            if(tmp[i][j] >= 10) mans[tmp[i][j] - 10] = {lu_corner.first + i, lu_corner.second + j};
            if(tmp[i][j] > 0 && tmp[i][j] < 10) tmp[i][j] -= 1;
            maze[lu_corner.first + i][lu_corner.second + j] = tmp[i][j];
            if(tmp[i][j] == -1) escape = {lu_corner.first + i, lu_corner.second + j};
        }
    }
}

void move_shortest(){
    bool flag;
    pii person;
    g.clear();

    for(int i : s) {
        person = mans[i];
        int up_down = person.first - escape.first;
        int r_l = person.second - escape.second;

        flag = false;

        // 상하 check
        if (up_down != 0) {
            if (up_down > 0 && maze[person.first - 1][person.second] <= 0) {
                mans[i].first -= 1;
                maze[person.first][person.second] = 0;
                maze[person.first - 1][person.second] = i + 10;
                dist_sum += 1;
                flag = true;
            }

            if (up_down < 0 && maze[person.first + 1][person.second] <= 0) {
                mans[i].first += 1;
                maze[person.first][person.second] = 0;
                maze[person.first + 1][person.second] = i + 10;
                dist_sum += 1;
                flag = true;
            }
        }

            // 상하 이동 안되면 좌우 체크
         if (r_l != 0 && !flag) {
             if (r_l > 0 && maze[person.first][person.second - 1] <= 0) {
                 mans[i].second -= 1;
                 maze[person.first][person.second] = 0;
                 maze[person.first][person.second - 1] = i + 10;
                 dist_sum += 1;
             }

             if (r_l < 0 && maze[person.first][person.second + 1] <= 0) {
                 mans[i].second += 1;
                 maze[person.first][person.second] = 0;
                 maze[person.first][person.second + 1] = i + 10;
                 dist_sum += 1;
             }
         }

        // 탈출구 도착하면
        if (mans[i].first == escape.first && mans[i].second == escape.second) {
            maze[mans[i].first][mans[i].second] = -1;
            g.insert(i);
        }
    }

    for(int i : g) s.erase(i);
}

void run(){
    init();
    input();

    while(cur_time <= K && !s.empty()){
        move_shortest();
        min_dist = {};
        for (int i : s) {
            min_dist.push({max(abs(mans[i].first - escape.first),
                               abs(mans[i].second - escape.second)), {mans[i].first, mans[i].second}});
        }

        clock_wise(min_dist.top());
        cur_time++;
    }

    cout << dist_sum << endl;
    cout << escape.first + 1 << " " << escape.second + 1;
}

int main(){
    // freopen("input.txt", "r", stdin);
    run();
    return 0;
}