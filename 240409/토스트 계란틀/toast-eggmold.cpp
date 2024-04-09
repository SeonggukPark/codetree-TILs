#include <iostream>
#include <cstring>
#include <vector>
#define endl '\n'
#define fastio ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_SIZE = 50;
int grid[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];
vector<pair<int, int> > v;
int n, l, r, rst;

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

void init(){
    memset(grid, 0, sizeof(grid));
    memset(visited, false, sizeof(visited));
}

void input(){
    cin >> n >> l >> r;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];
        }
    }
}

pair<int, int> dfs(int x, int y){
    v.emplace_back(x, y);
    int num = grid[x][y];
    int cnt = 1;
    int nx, ny;
    visited[x][y] = true;

    for(int i = 0; i < 4; i++){
        nx = x + dx[i], ny = y + dy[i];

        if(nx < 0 || nx >= n || ny < 0 || ny >= n) continue;

        if(!visited[nx][ny] && abs(grid[nx][ny] - grid[x][y]) >= l && abs(grid[nx][ny] - grid[x][y]) <= r){
            auto tmp = dfs(nx, ny);
            num += tmp.first;
            cnt += tmp.second;
        }
    }

    return {num, cnt};
}


bool is_possible(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i != 0){
                int diff = abs(grid[i - 1][j] - grid[i][j]);
                if(diff >= l && diff <= r) return false;
            }

            if(i != n - 1){
                int diff = abs(grid[i + 1][j] - grid[i][j]);
                if(diff >= l && diff <= r) return false;
            }

            if(j != 0){
                int diff = abs(grid[i][j - 1] - grid[i][j]);
                if(diff >= l && diff <= r) return false;
            }

            if(j != n - 1){
                int diff = abs(grid[i][j + 1] - grid[i][j]);
                if(diff >= l && diff <= r) return false;
            }
        }
    }

    return true;
}


void run(){
    rst = 0;
    while(!is_possible()){
        rst++;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                v.clear();
                auto data = dfs(i, j);
                int new_data = data.first / data.second;

                if(v.empty()) continue;
                for(auto k : v){
                    grid[k.first][k.second] = new_data;
                }
            }
        }

        memset(visited, false, sizeof(visited));
    }
}

void traverse(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << grid[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void traverse_pos(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << visited[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    fastio
    init();
    input();
    run();

    cout << rst;
    return 0;
}