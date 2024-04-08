#include <iostream>
#include <stack>
#include <vector>
#include <cstring>
using namespace std;
constexpr size_t MAX_N = 10, MAX_H = 30;
int n, m, h, rst;
int grid[MAX_H][MAX_N];

void init(){
    memset(grid, 0, sizeof(grid));
    rst = 4;
}

void traverse(){
    for(int i = 0; i < h; i++){
        for (int j = 0; j < n - 1; ++j) {
            cout << grid[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}


void input(){
    int x, y;
    cin >> n >> m >> h;


    while(m--){
        cin >> x >> y;
        grid[x - 1][y - 1] = 1;
        // cout << "x, y: " << x-1 <<' ' << y-1 << endl;
    }
}

bool is_possible() {
    for (int i = 0; i < n - 1; i++) {
        int start = i;
        for (int j = 0; j < h; j++) {
            if (grid[j][start] == 1) {
                start += 1;
            } else if (start - 1 >= 0 && grid[j][start - 1] == 1) {
                start -= 1;
            }
        }
        if (start != i) return false;
    }
    return true;
}


void dfs(int x, int y, int depth){
    if(depth > 3) return;

    for(int i = x; i < h; i++){
        for (int j = y; j < n - 1; ++j) {
            if(grid[i][j] == 0) {

                // 양 옆 확인
                if(j != 0 && grid[i][j - 1] == 1){
                    continue;
                }

                if(j != n - 2 && grid[i][j + 1] == 1){
                    continue;
                }

                grid[i][j] = 1;

                if(is_possible()) rst = min(rst, depth);

                dfs(i, j, depth + 1);
                grid[i][j] = 0;
            }
        }
    }


}

void run(){
    // 초기 상태가 이미 해결된 경우
    if(is_possible()){
        rst = 0;
        return;
    }

    dfs(0, 0, 1);
}


int main() {
    init();
    // traverse();

    input();

    // traverse();
    run();

    rst == 4 ? cout << -1 : cout << rst;
    return 0;
}