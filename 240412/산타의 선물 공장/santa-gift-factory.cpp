#include <iostream>
#include <cstring>
#include <unordered_map>
using namespace std;
int q, n, m, p_idx;
constexpr size_t MAX_N = 100001;
unordered_map<int, int> um; // id <-> idx 매핑 용

struct Product{
    int id, weight, belt_num;
    int prev, next; // products idx로 저장
};

struct Belt{
    int head, tail, cnt;
    bool alive;
};

Product products[MAX_N];
Belt belt[MAX_N];

void init(){

}

void traverse_product(){
    for (int i = 1; i <= n; ++i) {
        cout << "Product " << i << ": " << "id, weight, belt_num, prev, next: "  << products[i].id <<
        ' ' <<products[i].weight << ' ' <<products[i].belt_num << ' ' << products[i].prev << ' ' << products[i].next << endl;
    }
}

void traverse_belt(){
    for (int i = 1; i <= m; ++i) {
        cout << "belt " << i << ": " << "head, tail, cnt: "  << products[belt[i].head].id << ' ' <<products[belt[i].tail].id << ' ' <<belt[i].cnt << endl;
    }
}

void Build_Factory() { // 공장 설립 - 100
    cin >> n >> m;
    int data, prev = 0;

    for (int i = 1; i <= n; ++i) {
        cin >> data;
        products[i].id = data;
        um[data] = i;
    }

    for (int i = 1; i <= n; ++i) {
        cin >> products[i].weight;

        if(i % 4 != 1){ // 맨 앞 상자가 아니라면
            products[i].prev = prev;
            products[prev].next = i;
        }

        products[i].belt_num = (i - 1) / 4 + 1;
        prev = i;
    }

    for (int i = 1; i <= m; ++i) {
        belt[i].cnt = n / m;
        belt[i].alive = true;
        belt[i].head = (n / m) * (i - 1) + 1;
        belt[i].tail = belt[i].head + (n / m - 1);
    }
}

void Down_Product() { // 물건 하차 - 200
    int w_max, sum = 0;
    cin >> w_max;

    // 1 ~ m 번 벨트 순환
    for(int i = 1; i <= m; i++){
        // 만약 빈 벨트면 skip
        if(belt[i].cnt == 0) continue;

        // 맨 앞 상자 무게 w_max 이하면 하차, sum 누적
        int front = belt[i].head, next = products[front].next;

        if(products[front].weight <= w_max){
            // 상품 정보 수정
            products[front].belt_num = 0;
            products[next].prev = 0;
            products[front].next = 0;

            // 벨트 정보 수정
            belt[i].cnt--;
            belt[i].head = next;

            if(belt[i].cnt == 0){
                belt[i].head = belt[i].tail = 0;
            }

            sum += products[front].weight;
        }

        // w_max 초과면 해당 벨트 맨 뒤로
        else{
            // 1. 벨트에 상자 하나 이상일 경우
            if(belt[i].cnt > 1){
                // 상품 정보 수정
                products[belt[i].tail].next = front;
                products[front].prev = belt[i].tail;
                products[front].next = 0;
                products[next].prev = 0;

                // 벨트 정보 수정
                belt[i].head = next;
                belt[i].tail = front;
            }
        }
    }


    // sum 출력
    cout << sum << endl;
}

void Remove_Product() { // 물건 제거 - 300
    int r_id;
    cin >> r_id;

    // products idx : um[r_id]
    int idx = um[r_id];

    // 해당 product가 없다면 (belt_num == 0) -1 출력 후 return
    if(products[idx].belt_num == 0){
        cout << -1 << endl;
        return;
    }

    // 벨트에서 해당 상자 제거
    int b_idx = products[idx].belt_num;
    belt[b_idx].cnt--;
    products[idx].next = products[idx].prev = 0;

    // (1) 맨 앞일 경우
    if(belt[b_idx].head == idx){
        int next = products[idx].next;
        products[idx].belt_num = 0;
        products[next].prev = 0;

        belt[b_idx].head = next;
    }

    // (2) 맨 뒤 일 경우
    else if(belt[b_idx].tail == idx){
        int prev = products[idx].prev;
        products[prev].next = 0;

        belt[b_idx].tail = prev;
    }

    // (3) 중간 일 경우
    else{
        int prev = products[idx].prev, next = products[idx].next;
        products[prev].next = products[next].prev = 0;
    }

    if(belt[b_idx].cnt == 1){
        belt[b_idx].tail = belt[b_idx].head;
    }

    if(belt[b_idx].cnt == 0){
        belt[b_idx].tail = belt[b_idx].head = 0;
    }

    cout << r_id << endl;
}

void Check_Product() { // 물건 확인 - 400
    int f_id;
    cin >> f_id;

    // products idx : um[f_id]
    int idx = um[f_id], b_idx = products[idx].belt_num;

    // 해당 product가 없다면 (belt_num == 0) -1 출력 후 return
    if(b_idx == 0){
        cout << -1 << endl;
        return;
    }

    // belt_num 출력
    cout << b_idx;

    // f_id 이상의 상자들 다 맨앞으로


    belt[b_idx].head = idx;
    belt[b_idx].tail = products[idx].prev;
}

void Broken_Belt() { // 벨트 고장 - 500
    int b_num;
    cin >> b_num;

    // 이미 b_num 고장이면 -1 출력 후 return
    if(!belt[b_num].alive){
        cout << -1 << endl;
        return;
    }

    // belt 폐쇄 (다시 사용 X)
    belt[b_num].alive = false;

    int b_idx = b_num;
    // b_num 벨트 오른쪽으로 순환하면서 살아있는 최초 벨트 찾기
    while(true){
        b_idx = b_idx % m + 1;
        if(belt[b_idx].alive) break;
    }

    // b_num 벨트의 상자 중 맨 앞 부터 해서 새로운 벨트 맨 뒤에서 부터 적재


    // b_idx가 빈 경우
    if(belt[b_idx].cnt == 0){
        belt[b_idx].head = belt[b_num].head;
        belt[b_idx].tail = belt[b_num].tail;
        belt[b_idx].cnt = belt[b_num].cnt;
    }

    // 하나 이상 일 경우
    else{

    }

    belt[b_num].head = belt[b_num].tail = belt[b_num].cnt = 0;

    // b_num 출력 후 리턴
    cout << b_num;
}

void run(){
    int cmd;
    cin >> q;

    while(q--){
        cin >> cmd;

        switch (cmd) {
            case 100: // 공장 설립
                Build_Factory();
                break;

            case 200: // 물건 하차
                Down_Product();
                break;

            case 300: // 물건 제거
                Remove_Product();
                break;

            case 400: // 물건 확인
                Check_Product();
                break;

            case 500: // 벨트 고장
                Broken_Belt();
                break;
        }

        cout << "cmd: " << cmd << endl;
        traverse_belt();
        traverse_product();
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    init();
    run();
    return 0;
}