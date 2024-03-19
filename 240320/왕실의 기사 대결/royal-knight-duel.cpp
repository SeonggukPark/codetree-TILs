// 왕실의 기사 대결
#include <bits/stdc++.h>
using namespace std;
constexpr size_t MAX_L = 40;
constexpr size_t MAX_KNIGHT = 40;
int L, N, Q;
int r, c, h, w, k;
int a, d;

typedef struct Knight{
    int index;
    pair<int, int> pos;
    pair<int, int> vol;
    int life;
    bool alive;
} knight;

int chess[MAX_L][MAX_L];
int chess_knight[MAX_L][MAX_L];
pair<int, int> dx_dy[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // 상, 우, 하, 좌
knight knights[MAX_KNIGHT + 1];
int damage[MAX_KNIGHT + 1];
unordered_set<int> S;
unordered_set<int> attacked;

void init(){
    memset(chess, 0, sizeof(chess));
    memset(chess_knight, 0 ,sizeof(chess_knight));
    memset(damage, 0 ,sizeof(damage));
    S.clear();

    for (int i = 1; i < MAX_KNIGHT + 1; ++i) {
        knights[i].index = -1;
        knights[i].pos = {-1, -1};
        knights[i].vol = {-1, -1};
        knights[i].life = -1;
        knights[i].alive = false;
    }
}

bool move_possible(int idx, int dir){
    int s, e;
    attacked.clear();
    knight cur;
    queue<int> q;
    q.push(idx);
    attacked.insert(idx);

    switch(dir) {
        case 0: // 상
            while(!q.empty()) {
                cur = knights[q.front()];
                q.pop();

                // 맨 위일 경우
                if (cur.pos.first == 0) return false;
                s = cur.pos.second, e = s + cur.vol.second;

                for (int i = s; i < e; i++) {
                    // 벽 있으면 이동 X
                    if (chess[cur.pos.first - 1][i] == 2) return false;

                    // 맞 닿는 곳에 기사 있으면 탐색 대상으로 추가
                    if (chess_knight[cur.pos.first - 1][i] != 0) {
                        q.push(knights[chess_knight[cur.pos.first - 1][i]].index);
                        attacked.insert(knights[chess_knight[cur.pos.first - 1][i]].index);
                    }
                }
            }
            break;

        case 1: // 우
            while(!q.empty()) {
                cur = knights[q.front()];
                q.pop();

                // 맨 오른쪽일 경우
                if (cur.pos.second + cur.vol.second == L) return false;
                s = cur.pos.first, e = s + cur.vol.first;


                for (int i = s; i < e; i++) {
                    // 벽 있으면 이동 X
                    if (chess[i][cur.pos.second + cur.vol.second] == 2) return false;

                    // 맞 닿는 곳에 기사 있으면 탐색 대상으로 추가
                    if (chess_knight[i][cur.pos.second + cur.vol.second] != 0) {
                        q.push(knights[chess_knight[i][cur.pos.second + cur.vol.second]].index);
                        attacked.insert(knights[chess_knight[i][cur.pos.second + cur.vol.second]].index);
                    }
                }
            }
            break;

        case 2: // 하
            while(!q.empty()) {
                cur = knights[q.front()];
                q.pop();


                // 맨 아래쪽일 경우
                if (cur.pos.first + cur.vol.first == L) return false;

                s = cur.pos.second, e = s + cur.vol.second;

                for (int i = s; i < e; i++) {
                    // 벽 있으면 이동 X
                    if (chess[cur.pos.first + cur.vol.first][i] == 2) return false;

                    // 맞 닿는 곳에 기사 있으면 탐색 대상으로 추가
                    if (chess_knight[cur.pos.first + cur.vol.first][i] != 0) {
                        q.push(knights[chess_knight[cur.pos.first + cur.vol.first][i]].index);
                        attacked.insert(knights[chess_knight[cur.pos.first + cur.vol.first][i]].index);
                    }
                }
            }
            break;

        case 3: // 좌
            while(!q.empty()) {
                cur = knights[q.front()];
                q.pop();

                // 맨 왼쪽일 경우
                if (cur.pos.second == 0) return false;
                s = cur.pos.first, e = s + cur.vol.first;

                for (int i = s; i < e; i++) {
                    // 벽 있으면 이동 X
                    if (chess[i][cur.pos.second - 1] == 2) return false;

                    // 맞 닿는 곳에 기사 있으면 탐색 대상으로 추가
                    if (chess_knight[i][cur.pos.second - 1] != 0) {
                        q.push(knights[chess_knight[i][cur.pos.second - 1]].index);
                        attacked.insert(knights[chess_knight[i][cur.pos.second - 1]].index);
                    }
                }
            }
            break;
    }

    return true;
}

void fight_knight(int idx){
    // 이동 완료 후
    for(auto iter : attacked){
        // 공격자는 제외
        if(iter == idx) continue;

        auto start = knights[iter].pos;
        int down = knights[iter].vol.first;
        int right = knights[iter].vol.second;
        int cnt = 0;

        for(int i = 0; i < down; ++i){
            for (int j = 0; j < right; ++j) {
                if(chess[start.first + i][start.second + j] == 1) cnt++;
            }
        }

        damage[iter] += cnt;

        if(damage[iter] >= knights[iter].life){
            knights[iter].alive = false;
            S.erase(S.find(iter));
        }
    }
}

void move_knight(int idx, int dir){
    // 체스판에 없는 기사일 경우 반응 X
    if(!knights[idx].alive) return;

    // 못 움직일 때도 반응 X
    if(!move_possible(idx, dir)) return;


    // 움직일 수 있다면 해당 기사들 다 옮겨주기기
    for(auto iter : attacked){
        knights[iter].pos.first += dx_dy[dir].first;
        knights[iter].pos.second += dx_dy[dir].second;
    }

    fight_knight(idx);

    // 지도 reset
    memset(chess_knight, 0, sizeof(chess_knight));


    // 지도 갱신
    for(auto iter : S){
        int u = knights[iter].pos.first, l = knights[iter].pos.second;
        int down = knights[iter].vol.first, right = knights[iter].vol.second;
        int ind = iter;

        for(int i = u; i < u + down; i++){
            for (int j = l; j < l + right; ++j) {
                chess_knight[i][j] = ind;
            }
        }
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int rst = 0;

    init();

    // L: 체스판 크기 (3 ~ 40), N: 기사 수 (1 ~ 30), Q: 명령수(1 ~ 100), k: 기사 체력 (1 ~ 100)
    cin >> L >> N >> Q;

    // 체스판 정보
    for(int i = 0; i < L; ++i){
        for (int j = 0; j < L; ++j) {
            cin >> chess[i][j];
        }
    }

    // 기사 정보 (r, c, h, w, k) -> (r, c) 좌픅 꼭지점, 세로 h, 가로 w, 초기 체력 K
    for (int i = 1; i <= N; ++i) {
        cin >> r >> c >> h >> w >> k;
        knights[i].pos = {r - 1, c - 1};
        knights[i].vol = {h, w};
        knights[i].index = i;
        knights[i].life = k;
        knights[i].alive = true;
        S.insert(i);

        for(int j = r - 1; j < r + h - 1; j++){
            for (int l = c - 1; l < c + w - 1; ++l) {
                chess_knight[j][l] = i;
            }
        }

    }


    // 왕의 명령 (i, d) -> i번 기사에게 방향 d로 한 칸 이동
    for (int i = 0; i < Q; ++i) {
        cin >> a >> d;
        move_knight(a, d);
    }

    for (auto i : S) {
       rst += damage[i];
    }
    
    cout << rst;
}