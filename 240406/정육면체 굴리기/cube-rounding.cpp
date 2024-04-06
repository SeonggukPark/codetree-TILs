#include <iostream>
#include <cstring>
#define endl '\n'
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_SIZE = 20;
int n, m, k;
int grid[MAX_SIZE][MAX_SIZE];
int dx[4] = {0, 0, -1, 1};
int dy[4] = {1, -1, 0, 0};
int dice[6] = {0, 1, 5, 4, 3, 2}; // 밑, 위, 동, 서, 북, 남
int dice_match[6] = {1, 0, 3, 2, 5, 4};
int dice_num[6] = {0, 0, 0, 0, 0, 0};

struct Pos{
    int x, y;
};

Pos pos;

void init(){
    memset(grid, 0 , sizeof(grid));
}

void input(){
    cin >> n >> m;
    cin >> pos.x >> pos.y >> k;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
        }
    }
}

bool is_move(int dir){
    int nx = pos.x + dx[dir - 1], ny = pos.y + dy[dir - 1];

    if(nx >= 0 && ny >= 0 && nx < n && ny < m){
        pos.x = nx, pos.y = ny;
        return true;
    }

    return false;
}

void run(){
    int dir;
    bool flag;
    int tmp[6] = {0, 0, 0, 0, 0, 0};
    
    while(k--){
        cin >> dir;
        flag = is_move(dir);
        if(!flag) continue; // 못 움직이면 skip

        tmp[0] = dice[dir + 1];
        tmp[1] = dice_match[tmp[0]];

        if(dir == 1 || dir == 2){
            tmp[2] = dice[2 - dir];
            tmp[3] = dice_match[tmp[2]];
            tmp[4] = dice[4];
            tmp[5] = dice[5];
        }

        else if(dir == 3 || dir == 4){
            tmp[2] = dice[2];
            tmp[3] = dice[3];
            tmp[4] = dice[4 - dir];
            tmp[5] = dice_match[tmp[4]];
        }

        for (int i = 0; i < 6; ++i) {
            dice[i] = tmp[i];
        }

        // case 1. 칸 수 0, 주사위 바닥 -> 칸
        if(grid[pos.x][pos.y] == 0){
            grid[pos.x][pos.y] = dice_num[dice[0]];
        }


        // case 2. 칸 수 0x, 칸 -> 주사위 바닥, 해당 칸 0
        else{
            dice_num[dice[0]] = grid[pos.x][pos.y];
            grid[pos.x][pos.y] = 0;
        }

        cout << dice_num[dice[1]] << endl;
    }
}

int main() {
    fastio
    init();
    input();
    run();
    return 0;
}