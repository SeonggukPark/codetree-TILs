#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;
#define x first
#define y second
#define pii pair<int, int>
constexpr size_t MAX_N = 11;
int N, M, K, acc, map[MAX_N][MAX_N], cp_map[MAX_N][MAX_N];
int dx[4] = { 1, -1, 0, 0 }; // down, up, left, right
int dy[4] = { 0, 0, -1, 1 };
pii exit_pos;
vector<pii> person;
set<int> alive_list;

inline bool is_valid(int x, int y) {
    if (x <= 0 || y <= 0 || x > N || y > N) return false;
    if (map[x][y] > 0) return false; // 벽이 있는 경우
    return true;
}

inline int make_dist(pii p1, pii p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

void input() {
    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> map[i][j];
        }
    }

    person.resize(M);
    for (int i = 0; i < M; i++) {
        alive_list.insert(i);
        cin >> person[i].x >> person[i].y;
    }

    cin >> exit_pos.x >> exit_pos.y;
}

void move_person() {
    set<int> dead_list;

    for (int tar : alive_list) {
        pii pos = person[tar];
        int new_dist, tar_dist = make_dist(exit_pos, pos);

        for (int dir = 0; dir < 4; ++dir) {
            int nx = pos.x + dx[dir], ny = pos.y + dy[dir];
            if (!is_valid(nx, ny)) continue;
            new_dist = make_dist(exit_pos, { nx, ny });
            if (new_dist >= tar_dist) continue; // 거리가 안가까워지면 이동 X

           //  cout << "per, nx, ny: " << tar << ' ' << nx << ' ' << ny << endl;
            person[tar] = { nx, ny };
            acc++;
            if (person[tar].x == exit_pos.x && person[tar].y == exit_pos.y) dead_list.insert(tar);
            break;
        }
    }

    if (!dead_list.empty()) {
        for (int tar : dead_list) {
            alive_list.erase(tar);
        }
    }
}

void rotate_map() {
    if (alive_list.empty()) return;

    pii min_tar = { -1, 100 };

    for (int tar : alive_list) {
        int dist = max(abs(person[tar].x - exit_pos.x), abs(person[tar].y - exit_pos.y));
        if (min_tar.y > dist) {
            min_tar = { tar, dist };
        }
    }

    // 좌 하단 좌표, 센터 좌표 잡기
    pii r_d = { max(exit_pos.x, person[min_tar.x].x), max(exit_pos.y, person[min_tar.x].y) };
    pii u_l = { r_d.x - min_tar.y, r_d.y - min_tar.y };

    if (u_l.x <= 0) {
        r_d = { r_d.x - u_l.x + 1, r_d.y};
        u_l.x = 1;
    }

    if (u_l.y <= 0) {
        r_d = { r_d.x, r_d.y + u_l.y + 1 };
        u_l.y = 1;
    }

    // cout << "upper left: " << u_l.x << ' ' << u_l.y << endl;
    
    int len = min_tar.y + 1;


    bool moved[MAX_N], moved_exit = false;
    fill(moved, moved + MAX_N, false);

    for (int i = 0; i < len; i++){
        for (int j = 0; j < len; j++){
            // cout << i + u_l.x << ' ' << j + u_l.y << ' ' << len - 1 - j + u_l.x << ' ' << i + u_l.y << endl;

            cp_map[i + u_l.x][j + u_l.y] = map[len - 1 - j + u_l.x][i + u_l.y];

            for (int per : alive_list) {
                if (!moved[per] && person[per].x == len - 1 - j + u_l.x && person[per].y == i + u_l.y) {
                    person[per] = { i + u_l.x, j + u_l.y };
                    moved[per] = true;
                }
            }

            if (!moved_exit && exit_pos.x == len - 1 - j + u_l.x && exit_pos.y == i + u_l.y) {
                exit_pos = { i + u_l.x, j + u_l.y };
                moved_exit = true;
            }
        }
    }
    // cout << endl << endl;


    for (int r = u_l.x; r <= r_d.x; r++){
        for (int c = u_l.y; c <= r_d.y; c++){
            map[r][c] = cp_map[r][c];
            if (map[r][c] > 0) map[r][c]--;
        }
    }

    /*
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << map[i][j] << ' ';
        }
        cout << endl;
    }
    */
}

void solve() {
    acc = 0;

    int cnt = 1;
    while (K-- && !alive_list.empty()) {
        // cout << "Round: " << cnt++ << endl;
        move_person();
        rotate_map();
        // cout << "acc: " << acc << endl;

        /*
        for (pii per : person) {
            cout << "pos: " << per.x << ' ' << per.y << endl;
        }*/
    }

    cout << acc << endl;
    cout << exit_pos.x << ' ' << exit_pos.y;
}


int main() {
    input();
    solve();
    return 0;
}