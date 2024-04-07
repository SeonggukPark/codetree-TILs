#include <iostream>
#include <cstring>
using namespace std;
constexpr size_t MAX_N = 100;
int n, l, rst;
int grid[MAX_N][MAX_N];
int cp_grid[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];

void init(){
    memset(grid, 0, sizeof(grid));
    memset(visited, false, sizeof(visited));
    rst = 0;
}

void input(){
    cin >> n >> l;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];
        }
    }
}

void run(){
    // 1. 행 scan
    for (int i = 0; i < n; ++i){
        int dir, idx, pivot, prev, next = 0;
        bool flag = false;

        while(next < n - 1){
            prev = next, next++;

            if(grid[i][prev] == grid[i][next]) continue;
            if(abs(grid[i][prev] - grid[i][next]) != 1) {
                flag = true;
                break;
            }

            if(grid[i][prev] > grid[i][next]){
                dir = 1;
                idx = next;
                pivot = prev;
            }

            else{
                dir = -1;
                idx = prev;
                pivot = next;
            }


            for (int k = 0; k < l; ++k) {
                if(idx < 0 || idx >= n || visited[i][idx] || grid[i][pivot] - grid[i][idx] != 1){
                    flag = true;
                    break;
                }

                visited[i][idx] = true;
                idx += dir;
            }

            if(flag){
                break;
            }
        }


        if(!flag){
            rst++;
        }
    }

    memset(visited, false, sizeof(visited));
}

void rotate_grid(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cp_grid[i][j] = grid[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            grid[j][n - 1 - i] = cp_grid[i][j];
        }
    }
}


int main() {
    init();
    input();

    run();
    rotate_grid();
    run();

    cout << rst;
    return 0;
}