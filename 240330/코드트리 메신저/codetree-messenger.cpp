#include <iostream>
#include <vector>
#include <cstring>
#define endl '\n'
#define fastio ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_TREE = 100001;
int N, Q;

struct tree{
    int id, auth;
    int par, right, left;
    int trans[20];
    bool is_alarm;
    int depth;
};

tree trees[MAX_TREE];

void init(){
    for(int i = 0; i <= MAX_TREE; ++i){
        trees[i].id = -1;
        trees[i].auth = -1;
        trees[i].par = -1;
        trees[i].right = -1;
        trees[i].left = -1;
        memset(trees[i].trans, 0, sizeof(trees[i].trans));
        trees[i].is_alarm = true;
        trees[i].depth = -1;

    }
}

void traverse(){
    for(int i = 1; i <= N; i++) {
        cout << "node[" << i << "] trans: ";

        for (int j = 1; j <= trees[i].depth; j++) {
            cout << trees[i].trans[j] << ' ';
        }

        cout << endl;
    }
}

void depth_maker(int start, int depth){
    if (start == -1) return;
    trees[start].depth = depth;
    depth_maker(trees[start].left, depth + 1);
    depth_maker(trees[start].right, depth + 1);
}


void prepare() { // 1. 메신저 준비
    // N - 1 번 id & par 입력, par의 left - right 갱신
    int input;
    trees[0].id = 0;

    for (int i = 1; i <= N; ++i) {
        trees[i].id = i;
        cin >> input;
        trees[input].left == -1 ? trees[input].left = i : trees[input].right = i;
        trees[i].par = input;
    }

    depth_maker(0, 0);

        // N - 1 번 auth 입력, 부모 타고 가면서 trans 갱신
    for (int i = 1; i <= N; ++i) {
        cin >> input;
        trees[i].auth = input;
        int par = i;

        for (int j = 0; j < input; j++) {
            for (int k = 1; k <= input - j; k++) {
                trees[par].trans[k]++;
            }

            par = trees[par].par;
        }
    }
}

void set_alarm(){ // 2. 알림망 설정 on/off
    // 채팅방 번호 입력
    int input;
    cin >> input;

    int mode, par;
    if(trees[input].is_alarm){
        mode = 0;
        trees[input].is_alarm = false;
    }

    else{
        mode = 1;
        trees[input].is_alarm = true;
    }

    par = trees[input].par;
    int child = input;
    int cnt = 1;

    // 해당 node trans -> tree 타고 올라가며 갱신
    if(!mode) { // 1. 알람 켜져 있던 경우 -> 올라가며 빼주기
        trees[child].trans[1] = 0;

        while(par != 0){
            for(int i = 1; i <= trees[par].depth; i++){
                trees[par].trans[i] -= trees[child].trans[i + cnt];
            }

            cnt++;
            par = trees[par].par;
        }

        return;
    }

    // 2. 알람 꺼져 있던 경우 -> 올라가며 더해 주기
    int left, right;
    trees[par].left == -1 ? left = 0 : left = trees[trees[input].left].trans[2];
    trees[input].right == -1 ? right = 0 : right = trees[trees[input].right].trans[2];
    trees[input].trans[1] = left + right + 1;

    while(par != 0){
        for(int i = 1; i <= trees[par].depth; i++){
            trees[par].trans[i] += trees[child].trans[i + cnt];
        }
        cnt++;
        par = trees[par].par;
    }
}

void change_auth(){ // 3. 권한 세기 변경
    // 채팅방 번호, 권한 입력
    int input, auth;
    cin >> input >> auth;

    int prev = trees[input].auth;
    int par = input;
    // 해당 node trans 갱신 -> tree 타고 가면서 갱신

    while(par != -1 && prev > 0){
        for(int i = 1; i <= prev; i++){
            trees[par].trans[i] -= 1;
        }

        prev--;
        par = trees[par].par;
    }

    par = input;
    while(par != -1 && auth > 0){
        for(int i = 1; i <= auth; i++){
            trees[par].trans[i] += 1;
        }

        auth--;
        par = trees[par].par;
    }
}

void exchange_par(){ // 4. 부모 채팅방 교환
    // 채팅방 번호 2개 입력
    int input1, input2;
    cin >> input1 >> input2;

    // 1. 두 채팅방 trans -> tree 타고 올라 가며 빼주기 (알람 켜진 경우만)
    if(trees[input1].is_alarm) {
        int child = input1;
        int par = trees[input1].par;

        while(par != 0){
            for(int i = 1; i <= trees[par].depth; i++){
                trees[par].trans[i] -= trees[child].trans[i + 1];
            }

            par = trees[par].par;
        }
    }

    if(trees[input2].is_alarm) {
        int child = input2;
        int par = trees[input2].par;

        while(par != 0){
            for(int i = 1; i <= trees[par].depth; i++){
                trees[par].trans[i] -= trees[child].trans[i + 1];
            }

            par = trees[par].par;
        }
    }


    // 2. 두 채팅방 par 교환
    int tmp = trees[input1].par;
    trees[input1].par = trees[input2].par;
    trees[input2].par = tmp;

    // 3. 두 채팅방 trans -> tree 타고 올라 가며 더해주기

    if(trees[input1].is_alarm) {
        int child = input1;
        int par = trees[input1].par;

        while(par != 0){
            for(int i = 1; i <= trees[par].depth; i++){
                trees[par].trans[i] += trees[child].trans[i + 1];
            }

            par = trees[par].par;
        }
    }

    if(trees[input2].is_alarm) {
        int child = input2;
        int par = trees[input2].par;

        while(par != 0){
            for(int i = 1; i <= trees[par].depth; i++){
                trees[par].trans[i] += trees[child].trans[i + 1];
            }

            par = trees[par].par;
        }
    }


    // 4. 부모의 left, right 수정
    trees[trees[input2].par].left == input1 ? trees[trees[input2].par].left = input2
            : trees[trees[input2].par].right = input2;

    trees[trees[input1].par].left == input2 ? trees[trees[input1].par].left = input1
                                            : trees[trees[input1].par].right = input1;

}

void query(){ // 5. 채팅방 수 조회
    // 채팅방 번호 입력
    int input;
    cin >> input;

    // 해당 node trans[0] 출력
    int left, right;
    !trees[trees[input].left].is_alarm ? left = 0 : left = trees[trees[input].left].trans[1];
    !trees[trees[input].right].is_alarm? right = 0 : right = trees[trees[input].right].trans[1];

    cout << left + right << endl;
}


void run(){
    int cmd;
    cin >> N >> Q;

    while(Q--){
        cin >> cmd;

        switch (cmd) {
            case 100: // 1. 메신저 준비
                prepare();
                break;

            case 200: // 2. 알림망 설정 on/off
                set_alarm();
                break;

            case 300: // 3. 권한 세기 변경
                change_auth();
                break;

            case 400: // 4. 부모 채팅방 교환
                exchange_par();
                break;

            case 500: // 5. 채팅방 수 조회
                query();
                break;
        }
    }
}

int main() {
    init();
    run();

    return 0;
}