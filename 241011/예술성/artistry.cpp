#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

#define pii pair<int, int>
#define x first
#define y second
#define endl '\n'
constexpr size_t MAX_N = 30;
int n, acc, grid[MAX_N][MAX_N], cp_grid[MAX_N][MAX_N], group[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
int dx[4] = { 0, 0, -1, 1 };
int dy[4] = { 1, -1, 0, 0 };

queue<pii> q;

struct GROUP {
    pii pos;
    int cnt;
};

vector<GROUP> group_info;

void init() {
    acc = 0;

    cin >> n;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
}

bool is_valid(int nx, int ny) {
    if (nx < 0 || ny < 0 || nx >= n || ny >= n) return false;
    return true;
}

void make_score() {
    for (int i = 0; i < n; ++i) fill(group[i], group[i] + n, -1);
    group_info.clear();

    int idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (group[i][j] != -1) continue;

            group[i][j] = idx; 
            group_info.push_back({ {i, j}, 1 });

            q = {};
            q.push({ i, j });

            while (!q.empty()) {
                pii top = q.front(); q.pop();

                for (int dir = 0; dir < 4; ++dir) {
                    int nx = top.x + dx[dir], ny = top.y + dy[dir];
                    if (!is_valid(nx, ny) || group[nx][ny] != -1 || grid[top.x][top.y] != grid[nx][ny]) continue;
                    group[nx][ny] = idx;
                    group_info[idx].cnt++;
                    q.push({ nx, ny });
                }
            }

            idx++;
        }
    }


    // 점수 집계
    int groud_cnt = (int)group_info.size();

    for (int i = 0; i < groud_cnt - 1; i++){
        for (int j = i + 1; j < groud_cnt; j++) {

            for (int k = 0; k < n; ++k) fill(visited[k], visited[k] + n, false);

            GROUP& g_i = group_info[i], g_j = group_info[j];
            int two_acc = (g_i.cnt + g_j.cnt) * grid[g_i.pos.x][g_i.pos.y] * grid[g_j.pos.x][g_j.pos.y];
            int line_cnt = 0;

            q = {};
            q.push({ g_i.pos.x, g_i.pos.y });
            visited[g_i.pos.x][g_i.pos.y] = true;

            while (!q.empty()) {
                pii top = q.front(); q.pop();

                for (int dir = 0; dir < 4; ++dir) { 
                    int nx = top.x + dx[dir], ny = top.y + dy[dir];
                    if (!is_valid(nx, ny)) continue;
                    if (group[nx][ny] == j) line_cnt++; // 변 개수 count
                    if (group[nx][ny] == i && !visited[nx][ny]) {
                        q.push({ nx, ny }); // 덩어리 q에 넣기
                        visited[nx][ny] = true;
                    }
                }
            }
            
            two_acc *= line_cnt;
            acc += two_acc;
        }
    }
}

void rotate_map() {
    // 십자 모양
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cp_grid[i][j] = grid[j][n - i - 1];
        }
    }

    // 4개 정사각형
    int len = (n - 1) / 2;
    pii seq[4] = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };

    for (int rpt = 0; rpt < 4; ++rpt) {
        pii cor = { (len + 1) * seq[rpt].x, (len + 1) * seq[rpt].y };

        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                cp_grid[i + cor.x][j + cor.y] = grid[len - j - 1 + cor.x][i + cor.y];
            }
        }
    }

    // 원본 변경
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = cp_grid[i][j];
        }
    }
}

void solve() {
    for (int i = 0; i < 4; ++i) {
        make_score();// 점수 구해서 누적
        rotate_map();// 회전
    }

    cout << acc;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // freopen("input.txt", "r", stdin);
    init();
    solve();
    return 0;
}