#include <iostream>
#include <queue>
#include <cstring>
#define endl '\n'
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

using namespace std;
constexpr size_t MAX_N = 20;
int n, max_block;
int grid[MAX_N][MAX_N];

void init(){
    memset(grid, 0, sizeof(grid));
}

void input(){
    cin >> n;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];
        }
    }
}

void shift(int dir){
    queue<int> q;

    switch(dir){
        case 0: // east
            for (int i = 0; i < n; ++i) {
                for (int j = n - 1; j >= 0; --j) {
                    if(grid[i][j]) {
                        q.push(grid[i][j]);
                        grid[i][j] = 0;
                    }
                }

                int idx = n - 1;

                while(!q.empty()){
                    int next = q.front();
                    q.pop();

                    if(grid[i][idx] == 0) grid[i][idx] = next;

                    else if(grid[i][idx] == next){
                        grid[i][idx] += next;
                        idx--;
                    }

                    else{
                        idx--;
                        grid[i][idx] = next;
                    }
                }
            }
            break;

        case 1: // west
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if(grid[i][j]) {
                        q.push(grid[i][j]);
                        grid[i][j] = 0;
                    }
                }

                int idx = 0;

                while(!q.empty()){
                    int next = q.front();
                    q.pop();

                    if(grid[i][idx] == 0) grid[i][idx] = next;

                    else if(grid[i][idx] == next){
                        grid[i][idx] += next;
                        idx++;
                    }

                    else{
                        idx++;
                        grid[i][idx] = next;
                    }
                }
            }
            break;

        case 2: // north
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if(grid[j][i]) {
                        q.push(grid[j][i]);
                        grid[j][i] = 0;
                    }
                }

                int idx = 0;

                while(!q.empty()){
                    int next = q.front();
                    q.pop();

                    if(grid[idx][i] == 0) grid[idx][i] = next;

                    else if(grid[idx][i] == next){
                        grid[idx][i] += next;
                        idx++;
                    }

                    else{
                        idx++;
                        grid[idx][i] = next;
                    }
                }
            }
            break;

        case 3: // south
            for (int i = 0; i < n; ++i) {
                for (int j = n - 1; j >= 0; --j) {
                    if(grid[j][i]) {
                        q.push(grid[j][i]);
                        grid[j][i] = 0;
                    }
                }

                int idx = n - 1;

                while(!q.empty()){
                    int next = q.front();
                    q.pop();

                    if(grid[idx][i] == 0) grid[idx][i] = next;

                    else if(grid[idx][i] == next){
                        grid[idx][i] += next;
                        idx--;
                    }

                    else{
                        idx--;
                        grid[idx][i] = next;
                    }
                }
            }
            break;
    }
}

void dfs(int depth){
    if(depth == 5){
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                max_block = max(max_block, grid[i][j]);
            }
        }

        return;
    }

    int cp_grid[MAX_N][MAX_N];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cp_grid[i][j] = grid[i][j];
        }
    }

    for (int k = 0; k < 4; ++k) {
        shift(k);
        dfs(depth + 1);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                grid[i][j] = cp_grid[i][j];
            }
        }
    }
}



int main() {
    fastio
    init();
    input();
    dfs(0);

    cout << max_block << endl;
    return 0;
}