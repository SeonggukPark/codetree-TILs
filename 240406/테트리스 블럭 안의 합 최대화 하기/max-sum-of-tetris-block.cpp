#include <iostream>
#include <cstring>
#define endl '\n'
#define pii pair<int, int>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

using namespace std;
constexpr size_t MAX_N = 20;
int n, m, max_sum;
int grid[MAX_N][MAX_N];

void init(){
    memset(grid, 0, sizeof(grid));
    max_sum = 0;
}

void input(){
    cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
        }
    }
}

void pat_1(){
    pii pat[2][4] = {{{0, 0}, {0, 1}, {0, 2}, {0, 3}}, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}};
    pii bound[2] = {{n, m - 3}, {n - 3, m}};

    for(int k = 0; k < 2; k++) {
        auto b = bound[k];
        for (int i = 0; i < b.first; ++i) {
            for (int j = 0; j < b.second; ++j) {
                int tmp = 0;

                for (auto &p: pat[k]) {
                    tmp += grid[i + p.first][j + p.second];
                }

                if (tmp > max_sum) max_sum = tmp;
            }
        }
    }
}

void pat_2(){
    pii pat[4] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    pii bound = {n - 1, m - 1};

    for (int i = 0; i < bound.first; ++i) {
        for (int j = 0; j < bound.second; ++j) {
            int tmp = 0;

            for (auto &p: pat) {
                tmp += grid[i + p.first][j + p.second];
            }

            if (tmp > max_sum) max_sum = tmp;
        }
    }
}

void pat_3(){
    pii pat[8][4] = {{{0, 0}, {1, 0}, {2, 0}, {2, 1}}, {{1, 0}, {1, 1}, {1, 2}, {0, 2}},
                     {{0, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {1, 0}, {0, 1}, {0, 2}},
                     {{2, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {0, 1}, {1, 2}, {0, 2}},
                     {{0, 0}, {0, 1}, {1, 0}, {2, 0}}, {{0, 0}, {1, 0}, {1, 1}, {1, 2}}};

    pii bound[8] = {{n - 2, m - 1}, {n - 1, m - 2}, {n - 2, m - 1}, {n - 1, m - 2},
                    {n - 2, m - 1}, {n - 1, m - 2}, {n - 2, m - 1}, {n - 1, m - 2}};

    for(int k = 0; k < 8; k++) {
        auto b = bound[k];
        for (int i = 0; i < b.first; ++i) {
            for (int j = 0; j < b.second; ++j) {
                int tmp = 0;

                for (auto &p: pat[k]) {
                    tmp += grid[i + p.first][j + p.second];
                }

                if (tmp > max_sum) max_sum = tmp;
            }
        }
    }
}

void pat_4(){
    pii pat[4][4] = {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {{1, 0}, {0, 1}, {1, 1}, {0, 2}},
                     {{0, 1}, {0, 2}, {1, 1}, {1, 0}}, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}};
    pii bound[4] = {{n - 2, m - 1}, {n - 1, m - 2}, {n - 2, m - 1}, {n - 1, m - 2}};

    for(int k = 0; k < 4; k++) {
        auto b = bound[k];
        for (int i = 0; i < b.first; ++i) {
            for (int j = 0; j < b.second; ++j) {
                int tmp = 0;

                for (auto &p: pat[k]) {
                    tmp += grid[i + p.first][j + p.second];
                }

                if (tmp > max_sum) max_sum = tmp;
            }
        }
    }
}

void pat_5(){
    pii pat[4][4] = {{{0, 0}, {1, 0}, {2, 0}, {1, 1}}, {{1, 0}, {0, 1}, {1, 1}, {1, 2}},
                     {{1, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {1, 1}, {0, 1}, {0, 2}}};

    pii bound[4] = {{n - 2, m - 1}, {n - 1, m - 2}, {n - 2, m - 1}, {n - 1, m - 2}};

    for(int k = 0; k < 4; k++) {
        auto b = bound[k];
        for (int i = 0; i < b.first; ++i) {
            for (int j = 0; j < b.second; ++j) {
                int tmp = 0;

                for (auto &p: pat[k]) {
                    tmp += grid[i + p.first][j + p.second];
                }

                if (tmp > max_sum) max_sum = tmp;
            }
        }
    }
}

void run(){
    pat_1();
    pat_2();
    pat_3();
    pat_4();
    pat_5();
}


int main() {
    fastio
    init();
    input();
    run();

    cout << max_sum << endl;
    return 0;
}