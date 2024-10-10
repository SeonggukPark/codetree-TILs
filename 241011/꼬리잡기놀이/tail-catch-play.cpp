#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>
using namespace std;

#define pii pair<int, int>
#define ll long long
#define x first
#define y second
#define endl '\n'
constexpr size_t MAX_N = 21;
int cur_turn, n, m, k, rst, grid[MAX_N][MAX_N], group[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
int dx[4] = { 0, -1, 0, 1 }; // right, up, left, down
int dy[4] = { 1, 0, -1, 0 };
vector<pii> headers, tails;
queue<pii> q;

void traverse_group() {
    cout << "Traverse group.." << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(2);
            cout << group[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

bool is_valid(int nx, int ny) {
    if (nx < 0 || ny < 0 || nx >= n || ny >= n) return false;
    return true;
}

void init() {
    int team_idx = 1;
    headers.push_back({ -1, -1 }), tails.push_back({ -1, -1 }); // idx 맞추기 위한 dummy node

    cin >> n >> m >> k;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];
            if (grid[i][j] == 0) group[i][j] = -1; // 벽인 경우
            if (grid[i][j] == 1) headers.push_back({ i, j });
        }
    }

    for (int i = 1; i < (int)headers.size(); ++i) {
        q = {};
        q.push({ headers[i].x, headers[i].y });
        pii top;

        while (!q.empty()) {
            top = q.front(); q.pop();
            group[top.x][top.y] = i;

            for (int dir = 0; dir < 4; ++dir) {
                int nx = top.x + dx[dir], ny = top.y + dy[dir];
                if (!is_valid(nx, ny)) continue;

                if ((grid[nx][ny] == 2 || grid[nx][ny] == 3) && group[nx][ny] == 0) {
                    q.push({ nx, ny });
                    break;
                }
            }
        }

        tails.push_back(top);
    }

    //  traverse_group();
}

void group_move() {
    for (int i = 1; i < (int)headers.size(); ++i) { // 각 그룹 머리 전진
        pii & cur_h = headers[i], & cur_t = tails[i];
        pii new_head, new_tail;

        // 새로운 머리 찾기
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur_h.x + dx[dir], ny = cur_h.y + dy[dir];
            if (!is_valid(nx, ny) || group[nx][ny] == -1) continue;
            if (make_pair(nx, ny) != cur_t && group[nx][ny] == i) continue;
            new_head = { nx, ny };
            break;
        }

        // 새로운 꼬리 찾기
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur_t.x + dx[dir], ny = cur_t.y + dy[dir];
            if (!is_valid(nx, ny) || group[nx][ny] != i) continue;
            if (make_pair(nx, ny) == cur_h) continue; // 머리-꼬리 이어진 경우 대비
            new_tail = { nx, ny };
            break;
        }

        // cout << "prev, new head: " << cur_h.x << ' ' << cur_h.y << ' ' << new_head.x << ' ' << new_head.y << endl;
        // cout << "prev, new tail: " << cur_t.x << ' ' << cur_t.y << ' ' << new_tail.x << ' ' << new_tail.y << endl;

        // 현재 꼬리 지우기
        group[cur_t.x][cur_t.y] = 0;
        cur_t = new_tail;

        // 새 머리 넣기
        cur_h = new_head;
        group[cur_h.x][cur_h.y] = i;
    }

    // traverse_group();
}

void throw_ball() {
    // 공 궤적 찾기
    pii pos;
    int ball_dir = ((cur_turn - 1) / n) % 4;
    int piv = (cur_turn - 1) % (4 * n);

    switch (ball_dir){
        case 0:
            pos = { piv, 0 };
            break;

        case 1:
            pos = { n - 1,  piv - n};
            break;

        case 2:
            pos = { (3 * n) - 1 - piv,  n - 1 };
            break;


        case 3:
            pos = { 0, (4 * n) - 1 - piv };
            break;

        default:
            break;
    }

    // cout <<pos.x << ' ' << pos.y << ' ' << ball_dir << endl;
    // cout << "cur_turn, dir: " << cur_turn << ' ' << ball_dir << endl;

    // 공 던져서 처음 맞은 사람 해당 그룹에서 위치 찾기
    while (true) {
        if (group[pos.x][pos.y] > 0) {
            int seq_cnt = 0, idx = group[pos.x][pos.y];

            // cout << "hit_info: " << pos.x << ' ' << pos.y << ' ' << idx << endl;

            pii top;
            pii & head = headers[idx], & tail = tails[idx];

            // bfs 돌려서 위치 찾기
            for (int i = 0; i < n; ++i) fill(visited[i], visited[i] + n, false);
            q = {};
            q.push({ head.x, head.y });
            visited[head.x][head.y] = true;

            while (!q.empty()) {
                top = q.front(); q.pop();
                seq_cnt++;

                if (pos.x == top.x && pos.y == top.y) {
                    rst += seq_cnt * seq_cnt;
                    swap(head, tail);
                    // cout << "head, tail: " << headers[idx].x << ' ' << headers[idx].y << ' ' << tails[idx].x << ' ' << tails[idx].y << endl;
                    return;
                }

                for (int dir = 0; dir < 4; ++dir) {
                    int nx = top.x + dx[dir], ny = top.y + dy[dir];
                    if (!is_valid(nx, ny) || visited[nx][ny] || group[nx][ny] != idx) continue;
                    if(make_pair(nx, ny) == tail && top == head) continue; // 꼬리랑 이어진 경우 대비
                    
                    q.push({ nx, ny });
                    visited[nx][ny] = true;
                    break;
                }
            }
        }

        pos.x += dx[ball_dir], pos.y += dy[ball_dir];
        if (!is_valid(pos.x, pos.y)) break; // 맞은 사람 없이 종료
    }
}

void solve() {
    cur_turn = 0;

    while (k--) {
        cur_turn++;
        group_move();
        throw_ball();
        // traverse_group();
    }

    cout << rst;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    init();
    solve();
    return 0;
}