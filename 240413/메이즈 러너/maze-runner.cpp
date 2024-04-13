#include <iostream>
#include <cstring>
#include <unordered_set>
using namespace std;
constexpr size_t MAX_N = 11, MAX_M = 11;

int N, M, K, rst;
int grid[MAX_N][MAX_N], cp_other[MAX_N][MAX_N], to_other[MAX_N][MAX_N], other_grid[MAX_N][MAX_N], cp_grid[MAX_N][MAX_N], to_grid[MAX_N][MAX_N];
int dist_arr[MAX_M];
int dx[4] = {-1, 1, 0, 0}; // 상, 하, 좌, 우
int dy[4] = {0, 0, -1, 1};

struct Pos{
    int x, y;
};

Pos escape, players[MAX_M];
unordered_set<int> alive_players; // 생존자 관리

void traverse_grid(){
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            cout << grid[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void traverse_player(){
    cout << "Escape: ";
    cout << escape.x << ' ' << escape.y << endl;

    for (int i = 1; i <= M; ++i) {
        cout << "Player " << i << ": ";
        cout << players[i].x << ' '<< players[i].y << endl;
    }

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            cout << other_grid[i][j] << ' ';
        }
        cout << endl;
    }
}

void init(){
    rst = 0;
    memset(grid, 0, sizeof(grid));
    memset(other_grid, 0, sizeof(other_grid));
    memset(dist_arr, 0, sizeof(dist_arr));
    alive_players.clear();
    escape.x = escape.y = 0;

    for(auto & player : players){
        player.x = player.y = 0;
    }
}

void input(){
    cin >> N >> M >> K;

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            cin >> grid[i][j];
        }
    }

    for (int i = 1; i <= M; ++i) {
        alive_players.insert(i);
        cin >> players[i].x >> players[i].y;
        other_grid[players[i].x][players[i].y] = i;
    }

    cin >> escape.x >> escape.y;
    other_grid[escape.x][escape.y] = -1;
}

int make_dist(Pos a, Pos b){
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool is_movable(int x, int y){
    if(x <= 0 || y <= 0 || x > N || y > N || grid[x][y] != 0) return false;
    return true;
}

void reset_cpmap(){
    memset(cp_grid, 0, sizeof(cp_grid));
    memset(cp_other, 0, sizeof(cp_grid));

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            cp_grid[i][j] = grid[i][j];
            cp_other[i][j] = other_grid[i][j];
        }
    }
}

void Player_Move(){
    unordered_set<int> tmp{};
    int ori_dist, bx, by, best_dist;
    bool flag;

    for(auto player : alive_players){ // 참조자?
        auto & p = players[player];
        ori_dist = make_dist({p.x, p.y}, {escape.x, escape.y});
        bx = p.x, by = p.y, best_dist = 100;
        flag = false;

        for(int i = 0; i < 4; i++){
            int nx = p.x + dx[i], ny = p.y + dy[i];
            if(!is_movable(nx, ny)) continue; // 해당 격자로 못 움직이면 pass
            int new_dist = make_dist({nx, ny}, {escape.x, escape.y});
            if(new_dist >= ori_dist) continue; // 현재 칸보다 가까워지지 않으면 이동 X
            if(best_dist > new_dist){ // 새로운 위치가 가장 가까워 지면 이동
                best_dist = new_dist, bx = nx, by = ny, flag = true;
            }
        }

        if(flag) dist_arr[player]++;

        other_grid[p.x][p.y] = 0;
        p.x = bx, p.y = by;

        //cout << "player " << player << " new pos: " << p.x << ' ' << p.y << endl;

        if(p.x == escape.x && p.y == escape.y){
            continue;
        }

        // 빠져 나가지 못한 플레이어만 계속 관리
        other_grid[p.x][p.y] = player;
        tmp.insert(player);
    }

    alive_players = tmp;
}

void Rotate_Maze(){
    unordered_set<int> targets{}, in_square{};
    int dist, min_dist = 100;

    // 생존자 순회 하면서 탈출구 와의 거리 가장 작은 인원들 찾기
    for(auto player : alive_players) {
        dist = max(abs(escape.x - players[player].x) + 1, abs(escape.y - players[player].y) + 1);

        if (min_dist < dist) continue;
        if (min_dist == dist) targets.insert(player);
        if (min_dist > dist) {
            targets = {};
            targets.insert(player);
            min_dist = dist;
        }
    }

    // 정사각형의 왼쪽 상단 좌표 (= 좌표 max 100개니까 그냥 순환)
    int lu_x, lu_y, rd_x, rd_y;
    bool flag;

    for(int i = 1; i <= N - min_dist + 1; i++){
        for (int j = 1; j <= N - min_dist + 1; ++j) {
            flag = false;

            lu_x = i, lu_y = j;
            rd_x = lu_x + min_dist - 1, rd_y = lu_y + min_dist - 1;

            if(escape.x < lu_x || escape.y < lu_y || escape.x > rd_x || escape.y > rd_y) continue;

            for(auto player : targets){
                int x = players[player].x, y = players[player].y;
                if(x >= lu_x && y >= lu_y && x <= rd_x && y <= rd_y){
                    flag = true;
                    break;
                }
            }

            if(flag) break;
        }
        if(flag) break;
    }



    //cout << "left upper: " << lu_x << ' ' <<lu_y << ' ' << min_dist << endl;

    // 정사각형 내부 인원 찾기
    for (auto player : targets){
        if(lu_x + min_dist > players[player].x && lu_y + min_dist > players[player].y) in_square.insert(player);
    }

    reset_cpmap();

    // 좌표 1, 1으로 통일
    for(int i = 1; i <= min_dist; i++){
        for (int j = 1; j <= min_dist; ++j) {
            cp_grid[i][j] = grid[lu_x + i - 1][lu_y + j - 1];
            cp_other[i][j] = other_grid[lu_x + i - 1][lu_y + j - 1];
        }
    }

    // 회전
    for(int i = 1; i <= min_dist; i++){
        for (int j = 1; j <= min_dist; ++j) {
            to_grid[i][j] = cp_grid[min_dist - j + 1][i];
            to_other[i][j] = cp_other[min_dist - j + 1][i];
        }
    }


    // grid 반영
    for(int i = 1; i <= min_dist; i++){
        for (int j = 1; j <= min_dist; ++j) {
            grid[lu_x + i - 1][lu_y + j - 1] = to_grid[i][j];
            if (grid[lu_x + i - 1][lu_y + j - 1] > 0) grid[lu_x + i - 1][lu_y + j - 1]--;

            other_grid[lu_x + i - 1][lu_y + j - 1] = to_other[i][j];
            if (other_grid[lu_x + i - 1][lu_y + j - 1] > 0) {
                players[to_other[i][j]].x = lu_x + i - 1;
                players[to_other[i][j]].y = lu_y + j - 1;
            }

            // 탈출구 반영
            if (other_grid[lu_x + i - 1][lu_y + j - 1] == -1) {
                escape.x = lu_x + i - 1, escape.y = lu_y + j - 1;
            }
        }
    }
}

void run(){
   // traverse_grid();
    //traverse_player();

    while(K-- && !alive_players.empty()){ // K초가 지나지 않고 참가자가 남아 있을 동안 진행
        Player_Move();
        Rotate_Maze();
        //traverse_grid();
        //traverse_player();
    }

    // 이동 거리 합 구하기
    for (int i = 0; i <= M; ++i) {
        rst += dist_arr[i];
    }

    // 이동 거리 합, 출구 좌표 출력
    cout << rst << endl << escape.x << ' ' << escape.y;
}

int main() {
    init();
    input();
    run();
    return 0;
}