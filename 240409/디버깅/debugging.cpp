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
            cout << i << ' ' << j << ' ' << grid[i][j] << ' ';
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


bool is_possible(){
    stack<vector<int> > s;
    vector<int> v;
    bool flag, null_flag;

    for(int i = 0; i < h; i++){
        // cout << "row, size, top: " << i << ' ' << s.size() << endl;
        v.clear();
        null_flag = true;
        flag = false;

        for(int j = 0; j < n - 1; j++){
            v.push_back(grid[i][j]);
            if(grid[i][j] != 0) null_flag = false;
        }

        if(null_flag) continue;

        if(s.empty()) {
            s.push(v);
            continue;
        }

        vector<int> tmp = s.top();


        for(int j = 0; j < n - 1; j++){
            // cout << tmp[j] << ' ' << v[j] << endl;
            if(tmp[j] != v[j]) {
                flag = true;
                break;
            }
        }

        if(!flag){
            s.pop();
            continue;
        }

        s.push(v);
    }

    if(s.empty()) return true;
    return false;
}

void bfs(int x, int y, int depth){
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

                bfs(i, j, depth + 1);
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

    bfs(0, 0, 1);
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