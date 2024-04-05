#include <iostream>
#include <queue>
#include <cstring>
#define endl '\n'
#define pii pair<int, int>
using namespace std;
constexpr size_t MAX_N = 10;
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
char box[MAX_N][MAX_N];
int n, m;
pii red, blue;

struct info{
    pii red, blue;
    int turn;
};

struct node{
    pii red, blue;
    int dir;
};

void init(){
    memset(box, 0, sizeof(box));
}

void input(){
    cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> box[i][j];

            if(box[i][j] == 'R') {
                red = {i, j};
                box[i][j] = '.';
            }

            else if(box[i][j] == 'B') {
                blue = {i, j};
                box[i][j] = '.';
            }
        }
    }
}


node mover(node & in){
    int check = 0; // 1: red, 2: blue;
    int rx = in.red.first, ry = in.red.second;
    int bx = in.blue.first, by = in.blue.second;

    bool other = false;
    while(true){
        int rnx = rx + dx[in.dir], rny = ry + dy[in.dir];

        if(box[rnx][rny] == '.'){
            if(make_pair(rnx, rny) == in.blue){
                other = true;
            }
            rx = rnx, ry = rny;
        }

        else if(box[rnx][rny] == '#'){
            if(other){
                rx = rx - dx[in.dir], ry = ry - dy[in.dir];
                break;
            }
            else break;
        }

        else if(box[rnx][rny] == 'O'){
            check++;
            break;
        }
    }

    other = false;
    while(true){
        int bnx = bx + dx[in.dir], bny = by + dy[in.dir];

        if(box[bnx][bny] == '.'){
            if(make_pair(bnx, bny) == in.red){
                other = true;
            }
            bx = bnx, by = bny;
        }

        else if(box[bnx][bny] == '#'){
            if(other){
                bx = bx - dx[in.dir], by = by - dy[in.dir];
                break;
            }
            else break;
        }

        else if(box[bnx][bny] == 'O'){
            check += 2;
            break;
        }
    }

    node tmp;
    tmp.blue = {bx, by};
    tmp.red = {rx, ry};
    tmp.dir = check;

    return tmp;
}

void run(){
    bool success = false;
    queue<info> q; // dir, turn
    info tmp1;
    tmp1.blue = blue;
    tmp1.red = red;
    tmp1.turn = 0;

    q.push(tmp1);

    while(!q.empty() && !success){
        info data = q.front();
        q.pop();

        if(data.turn == 10) continue;

        for (int i = 0; i < 4; ++i) {
            node in;
            in.red = data.red;
            in.blue = data.blue;
            in.dir = i;

            node tmp = mover(in);
            if(tmp.dir == 0) {
                info f;
                f.red = tmp.red;
                f.blue = tmp.blue;
                f.turn = data.turn + 1;
                q.push(f);
            }

            else if(tmp.dir == 1){
                cout << data.turn + 1;
                success = true;
                break;
            }
        }

    }

    if(!success) cout << -1;
}


int main() {
    init();
    input();
    run();

    return 0;
}