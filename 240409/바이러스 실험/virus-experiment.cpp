#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#define endl '\n'
using namespace std;
constexpr size_t MAX_SIZE = 10;
int grid[MAX_SIZE][MAX_SIZE];
int adder[MAX_SIZE][MAX_SIZE];
int n, m, k;
int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
int dy[8] = {1, 0, -1, 1, -1, 1, 0, -1};

struct virus{
    int x, y;
    int age;
    bool alive;
};

/*
struct cmp{
    bool operator()(const virus & a, const virus & b){
        return a.age < b.age;
    }
};*/

bool cmp(const virus & a, const virus & b){
    return a.age < b.age;
};

vector<virus> v, virus_v;

void init(){
    memset(grid, 0, sizeof(grid));
    memset(adder, 0, sizeof(adder));
    v.clear();
}

void input(){
    cin >> n >> m >> k;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            grid[i][j] = 5;
            cin >> adder[i][j];
        }
    }

    for (int i = 0; i < m; ++i) {
        virus tmp{};
        cin >> tmp.x >> tmp.y >> tmp.age;
        tmp.x--, tmp.y--;
        tmp.alive = true;
        v.push_back(tmp);
    }
}

void scan_grid(){
    cout << "grid scan: " << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << grid[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void traverse_v(){
    for(auto i : v){
        cout << i.x << ' ' << i.y << ' ' << i.age << endl;
    }
    cout << endl;
}

void consume_nut(){
    for(auto & i : v){
        // 양분 부족하면 바로 사망
        if(grid[i.x][i.y] < i.age){
            i.alive = false;
            continue;
        }

        // 양분 섭취 후 나이 1 증가
        grid[i.x][i.y] -= i.age;
        i.age++;
    }
}

void virus_ground(){
    for(auto iter = v.begin(); iter != v.end();){
        // 살아 있으면 지나가기
        if(iter->alive) {
            iter++;
            continue;
        }

        // 사망한 바이러스 제거 & grid 양분 충전
        grid[iter->x][iter->y] += (iter->age) / 2;
        v.erase(iter);
    }
}

void virus_increase(){
    virus_v.clear();
    for(auto & i : v){
        // 나이 5의 배수 아니면 지나가기
        if(i.age % 5 != 0) continue;

        // 8방향 돌면서 grid 안이면 바이러스 추가
        for(int j = 0; j < 8; j++){
            int nx = i.x + dx[j], ny = i.y + dy[j];

            // grid 밖인 경우 pass
            if(nx < 0 || nx >= n || ny < 0 || ny >= n) continue;

            virus tmp{};
            tmp.x = nx, tmp.y = ny, tmp.age = 1, tmp.alive = true;
            virus_v.push_back(tmp);
        }
    }

    for(auto & i : virus_v){
        v.push_back(i);
    }
}

void grid_increase(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            grid[i][j] += adder[i][j];
        }
    }
}

void run(){
    while(k-- && !v.empty()){
        // 0. virus 나이 순 정렬
        sort(v.begin(), v.end(), cmp);
        //traverse_v();

        // 1. 양분 섭취
        consume_nut();

        // 2. 바이러스 흙으로
        virus_ground();

        // 3. 바이러스 번식
        virus_increase();

        // 4. grid 양분 추가
        grid_increase();


        // cout << k << endl;
        //traverse_v();
        // scan_grid();
    }

    cout << v.size();
}

int main() {
    init();
    input();
    run();

    return 0;
}