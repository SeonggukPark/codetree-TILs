#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#define endl '\n'
#define x first
#define y second
#define pii pair<int, int>

using namespace std;
constexpr size_t MAX_L = 42, MAX_N = 31;
int L, N, Q, grid[MAX_L][MAX_L], chess[MAX_L][MAX_L], bfs[MAX_L][MAX_L];
int dx[4] = { -1, 0, 1, 0 }; // U, R, D, L
int dy[4] = { 0, 1, 0, -1 };

struct KNIGHT {
	int idx, r, c, h, w, life, dam;
};

vector<KNIGHT> knights;
queue<pii> q;

void input() {
	cin >> L >> N >> Q;
	for (int i = 0; i <= L + 1; ++i) {
		fill(chess[i], chess[i] + L + 1, 0);
		fill(grid[i], grid[i] + L + 2, 2);
	}

	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> grid[i][j];
		}
	}

	int r, c, h, w, k;
	knights.resize(N + 1);

	for (int i = 1; i <= N; i++) {
		cin >> r >> c >> h >> w >> k;
		knights[i] = { i, r, c, h, w, k, 0 };

		for (int top_u = r; top_u < r + h; top_u++) {
			for (int top_l = c; top_l < c + w; top_l++) {
				chess[top_u][top_l] = i;
			}
		}
	}
}

set<int> check_movabilty(int idx, int dir) {
	set<int> rst;
	for (int i = 1; i <= L; ++i) {
		fill(bfs[i], bfs[i] + L + 1, 0);
	}

	// bfs로 덩어리 만들기
	q = {};

	KNIGHT tar = knights[idx];
	rst.insert(idx);

	for (int top_u = tar.r; top_u < tar.r + tar.h; top_u++) {
		for (int top_l = tar.c; top_l < tar.c + tar.w; top_l++) {
			q.push({ top_u, top_l });
			bfs[top_u][top_l] = tar.idx;
		}
	}

	while (!q.empty()) {
		pii top = q.front(); q.pop();
		int nx = top.x + dx[dir], ny = top.y + dy[dir];
		int next_idx = chess[nx][ny];

		if (grid[nx][ny] == 2) return {}; // 벽 때문에 못 움직이는 경우

		if (bfs[nx][ny] != 0 || next_idx == 0) continue;
		if (rst.find(next_idx) == rst.end()) {
			rst.insert(next_idx);

			tar = knights[next_idx];
			for (int top_u = tar.r; top_u < tar.r + tar.h; top_u++) {
				for (int top_l = tar.c; top_l < tar.c + tar.w; top_l++) {
					q.push({ top_u, top_l });
					bfs[top_u][top_l] = tar.idx;
				}
			}
		}
	}
	return rst;
}

void move_knights(const set<int> & s, int dir) {
	for (auto i : s) {
		KNIGHT tar = knights[i];

		// erase
		for (int top_u = tar.r; top_u < tar.r + tar.h; top_u++) {
			for (int top_l = tar.c; top_l < tar.c + tar.w; top_l++) {
				chess[top_u][top_l] = 0;
			}
		}

		knights[i].r += dx[dir], knights[i].c += dy[dir];
	}

	for (auto i : s) {
		KNIGHT tar = knights[i];

		// fill
		for (int top_u = tar.r; top_u < tar.r + tar.h; top_u++) {
			for (int top_l = tar.c; top_l < tar.c + tar.w; top_l++) {
				chess[top_u][top_l] = i;
			}
		}
	}
}

void calc_damage(const set<int> & s) {
	// 데미지 계산
	for (auto i : s) {
		KNIGHT tar = knights[i];

		// 데미지 계산
		for (int top_u = tar.r; top_u < tar.r + tar.h; top_u++) {
			for (int top_l = tar.c; top_l < tar.c + tar.w; top_l++) {
				if (grid[top_u][top_l] == 1) {
					knights[i].life--, knights[i].dam++;
				}
			}
		}

		// 사망자 필터링
		if (knights[i].life <= 0) {
			for (int top_u = tar.r; top_u < tar.r + tar.h; top_u++) {
				for (int top_l = tar.c; top_l < tar.c + tar.w; top_l++) {
					chess[top_u][top_l] = 0;
				}
			}
		}
	}
}

void solve() {
	int idx, dir;
	for (int i = 0; i < Q; i++) {
		cin >> idx >> dir;
		if (knights[idx].life <= 0) continue; // 이미 죽은 경우 명령 무시
		set<int> lst = check_movabilty(idx, dir);
		if (lst.size() == 0) continue; // 이동 불가능 한 경우 명령 무시
		move_knights(lst, dir); // 기사들 이동
		lst.erase(idx); // 명령 받은 기사는 데미지 제외
		calc_damage(lst); // 데미지 계산 + 죽은 기사 격자에서 제거
	}

	int rst = 0;
	for (int i = 1; i <= N; ++i) {
		if (knights[i].life > 0) rst += knights[i].dam;
	}

	cout << rst;
}

int main() {
	// ios_base::sync_with_stdio(false);
	// cin.tie(nullptr);
	input();
	solve();
	return 0;
}