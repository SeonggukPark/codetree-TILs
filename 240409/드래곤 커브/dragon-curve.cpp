#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
#define endl '\n'
#define fastio ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_SIZE = 101;
int n, rst;
int grid[MAX_SIZE][MAX_SIZE];
int dx[4] = {0, -1, 0, 1};
int dy[4] = {1, 0, -1, 0};
stack<int> s;
vector<int> v;

void init(){
    memset(grid, 0, sizeof(grid));
    s = {};
    v.clear();
}

void input(){
    cin >> n;
}

void dfs(int x, int y, int g, int depth){
    int nx = x, ny = y;
    if(depth == g) return;

    while(!s.empty()){
        v.push_back((s.top() + 1) % 4);
        s.pop();
        nx += dx[v.back()], ny += dy[v.back()];
        grid[nx][ny] = 1;
    }

    for(auto i : v){
        s.push(i);
    }

    dfs(nx, ny, g, depth + 1);
}

void square_count(){
    rst = 0;
    for (int i = 0; i < MAX_SIZE - 1; ++i) {
        for (int j = 0; j < MAX_SIZE - 1; ++j) {
            if (grid[i][j] == 1 && grid[i + 1][j] == 1 && grid[i][j + 1] == 1 && grid[i + 1][j + 1] == 1) {
                rst++;
            }
        }
    }
}

void run(){
    int x, y, d, g;

    while(n--){
        cin >> x >> y >> d >> g;
        s = {};
        v.clear();
        s.push(d);
        grid[x][y] = 1;
        int nx = x + dx[d], ny = y + dy[d];
        grid[nx][ny] = 1;

        v.push_back(d);
        dfs(nx, ny, g, 0);
    }

    square_count();
}

int main() {
    fastio
    init();
    input();
    run();

    cout << rst;
    return 0;
}