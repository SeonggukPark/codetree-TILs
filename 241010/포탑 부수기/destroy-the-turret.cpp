#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;
#define pii pair<int, int>
#define x first
#define y second
#define endl '\n'
constexpr size_t MAX_N = 11;
int damage[MAX_N][MAX_N], atk_time[MAX_N][MAX_N], dam_time[MAX_N][MAX_N];
int N, M, K, cur_time;
int dr[8] = { 0, 1, 0, -1, 1, 1, -1, -1}; // 우, 하, 좌, 상 + 4 대각선
int dc[8] = { 1, 0, -1, 0, 1, -1, 1, -1};
pii wk, st;

void input() {
    cin >> N >> M >> K;
    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= M; ++c) {
            cin >> damage[r][c];
        }
    }
}

struct Node {
    pii pos;
    int atk_time, dam;
};

struct cmp_wk {
    bool operator()(const Node& a, const Node& b) {
        if (a.dam != b.dam) return a.dam > b.dam;
        if (a.atk_time != b.atk_time) return a.atk_time < b.atk_time;
        if (a.pos.x + a.pos.y != b.pos.x + b.pos.y) return a.pos.x + a.pos.y < b.pos.x + b.pos.y;
        return a.pos.y < b.pos.y;
    }
};

struct cmp_st {
    bool operator()(const Node& a, const Node& b) {
        if (a.dam != b.dam) return a.dam < b.dam;
        if (a.atk_time != b.atk_time) return a.atk_time > b.atk_time;
        if (a.pos.x + a.pos.y != b.pos.x + b.pos.y) return a.pos.x + a.pos.y > b.pos.x + b.pos.y;
        return a.pos.y > b.pos.y;
    }
};

priority_queue<Node, vector<Node>, cmp_wk> pq_wk; // 공격자 선정 용
priority_queue<Node, vector<Node>, cmp_st> pq_st; // 방어자 선정 용

void select_targets() {
    pq_wk = {}, pq_st = {};

    // grid 순환 하면서 pq 2개에 넣기
    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= M; ++c) {
            if (damage[r][c] <= 0) continue; // 부서진 타워 무시
            pq_wk.push({ {r, c}, atk_time[r][c], damage[r][c] });
            pq_st.push({ {r, c}, atk_time[r][c], damage[r][c] });
        }
    }

    // 1. 공격자 선정
    wk = pq_wk.top().pos;
    damage[wk.x][wk.y] += (N + M); // 핸디캡 적용
    atk_time[wk.x][wk.y] = cur_time;

    // 2. 피해자 선정
    while (!pq_st.empty() && pq_st.top().pos == wk) pq_st.pop(); // 공격자랑 동일해지는 것 방지
    st = pq_st.top().pos;
}

queue<pii> q;
void attack() {
    // 1. 레이저 공격 시도
    pii track[MAX_N][MAX_N]; // 경로 추적용
    for (int i = 1; i <= N; i++){
        fill(track[i], track[i] + M + 1, make_pair(-1, -1));
    }

    q = {};
    q.push(wk);
    int cur_dam = damage[wk.x][wk.y];

    while (!q.empty()) {
        pii top = q.front(); q.pop();
        if (top == st) {
            
            damage[st.x][st.y] -= cur_dam; // 대상 공격

            pii prev = track[st.x][st.y];
            while (prev != wk) { // 경로 공격
                // cout << "prev: " << prev.x << ' ' << prev.y << endl;
                damage[prev.x][prev.y] -= (cur_dam / 2);
                dam_time[prev.x][prev.y] = cur_time;
                prev = track[prev.x][prev.y];
            }

            return;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = top.x + dr[i], nc = top.y + dc[i];
            if (nr == 0) nr = N;
            if (nr == N + 1) nr = 1;
            if (nc == 0) nc = M;
            if (nc == M + 1) nc = 1;

            if (damage[nr][nc] == 0) continue; // 부서진 포탑인 경우
            if (track[nr][nc] != make_pair(-1, -1)) continue; // 이미 방문한 경우

            track[nr][nc] = top;
            q.push({ nr, nc });
        }
    }

    // 2. 포탄 공격
    // 2. 포탄 공격
    damage[st.x][st.y] -= cur_dam;

    for (int i = 0; i < 8; ++i) { // 주변 공격
        int nr = st.x + dr[i], nc = st.y + dc[i];
        if (nr == 0) nr = N;
        if (nr == N + 1) nr = 1;
        if (nc == 0) nc = M;
        if (nc == M + 1) nc = 1;

        if (damage[nr][nc] <= 0) continue; // 이미 부서진 경우

        dam_time[nr][nc] = cur_time;
        damage[nr][nc] -= (cur_dam / 2);
    }
}

void repair() {
    // grid 순환 하면서 대상 포탑 정비
    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= M; ++c) {
            if (damage[r][c] <= 0) continue; // 부서진 타워 무시

            pii cur = { r, c };
            if (st == cur || wk == cur) continue; // 공격, 방어자 제외
            if (dam_time[r][c] == cur_time) continue; // 주변부 피해자 제외

            damage[r][c]++;
        }
    }
}

int check_alive() {
    int cnt = 0;
    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= M; ++c) {
            if (damage[r][c] > 0) cnt++; 
        }
    }
    return cnt;
}

void print_result() {
    int max_dam = 0;
    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= M; ++c) {
            max_dam = max(max_dam, damage[r][c]);
        }
    }
    cout << max_dam;
}

void traverse() {
    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= M; ++c) {
            cout << damage[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void solve() {
    cur_time = 0;
    
    while (K--) {
        cur_time++;
        select_targets(); // 공격자, 피해자 선정
        attack(); // 공격
        repair(); // 정비
        if (check_alive() <= 1) break; // 부서지지 않은 포탑 1개 이하면 즉시 종료
        // traverse();
    }
    print_result(); // 살아 있는 포탑 중 가장 강한 포탑 공격력 출력
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    input();
    solve();
    return 0;
}