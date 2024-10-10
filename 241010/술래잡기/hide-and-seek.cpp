#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
using namespace std;

#define pii pair<int, int>
#define x first
#define y second
#define endl '\n'
constexpr size_t MAX_N = 100;
int n, m, h, k, cur_turn, dir_boss, prev_boss, cnt_boss, score_boss, grid[MAX_N][MAX_N];
int dx[4] = { -1, 0, 1, 0 }; // 상, 우, 하, 좌
int dy[4] = { 0, 1, 0, -1 };

bool rev_boss, rpt_boss, trees[MAX_N][MAX_N];
pii boss;

struct TARGET {
    int r, c, idx, dir;
};

set<int> alive_targets, killed;
vector<TARGET> targets;

bool is_valid(int r, int c) {
    if (r <= 0 || c <= 0 || r > n || c > n) return false;
    return true;
}

int make_distance(pii a, pii b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}


void input() {
    for (int i = 0; i <= n; i++) {
        fill(trees[i], trees[i] + n + 1, false);
        fill(grid[i], grid[i] + n + 1, -1);
    }

    cin >> n >> m >> h >> k;
    int r, c, d;

    for (int i = 0; i < m; i++) {
        cin >> r >> c >> d;
        targets.push_back({ r, c, i, d });
        alive_targets.insert(i);
    }

    for (int i = 0; i < h; i++) {
        cin >> r >> c;
        trees[r][c] = true;
    }

    boss = { (n / 2) + 1, (n / 2) + 1 };
    rpt_boss = false, prev_boss = 1, cnt_boss = 1, dir_boss = 0, rev_boss = false, score_boss = 0;
}

void move_targets() {
    for (int cur : alive_targets) {
        TARGET & tar = targets[cur];
        if (make_distance({ tar.r, tar.c }, boss) > 3) continue; // 거리 3 이하일 때만 이동

        int nx = tar.r + dx[tar.dir], ny = tar.c + dy[tar.dir];

        if (!is_valid(nx, ny)) {
            tar.dir = (tar.dir + 2) % 4; // 방향 전환
            nx = tar.r + dx[tar.dir], ny = tar.c + dy[tar.dir];
        }
         
        if (nx == boss.x && ny == boss.y) continue; // 앞 칸에 술래 있을 경우 이동 X
        tar.r = nx, tar.c = ny;
    }

    /*
    for (int cur : alive_targets) {
        TARGET& tar = targets[cur];
        cout << tar.idx << ' ' << tar.r << ' ' << tar.c << ' ' << tar.dir << endl;
    }
    */
}

void move_boss() {
    boss.x += dx[dir_boss], boss.y += dy[dir_boss], cnt_boss--; // boss 이동

    if (!rev_boss) { // 순방향 진행
        if (cnt_boss == 0) { // boss 방향 전환 판단 후 변경
            if (rpt_boss) prev_boss++;
            cnt_boss = prev_boss, rpt_boss = !rpt_boss;
            dir_boss = (dir_boss + 1) % 4;
        }
    }

    else { // 역방향 진행
        if (cnt_boss == 0) { // boss 방향 전환 판단 후 변경
            if (rpt_boss) prev_boss--;
            cnt_boss = prev_boss, rpt_boss = !rpt_boss;
            dir_boss = (dir_boss + 3) % 4;
        }
    }

    if (boss == make_pair(1, 1)) { // 역방향 전환
        rpt_boss = false, prev_boss = n - 1, cnt_boss = n - 1, dir_boss = 2, rev_boss = true;
    }

    if (boss == make_pair((n / 2) + 1, (n / 2) + 1)) { // 순방향 전환
        rpt_boss = false, prev_boss = 1, cnt_boss = 1, dir_boss = 0, rev_boss = false;
    }
}

void arrest_targets() {
    int cnt = 0;
    killed = {};

    for (int i = 0; i < 3; ++i) {
        int nx = boss.x + (i * dx[dir_boss]), ny = boss.y + (i * dy[dir_boss]);
        if (!is_valid(nx, ny) || trees[nx][ny]) continue;

        for (int idx : alive_targets) {
            auto tar = targets[idx];
            if (tar.r == nx && tar.c == ny) {
                cnt++;
                killed.insert(idx);
            }
        }
    }

    score_boss += cur_turn * cnt;

    if (!killed.empty()) {
        for (int kill : killed) {
            alive_targets.erase(kill);
        }
    }
}

void solve() {
    cur_turn = 0;

    while (k--) {
        cur_turn++;
        move_targets(); // 도망자 이동
        move_boss(); // 술래 이동
        arrest_targets(); // 도망자 체포
    }

    cout << score_boss;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    input();
    solve();
    return 0;
}