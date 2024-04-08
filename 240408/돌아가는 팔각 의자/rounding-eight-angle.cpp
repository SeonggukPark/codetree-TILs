#include <iostream>
#include <deque>
#include <cstring>
#include <string>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;
int k, n, d;
deque<int> table[4];

void init(){
    for(auto i : table){
        i.clear();
    }
}

void input(){
    string str;

    for(auto & i : table){
        cin >> str;

        for(int j = 0; j < 8; j++){
            i.push_back(str[j] - '0');
        }
    }

    cin >> k;
}

void rotate(int idx, int dir){
    if(dir == 1){  // 시계 방향
        int tar = table[idx].back();
        table[idx].pop_back();
        table[idx].push_front(tar);
    }

    else{ // 반시계 방향
        int tar = table[idx].front();
        table[idx].pop_front();
        table[idx].push_back(tar);
    }
}

void run(){
    while(k--){
        cin >> n >> d;

        switch(n){
            case 1:
                if(table[0][2] != table[1][6]){
                    if(table[1][2] != table[2][6]){
                        if(table[2][2] != table[3][6]) rotate(3, -d);
                        rotate(2, d);
                    }
                    rotate(1, -d);
                }
                rotate(0, d);
                break;

            case 2:
                if(table[0][2] != table[1][6]){
                    rotate(0, -d);
                }

                if(table[1][2] != table[2][6]){
                    if(table[2][2] != table[3][6]){
                        rotate(3, d);
                    }
                    rotate(2, -d);
                }
                rotate(1, d);
                break;

            case 3:
                if(table[2][2] != table[3][6]){
                    rotate(3, -d);
                }

                if(table[1][2] != table[2][6]){
                    if(table[0][2] != table[1][6]){
                        rotate(0, d);
                    }
                    rotate(1, -d);
                }
                rotate(2, d);
                break;

            case 4:
                if(table[2][2] != table[3][6]){
                    if(table[1][2] != table[2][6]){
                        if(table[0][2] != table[1][6]) rotate(0, -d);
                        rotate(1, d);
                    }
                    rotate(2, -d);
                }
                rotate(3, d);
                break;
        }
    }

    int rst = 0;
    for(int i = 3; i >= 0; i--){
        rst *= 2;
        rst += table[i][0];
    }

    cout << rst;
}

int main() {
    fastio
    init();
    input();
    run();

    return 0;
}