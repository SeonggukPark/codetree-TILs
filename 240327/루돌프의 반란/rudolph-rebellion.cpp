#include <bits/stdc++.h>
#define fastio ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
#define endl '\n'
#define pipii pair<int, pair<int, int>>
#define pii pair<int, int>

using namespace std;
constexpr size_t MAX_SANTA = 31;
constexpr size_t MAX_N = 51;
int n, m; // 게임판 크기, 턴 수
int p, c, d; // 산타 수, 루돌프 힘, 산타 힘
int rudolf_r, rudolf_c; // 루돌프 좌표
int santa_r[MAX_SANTA + 1], santa_c[MAX_SANTA + 1];
int score[MAX_SANTA];
int stunned[MAX_SANTA];
int grid[MAX_N][MAX_N];
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};
int cur_turn;


struct cmp{
    bool operator()(const pipii & a, const pipii & b){
        if(a.first != b.first) return a.first > b.first;
        if(a.second.first != b.second.first) return a.second.first < b.second.first;
        return a.second.second < b.second.second;
    }
};

struct cmp2{
    bool operator()(const pii & a, const pii & b){
        if(a.first != b.first) return a.first > b.first;
        return a.second > b.second;
    }
};


priority_queue<pipii, vector< pipii >, cmp> dist;
unordered_set<int> alive_santa;

void init(){
    memset(santa_r, 0, sizeof(santa_r));
    memset(santa_c, 0, sizeof(santa_c));
    memset(grid, 0, sizeof(grid));
    memset(score, 0, sizeof(score));
    memset(stunned, 0 ,sizeof(stunned));
    alive_santa.clear();
    cur_turn = 1;
}

void input(){
    int pn;
    cin >> n >> m >> p >> c >> d;
    cin >> rudolf_r >> rudolf_c;
    grid[rudolf_r][rudolf_c] = 31;

    for (int i = 0; i < p; ++i) {
        cin >> pn;
        cin >> santa_r[pn] >> santa_c[pn];
        grid[santa_r[pn]][santa_c[pn]] = pn;
        // cout << "pn info: " << pn << ' ' << santa_r[pn] << ' ' << santa_c[pn] << endl;
        alive_santa.insert(pn);
    }

    /*
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
     */
}


void traverse(){
    cout << "grid state: " << endl;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cout << grid[i][j] << ' ';
        }
        cout << endl;
    }

    cout << endl;

    for(auto i : alive_santa){
        cout << i << ' ';
    }

    cout << endl;
}


void rudolf_turn(){
    // cout << "cur rudolf: " << rudolf_r << ' ' <<rudolf_c << endl;
    dist = {};
    for(int i = 1; i <= p; i++){
        if(alive_santa.find(i) == alive_santa.end()) continue;
        dist.push({pow(rudolf_c - santa_c[i], 2)  + pow(rudolf_r - santa_r[i], 2), {santa_r[i], santa_c[i]}});
    }

    auto hit_rudolf = dist.top();
    // cout << "hit_rudolf: " << hit_rudolf.first << ' ' << hit_rudolf.second.first << ' ' << hit_rudolf.second.second << endl;

    int r_l, u_d;

    if(hit_rudolf.second.first > rudolf_r) u_d = 1;
    else if(hit_rudolf.second.first < rudolf_r) u_d = -1;
    else u_d = 0;

    if(hit_rudolf.second.second > rudolf_c) r_l = 1;
    else if(hit_rudolf.second.second < rudolf_c) r_l = -1;
    else r_l = 0;

    // cout << "ud, rl: " << u_d << ' ' << r_l << endl;

    grid[rudolf_r][rudolf_c] = 0;
    rudolf_r += u_d, rudolf_c += r_l;
    int row = rudolf_r, col = rudolf_c, prev = 31;

    // 충돌 계산 & 루돌프 이동
    // 해당 칸에 산타 존재할 경우
    bool flag = false;

    if(grid[row][col] != 0) stunned[grid[row][col]] = cur_turn + 1;

    while(grid[row][col] != 0){
        int tar_santa = grid[row][col];
        grid[row][col] = prev;
        score[tar_santa] += c;

        // c ~ 루돌프 힘만큼
        // 1. row 이동
        if(u_d != 0){
            prev == 31 ? row = rudolf_r + u_d * c : row += u_d;
        }

        // 2. col 이동
        if(r_l != 0) {
            prev == 31 ? col = rudolf_c + r_l * c : col += r_l;
        }

        // 경기장 밖으로 밀려 나면 산타 사망
        if(row < 1 || row > n || col < 1 || col > n) {
            flag = true;
            alive_santa.erase(tar_santa);

            break;
        }
        prev = tar_santa;
    }


    if(!flag){
        grid[row][col] = prev;
        santa_r[prev] = row, santa_c[prev] = col;
    }
}

void interaction(){

}

// 상우하좌
void santa_turn() {
    priority_queue<pii, vector<pii>, cmp2> pq;
    vector<int> runner;
    int prev;
    bool flag;
    for (int i = 1; i <= p; i++) {
        // 사망한 경우 pass
        if(alive_santa.find(i) == alive_santa.end()) continue;
        // 기절한 경우 pass
        if(cur_turn <= stunned[i]) continue;

        flag = false;
        pq = {};

        for(int j = 0; j < 4; j++){
            int new_row = santa_r[i] + dr[j], new_col = santa_c[i] + dc[j];

            if (new_row < 1 || new_row > n || new_col < 1 || new_col > n) {
                continue;
            }

            // 빈 칸이거나 루돌프 있는 칸일 경우 이동 후보
            if(grid[new_row][new_col] == 0 || grid[new_row][new_col] == 31) {
                pq.emplace(pow(new_row - rudolf_r, 2) + pow(new_col - rudolf_c, 2), j);
            }

        }

        int u_d = dr[pq.top().second], r_l = dc[pq.top().second];
        // cout << "ud, rl: " << u_d << ' ' << r_l << endl;

        grid[santa_r[i]][santa_c[i]] = 0;

        // 빈칸 이면 그냥 이동
        if (grid[santa_r[i] + u_d][santa_c[i] + r_l] == 0) {
            grid[santa_r[i] + u_d][santa_c[i] + r_l] = i;
            santa_r[i] += u_d, santa_c[i] += r_l;
        }

        // 루돌프랑 충돌하는 경우
        if (rudolf_r == santa_r[i] + u_d && rudolf_c == santa_c[i] + r_l) {
            stunned[i] = cur_turn + 1;
            score[i] += c;
            int row = santa_r[i] + u_d * (-d + 1), col = santa_c[i] + r_l * (-d + 1);
            prev = i;

            // 경기장 밖으로 밀려날 경우
            if (row < 1 || row > n || col < 1 || col > n) {
                alive_santa.erase(i);
                continue;
            }

            // 연쇄 추돌
            while (grid[row][col] != 0) {
                int tar_santa = grid[row][col];
                grid[row][col] = prev;
                santa_r[prev] -= u_d, santa_c[prev] -= r_l;

                row -= u_d, col -= r_l;

                if (row < 1 || row > n || col < 1 || col > n) {
                    alive_santa.erase(i);
                    flag = true;
                    break;
                }

                prev = tar_santa;
            }

            if(!flag){
                grid[row][col] = prev;
                santa_r[prev] -= u_d, santa_c[prev] -= r_l;
            }
        }

        // traverse();
    }
}

void turn_scoring(){
    for(auto i : alive_santa){
        score[i]++;
    }

    /*
    for(int i = 1; i <= p; i++){
        cout << i << ": " << score[i] << endl;
    }
*/

    cur_turn++;
}


void run(){
    // 움직임: 루돌프 -> 산타 (1 ~ P번 순)
    // 기절, 탈락 (= 격자 밖으로 이동)한 산타 이동 X

    // 게임 턴 수, 산타 살아 있는 동안 계속
    while(cur_turn <= m && !alive_santa.empty()){
        // cout << endl << "cur_turn: " << cur_turn << endl;

        rudolf_turn();
        // traverse();

        santa_turn();
        // traverse();

        turn_scoring();
    }
}

void final_score(){
    for(int i = 1; i <= p; i++){
        cout << score[i] << ' ';
    }
}

int main(){
    fastio
    init();
    input();

    run();
    final_score();

    return 0;
}