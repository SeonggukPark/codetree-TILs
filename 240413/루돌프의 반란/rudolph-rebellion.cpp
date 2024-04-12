#include <iostream>
#include <cstring>
#include <queue>
#include <unordered_set>

using namespace std;
constexpr size_t MAX_N = 51, MAX_P = 31;
int n, m, p, c, d, cur_turn;

struct Pos{
    int x, y;
};
Pos rudolf;
Pos santas[MAX_P];
unordered_set<int> alive_santa;
int score_santa[MAX_P];
int stun_santa[MAX_P];

int grid[MAX_N][MAX_N];
int ddx[8] = {-1, 0, 1, 0, 1, 1, -1, -1};
int ddy[8] = {0, 1, 0, -1, 1, -1, 1, -1};
int dx[4] = {-1, 0, 1, 0}; // 상, 우, 하, 좌 순
int dy[4] = {0, 1, 0, -1};

void init(){
    alive_santa = {};
    rudolf.x = rudolf.y = 0;

    for(auto & santa : santas){
        santa.x = santa.y = 0;
    }

    memset(grid, 0, sizeof(grid));
    memset(score_santa, 0, sizeof(score_santa));
    memset(stun_santa, 0, sizeof(stun_santa));
    cur_turn = 0;
}

void input(){
    int idx;
    cin >> n >> m >> p >> c >> d;
    cin >> rudolf.x >> rudolf.y;
    grid[rudolf.x][rudolf.y] = 31; // 루돌프: 31번

    for(int i = 1; i <= p; i++){
        cin >> idx;
        alive_santa.insert(idx);
        cin >> santas[idx].x >> santas[idx].y;
        grid[santas[idx].x][santas[idx].y] = idx;
    }
}

int make_dist(Pos a, Pos b){
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

bool is_grid(int x, int y){
    if(x <= 0 || y <= 0 || x > n || y > n) return false;
    return true;
}

struct info{
    int x, y, dist;
};

struct cmp{
    bool operator()(const info & a, const info & b){
        if(a.dist != b.dist) return a.dist > b.dist;
        if(a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

void Interaction(int x, int y, int idx, int dir){ // dir 산타가 x, y를 향해 dir 방향으로 이동
    int at, df = idx;
    bool flag = false;

    // 산타가 있으면 계속 반복
    while(grid[x][y] != 0){
        // 이전의 방어자가 공격, 새로운 칸의 산타가 방어
        at = df, df = grid[x][y];
        grid[x][y] = at;
        santas[at].x = x, santas[at].y = y;

        x += ddx[dir], y += ddy[dir];

        // df가 grid 밖으로 밀리면 사망하고 종료
        if(!is_grid(x, y)){
            alive_santa.erase(df);
            flag = true;
            break;
        }
    }

    if(!flag) {
        grid[x][y] = df;
        santas[df].x = x, santas[df].y = y;
    }
}

void Collapse(int at, int df, int dir){ // at, df: 1 ~ 30 :산타, 31: 루돌프 || dir: at가 이동'했던'방향
    int santa_x, santa_y;
    int santa, new_dir;

    // 1. 루돌프가 이동한 경우
    if(at == 31) {
        score_santa[df] += c;
        santa_x = santas[df].x + c * ddx[dir], santa_y = santas[df].y + c * ddy[dir];
        santa = df;
        new_dir = dir;
    }

        // 2. 산타가 이동한 경우
    else{
        score_santa[at] += d;
        santa_x = santas[at].x - d * dx[dir], santa_y = santas[at].y - d * dy[dir];
        santa = at;
        new_dir = (dir + 2) % 4;
    }

    santas[santa].x = santa_x, santas[santa].y = santa_y;

    // grid 밖으로 밀려난 경우 사망
    if(!is_grid(santa_x, santa_y)) {
        alive_santa.erase(santa);
        return;
    }

    // 기절 처리
    stun_santa[santa] = cur_turn + 1;

    // 만약 빈 칸이면 그냥 이동
    if(grid[santa_x][santa_y] == 0) {
        grid[santa_x][santa_y] = santa;
        return;
    }

    // 해당 칸에 다른 산타 있으면 상호작용 발생
    Interaction(santa_x, santa_y, santa, new_dir);
}

void Rudolf_Move(){
    // 이동 전 위치 제거
    grid[rudolf.x][rudolf.y] = 0;

    // 가장 가까운 산타 탐색
    priority_queue<info, vector<info>, cmp> pq;
    for(auto s : alive_santa){
        int dist = make_dist({rudolf.x, rudolf.y}, {santas[s].x, santas[s].y});
        pq.push({santas[s].x, santas[s].y, dist});
    }

    auto target = pq.top();

    // 가장 가까워지는 방향 탐색
    int bx, by, bdir, new_dist, best_dist = 5001;
    for(int i = 0; i < 8; i++){
        int nx = rudolf.x + ddx[i], ny = rudolf.y + ddy[i];
        new_dist = make_dist({nx, ny}, {target.x, target.y});

        if(new_dist < best_dist){
            best_dist = new_dist;
            bx = nx, by = ny, bdir = i;
        }
    }

    // 루돌프 이동 (일단 grid는 X)
    rudolf.x = bx, rudolf.y = by;

    // 이동 장소에 산타가 있다면 충돌 처리
    if(grid[bx][by] != 0){
        // 충돌 처리
        Collapse(31, grid[bx][by], bdir);
    }

    grid[rudolf.x][rudolf.y] = 31;
}

void Santa_Move(){
    for(int i = 1; i <= p; i++){
        auto & santa = santas[i];

        // 만약 산타가 기절 상태거나 죽었다면 pass
        if(stun_santa[i] >= cur_turn || alive_santa.find(i) == alive_santa.end()){
            continue;
        }

        // 가장 가까워지는 방향 탐색
        int bx = -1, by = -1, bdir, basic_dist, new_dist, best_dist = 5001;

        // 기존보다 가까워 지는지 비교 위한 기존 거리
        basic_dist = make_dist({santa.x, santa.y}, {rudolf.x, rudolf.y});
        for(int j = 0; j < 4; j++){
            int nx = santa.x + dx[j], ny = santa.y + dy[j];
            new_dist = make_dist({nx, ny}, {rudolf.x, rudolf.y});

            // 해당 자리에 다른 산타가 있거나, 격자 밖이거나, 루돌프와 가까워지지 않으면 pass
            if((grid[nx][ny] != 0) && (grid[nx][ny] != 31) || !is_grid(nx, ny)) continue;
            if(basic_dist <= new_dist) continue;

            // 가장 가까워지는 지점 탐색
            if(new_dist < best_dist){
                best_dist = new_dist;
                bx = nx, by = ny, bdir = j;
            }
        }

        // 산타 이동 못하면 pass
        if(bx == -1 || by == -1) continue;

        // 산타 이동 (일단 grid는 X)
        grid[santa.x][santa.y] = 0;
        santa.x = bx, santa.y = by;

        // 만약 이동한 장소에 루돌프가 없다면 그냥 이동 처리
        if(grid[bx][by] != 31){
            grid[bx][by] = i;
        }

        // 충돌 처리
        else {
            Collapse(i, 31, bdir);
        }
    }
}

void Turn_Scoring(){
    for(auto a : alive_santa){
        score_santa[a]++;
    }
}

void run() {
    while (m-- && !alive_santa.empty()) {
        cur_turn++;
        Rudolf_Move();
        Santa_Move();
        Turn_Scoring();
    }

    for (int i = 1; i <= p; i++) {
        cout << score_santa[i] << ' ';
    }
}

int main() {
    // freopen("input.txt", "r", stdin);
    init();
    input();
    run();
    return 0;
}