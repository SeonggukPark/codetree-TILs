#include <iostream>
#include <cstring>
using namespace std;
constexpr size_t MAX_N = 100;
int n, l, rst;
int grid[MAX_N][MAX_N];
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

    // 2. 열 scan
    for (int i = 0; i < n; ++i){
        int dir, idx, pivot, prev, next = 0;
        bool flag = false;

        while(next < n - 1){
            prev = next, next++;

            if(grid[prev][i] == grid[next][i]) continue;
            if(abs(grid[prev][i] - grid[next][i]) != 1) {
                flag = true;
                break;
            }

            if(grid[prev][i] > grid[next][i]){
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
                if(idx < 0 || idx >= n || visited[idx][i] || grid[pivot][i] - grid[idx][i] != 1){
                    flag = true;
                    break;
                }

                visited[idx][i] = true;
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
}


int main() {
    init();
    input();
    run();

    cout << rst;
    return 0;
}