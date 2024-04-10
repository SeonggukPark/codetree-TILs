#include <iostream>
#include <cstring>
#include <unordered_map>
#include <queue>
#include <climits>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
#define pii pair<int, int>
#define ll long long
using namespace std;
// --------------------------
int Q, N, M, P, K, S, L, um_idx;
ll total_score;
constexpr size_t MAX_P = 2001; // 최대 토끼 2000 마리
int dr[4] = {0, 0, 1, -1};
int dc[4] = {1, -1, 0, 0};
ll minus_score[MAX_P];

// --------------------------
unordered_map<int, int> um; // pid mapping 목적

struct Rabbit{
    int r, c, pid, jump_cnt, dist, idx;
};

struct cmp_rabbit{
    bool operator()(const Rabbit & a, const Rabbit & b){
        if(a.jump_cnt != b.jump_cnt) return a.jump_cnt > b.jump_cnt;
        if(a.r + a.c != b.r + b.c) return a.r + a.c > b.r + b.c;
        if(a.r != b.r) return a.r > b.r;
        if(a.c != b.c) return a.c > b.c;
        return a.pid > b.pid;
    }
};

struct cmp_race{
    bool operator()(const Rabbit & a, const Rabbit & b){
        if(a.r + a.c != b.r + b.c) return a.r + a.c < b.r + b.c;
        if(a.r != b.r) return a.r < b.r;
        if(a.c != b.c) return a.c < b.c;
        return a.pid < b.pid;
    }
};

struct Pos{
    int r, c;
};

struct cmp_pos{
    bool operator()(const pii & a, const pii & b){
        if (a.first + a.second != b.first + b.second) return a.first + a.second < b.first + b.second;
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    }
};

Rabbit rabbit[MAX_P];

void Prepare(){
    // initialize
    memset(minus_score, 0, sizeof(minus_score));
    um_idx = 1;
    total_score = 0;
    um.clear();

    int pid, d;
    cin >> N >> M >> P; // N * M grid,

    for(int i = 0; i < P; i++){
        cin >> pid >> d;
        rabbit[um_idx].r = 1;
        rabbit[um_idx].c = 1;
        rabbit[um_idx].jump_cnt = 0;
        rabbit[um_idx].pid = pid;
        rabbit[um_idx].dist = d;
        rabbit[um_idx].idx = um_idx;

        um[pid] = um_idx++;
    }
}

Rabbit U_Rabbit(Rabbit curRabbit, int dis) {
    Rabbit upRabbit = curRabbit;
    dis %= 2 * (N - 1);

    if (dis >= upRabbit.r - 1) {
        dis -= (upRabbit.r - 1);
        upRabbit.r = 1;
    } else {
        upRabbit.r -= dis;
        dis = 0;
    }

    if (dis >= N - upRabbit.r) {
        dis -= (N - upRabbit.r);
        upRabbit.r = N;
    } else {
        upRabbit.r += dis;
        dis = 0;
    }

    upRabbit.r -= dis;

    return upRabbit;
}

Rabbit D_Rabbit(Rabbit curRabbit, int dis) {
    Rabbit downRabbit = curRabbit;
    dis %= 2 * (N - 1);

    if (dis >= N - downRabbit.r) {
        dis -= (N - downRabbit.r);
        downRabbit.r = N;
    } else {
        downRabbit.r += dis;
        dis = 0;
    }

    if (dis >= downRabbit.r - 1) {
        dis -= (downRabbit.r - 1);
        downRabbit.r = 1;
    } else {
        downRabbit.r -= dis;
        dis = 0;
    }

    downRabbit.r += dis;
    return downRabbit;
}

Rabbit R_Rabbit(Rabbit curRabbit, int dis) {
    Rabbit rightRabbit = curRabbit;
    dis %= 2 * (M - 1);

    if (dis >= rightRabbit.c - 1) {
        dis -= (rightRabbit.c - 1);
        rightRabbit.c = 1;
    } else {
        rightRabbit.c -= dis;
        dis = 0;
    }

    if (dis >= M - rightRabbit.c) {
        dis -= (M - rightRabbit.c);
        rightRabbit.c = M;
    } else {
        rightRabbit.c += dis;
        dis = 0;
    }

    rightRabbit.c -= dis;

    return rightRabbit;
}

Rabbit L_Rabbit(Rabbit curRabbit, int dis) {
    Rabbit leftRabbit = curRabbit;
    dis %= 2 * (M - 1);

    if (dis >= M - leftRabbit.c) {
        dis -= (M - leftRabbit.c);
        leftRabbit.c = M;
    } else {
        leftRabbit.c += dis;
        dis = 0;
    }

    if (dis >= leftRabbit.c - 1) {
        dis -= (leftRabbit.c - 1);
        leftRabbit.c = 1;
    } else {
        leftRabbit.c -= dis;
        dis = 0;
    }

    leftRabbit.c += dis;
    return leftRabbit;
}

bool round_jump[MAX_P];

void Race(){
    cin >> K >> S; // K번 진행 후 우선순위 높은 토끼 점수 +S
    priority_queue<Rabbit, vector<Rabbit>, cmp_rabbit> pq_rabbit;
    priority_queue<pii, vector<pii>, cmp_pos> pq_pos;
    priority_queue<Rabbit, vector<Rabbit>, cmp_race> pq_race;

    for(int i = 1; i <= P; ++i) {
        round_jump[i] = false;
        pq_rabbit.push(rabbit[i]);
    }

    while(K--){
        auto target = pq_rabbit.top();
        pq_rabbit.pop();

        rabbit[target.idx].jump_cnt++;
        round_jump[target.idx] = true;

        pq_pos = {};
        int dist = target.dist;

        auto move = U_Rabbit(rabbit[target.idx], dist);
        // cout << move.r << ' ' << move.c << endl;
        pq_pos.push({move.r, move.c});

        move = D_Rabbit(rabbit[target.idx], dist);
        // cout << move.r << ' ' << move.c << endl;
        pq_pos.push({move.r, move.c});

        move = R_Rabbit(rabbit[target.idx], dist);
        // cout << move.r << ' ' << move.c << endl;
        pq_pos.push({move.r, move.c});

        move = L_Rabbit(rabbit[target.idx], dist);
       //  cout << move.r << ' ' << move.c << endl;
        pq_pos.push({move.r, move.c});

        pii best = pq_pos.top();
        Pos new_pos = {best.first, best.second};
        // cout << "rabbit: " << target.idx << ' ' << new_pos.r << ' ' <<new_pos.c << endl;

        rabbit[target.idx].r = new_pos.r, rabbit[target.idx].c = new_pos.c;

        // i 번째 토끼 점수: total_score - minus_score[i]
        total_score += new_pos.r + new_pos.c;
        minus_score[target.idx] += new_pos.r + new_pos.c;

        pq_rabbit.push(rabbit[target.idx]);
    }

    for(int i = 1; i <= P; ++i) {
        // K번 동안 한 번이라도 뽑힌 토끼만 pq에 추가
        if(round_jump[i] != 0) pq_race.push(rabbit[i]);
    }

    auto target = pq_race.top();
    // cout << "after race: " << target.idx << endl;
    minus_score[target.idx] -= S;
}

void Change_Dist(){
    int pid, l;
    cin >> pid >> l;

    int idx = um[pid];
    rabbit[idx].dist *= l;
}

void Select_Winner(){
    ll score, rst = LONG_LONG_MIN;
    for(int i = 1; i <= P; ++i) {
        score = total_score - minus_score[i];
        // cout << "rabbit: " << i << ' ' << score << endl;
        rst = max(rst, score);
    }

    cout << rst;
}


void run(){
    int cmd;
    cin >> Q;

    while(Q--){
        cin >> cmd;
        switch (cmd) {
            case 100:
                Prepare();
                break;

            case 200:
                Race();
                break;

            case 300:
                Change_Dist();
                break;

            case 400:
                Select_Winner();
                break;
        }
    }
}



int main() {
    fastio
    // freopen("input.txt", "r", stdin);
    run();

    return 0;
}