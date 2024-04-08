#include <iostream>
#include <cstring>
#include <vector>

using namespace std;
constexpr size_t MAX_NUM = 8;
int n, m, rst;
int grid[MAX_NUM][MAX_NUM];

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

struct chess{
    int idx;
    pair<int, int> pos;
};

vector<chess> v;

void init(){
    memset(grid, 0, sizeof(grid));
    v.clear();
    rst = MAX_NUM * MAX_NUM + 1;
}

void input(){
    cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];

            if(grid[i][j] != 0 && grid[i][j] != 5 && grid[i][j] != 6){
                chess tmp;
                tmp.pos = {i, j};
                tmp.idx = grid[i][j];
                v.push_back(tmp);
            }
        }
    }
}

int count_empty(){
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if(grid[i][j] == 0) cnt++;
        }
    }
    return cnt;
}

void fill_line(int x, int y, int dir){
    int nx = x + dx[dir], ny = y + dy[dir];

    while(nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] != 6){
        if(grid[nx][ny] == 0) grid[nx][ny] = 7;
        nx += dx[dir], ny += dy[dir];
    }
}

void fill_5(){ // 5는 회전 영향 안 받음 -> 미리 채우기
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if(grid[i][j] == 5){

                for(int k = 0; k < 4; k++){
                    fill_line(i, j, k);
                }
            }
        }
    }
}

void color(int i, int head_dir){ // head_dir: 0 ~ 3 -> E, W, S, N
    int idx = v[i].idx, x = v[i].pos.first, y = v[i].pos.second;

    // 1번 말은 한 칸만
    if(idx == 1){
        fill_line(x, y, head_dir);
        return;
    }

    // 2번 말은 두 방향
    if(idx == 2){
        if(head_dir == 0 || head_dir == 1){ // 동서 방향 머리
            fill_line(x, y, 0);
            fill_line(x, y, 1);
        }

        else if(head_dir == 2 || head_dir == 3){ // 남북 방향 머리
            fill_line(x, y, 2);
            fill_line(x, y, 3);
        }

        return;
    }

    // 3번 말은 헤드 + 헤드 오른쪽
    if(idx == 3){
        switch(head_dir){
            case 0:
                fill_line(x, y, 0);
                fill_line(x, y, 2);
                break;

            case 1:
                fill_line(x, y, 1);
                fill_line(x, y, 3);
                break;

            case 2:
                fill_line(x, y, 2);
                fill_line(x, y, 1);
                break;

            case 3:
                fill_line(x, y, 3);
                fill_line(x, y, 0);
                break;
        }

        return;
    }


    if(idx == 4){
        switch(head_dir){
            case 0:
                fill_line(x, y, 0);
                fill_line(x, y, 2);
                fill_line(x, y, 3);
                break;

            case 1:
                fill_line(x, y, 1);
                fill_line(x, y, 2);
                fill_line(x, y, 3);
                break;

            case 2:
                fill_line(x, y, 2);
                fill_line(x, y, 0);
                fill_line(x, y, 1);
                break;

            case 3:
                fill_line(x, y, 3);
                fill_line(x, y, 0);
                fill_line(x, y, 1);
                break;
        }

        return;
    }
}

void bfs(int depth){
    if(depth == v.size()){
        int cnt = count_empty();
        if(cnt < rst) rst = cnt;
        return;
    }

    int cp_grid[MAX_NUM][MAX_NUM];
    memset(cp_grid, 0, sizeof(grid));


    for(int k = 0; k < 4; k++){
        // grid backup
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cp_grid[i][j] = grid[i][j];
            }
        }

        // dir 헤드 방향 색칠
        color(depth, k);
        bfs(depth + 1);

        // 복귀
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                grid[i][j] = cp_grid[i][j];
            }
        }
    }
}

void run() {
    fill_5();
    bfs(0);
}


int main() {
    freopen("input.txt", "r", stdin);
    init();
    input();
    run();
    cout << rst;

    return 0;
}