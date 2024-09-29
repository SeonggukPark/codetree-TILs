#include <iostream>
#include <vector>
#include <set>
#define x first
#define y second
#define pii pair<int, int>
using namespace std;
constexpr int INF = 987654321;
constexpr size_t MAX_N = 50, MAX_SANTA = 30;
int N, M, P, C, D, cur_turn;
int grid[MAX_N + 1][MAX_N + 1], santa_score[MAX_SANTA], santa_stun[MAX_SANTA];
int dx[8] = {-1, 0, 1, 0, 1, 1, -1, -1}; // 상 우 하 좌
int dy[8] = {0, 1, 0, -1, 1, -1, 1, -1};
pii ru_pos, santa[MAX_SANTA + 1];
set<int> alive_santa, delete_santa;

int make_dist(const pii & a, const pii & b){
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

bool is_valid(int r, int c){
    if(r <= 0 || c <= 0 || r > N || c > N) return false;
    return true;
}

void collapse(bool hit_by_rudolf_move, int dir, int santa_idx){
    int tar;
    if(hit_by_rudolf_move){ // 루돌프가 친 경우
        tar = grid[ru_pos.x][ru_pos.y];
        santa_score[tar] += C;
        santa[tar] = {santa[tar].x + C * dx[dir], santa[tar].y + C * dy[dir]};

        if(!is_valid(santa[tar].x, santa[tar].y)){ // 사망 판정
            delete_santa.insert(tar);
            return;
        }
        santa_stun[tar] = cur_turn + 1; // 기절
    }

    else{ // 산타가 친 경우
        tar = santa_idx;
        santa_score[tar] += D;
        santa[tar] = {santa[tar].x + D * dx[dir], santa[tar].y + D * dy[dir]};

        if(!is_valid(santa[tar].x, santa[tar].y)){ // 사망 판정
            delete_santa.insert(tar);
            return;
        }
        santa_stun[tar] = cur_turn + 1; // 기절
    }

    // 상호작용
    int prev, next = tar;
    while(is_valid(santa[next].x, santa[next].y) && grid[santa[next].x][santa[next].y] != 0){
        prev = next, next = grid[santa[next].x][santa[next].y];
        grid[santa[prev].x][santa[prev].y] = prev;

        santa[next] = {santa[next].x + dx[dir], santa[next].y + dy[dir]};
        if(!is_valid(santa[next].x, santa[next].y)) {
            delete_santa.insert(next);
            return;
        }
    }

    grid[santa[next].x][santa[next].y] = next;
}

void rudolf_move(){
    pii tar = {-1, INF};
    for(int idx : alive_santa){
        int dist = make_dist(ru_pos, santa[idx]);
        if(dist < tar.y) tar = {idx, dist};
        else if(dist == tar.y){
            if(santa[tar.x].x < santa[idx].x
            || (santa[tar.x].x == santa[idx].x && santa[tar.x].y < santa[idx].y)) {
                tar = {idx, dist};
            }
        }
    }

    // 가까워 지는 방향 루돌프 이동
    pii min_dir = {-1, INF};
    for (int i = 0; i < 8; ++i) {
        int nx = ru_pos.x + dx[i], ny = ru_pos.y + dy[i];
        if(!is_valid(nx, ny)) continue;
        int dist = make_dist({nx, ny}, {santa[tar.x].x, santa[tar.x].y});
        if(dist < min_dir.y) min_dir = {i, dist};
    }

    grid[ru_pos.x][ru_pos.y] = 0;
    ru_pos = {ru_pos.x + dx[min_dir.x], ru_pos.y + dy[min_dir.x]};

    // 이동 자리에 산타 있을 경우
    if(grid[ru_pos.x][ru_pos.y] != 0){
        // 충돌
        collapse(true, min_dir.x, -1);
    }

    grid[ru_pos.x][ru_pos.y] = -1;
}

void santa_move() {
    if(alive_santa.empty()) return;

    for (int idx: alive_santa) {
        if(delete_santa.find(idx) != delete_santa.end() || santa_stun[idx] >= cur_turn) continue;

        // 가까워 지는 방향 산타 이동
        pii min_dir = {-1, make_dist(ru_pos, santa[idx])};
        for (int i = 0; i < 4; ++i) {
            int nx = santa[idx].x + dx[i], ny = santa[idx].y + dy[i];
            if(!is_valid(nx, ny) || grid[nx][ny] > 0) continue;
            int dist = make_dist({nx, ny}, ru_pos);
            if(dist < min_dir.y) min_dir = {i, dist};
        }

        // 움직일 수 있는 칸이 없거나 루돌프로부터 가까워질 수 있는 방법이 없다면 산타는 움직이지 않습니다.
        if(min_dir.x == -1) continue;

        grid[santa[idx].x][santa[idx].y] = 0;
        santa[idx] = {santa[idx].x + dx[min_dir.x], santa[idx].y + dy[min_dir.x]};

        // 루돌프 존재 한다면
        if(grid[santa[idx].x][santa[idx].y] != 0) collapse(false, (min_dir.x + 2) % 4, idx);
        else grid[santa[idx].x][santa[idx].y] = idx;
    }
}

void turn_score(){
    for(int idx : delete_santa){
        alive_santa.erase(idx);
    }

    if(alive_santa.empty()) return;

    for(int idx : alive_santa){
        santa_score[idx]++;
    }

    delete_santa.clear();
}

void solve(){
    cin >> N >> M >> P >> C >> D >> ru_pos.x >> ru_pos.y;
    grid[ru_pos.x][ru_pos.y] = -1;

    int idx;
    for (int i = 0; i < P; ++i) {
        cin >> idx;
        alive_santa.insert(idx);
        cin >> santa[idx].x >> santa[idx].y;
        grid[santa[idx].x][santa[idx].y] = idx;
    }

    cur_turn = 0;
    while(M-- && !alive_santa.empty()){
        cur_turn++;
        rudolf_move();
        santa_move();
        turn_score();
    }

    for(int i = 1; i <= P; ++i){
        cout << santa_score[i] << ' ';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}