#include <iostream>
#include <cstring>

using namespace std;
constexpr size_t MAX_N = 100001;
int q, n, m;

struct Node{
    int prev, next;
};

struct Belt{
    int h, t, cnt;
};

Belt belt[MAX_N];
Node node_pool[MAX_N];

void traverse_node(){
    for(int i = 1; i <= m; i++) {
        cout << "Node " << i << ": " << "prev, next: " << node_pool[i].prev << ' ' << node_pool[i].next << endl;
    }
    cout << endl;
}

void traverse_belt(){
    for(int i = 1; i <= n; i++) {
        cout << "Belt " << i << ": " << "head, tail, cnt: " << belt[i].h << ' ' << belt[i].t << ' ' << belt[i].cnt << endl;
    }
    cout << endl;
}

void Build_Factory(){
    for(int i = 0; i < MAX_N; i++){
        belt[i].h = belt[i].t = belt[i].cnt = 0;
        node_pool[i].prev = node_pool[i].next = 0;
    }

    int prev[MAX_N];
    memset(prev, 0, sizeof(prev));

    int idx;
    cin >> n >> m;

    for(int i = 1; i <= m; i++){
        cin >> idx;
        belt[idx].t = i;
        belt[idx].cnt++;

        if(prev[idx] == 0) {
            prev[idx] = i;
            belt[idx].h = i;
            node_pool[i].prev = 0;
            continue;
        }

        node_pool[prev[idx]].next = i;
        node_pool[i].prev = prev[idx];
        prev[idx] = i;
    }
}

void Move_All(){
    int src, dst;
    cin >> src >> dst;

    if(belt[src].cnt == 0){
        cout << belt[dst].cnt << endl;
        return;
    }

    node_pool[belt[dst].h].prev = belt[src].t;
    node_pool[belt[src].t].next = belt[dst].h;

    belt[dst].h = belt[src].h;
    if(belt[dst].t == 0) belt[dst].t = belt[src].t;
    
    belt[dst].cnt += belt[src].cnt;
    belt[src].t = belt[src].h = belt[src].cnt = 0;



    cout << belt[dst].cnt << endl;
}

void Replace_Front(){
    int src, dst;
    cin >> src >> dst;

    int src_h = belt[src].h, dst_h = belt[dst].h;
    int src_n = node_pool[src_h].next, dst_n = node_pool[dst_h].next;

    if(src_h == 0 && dst_h == 0) {
        cout << belt[dst].cnt << endl;
        return;
    }

    if(src_h == 0){
        belt[src].h = belt[src].t = dst_h;
        belt[dst].h = node_pool[dst_h].next;

        node_pool[dst_h].prev = node_pool[dst_h].next = 0;
        node_pool[belt[dst].h].prev = 0;

        belt[src].cnt = 1;
        belt[dst].cnt--;

        cout << belt[dst].cnt << endl;
        return;
    }

    if(dst_h == 0){
        belt[dst].h = belt[dst].t = src_h;
        belt[src].h = node_pool[src_h].next;

        node_pool[src_h].prev = node_pool[src_h].next = 0;
        node_pool[belt[src].h].prev = 0;

        belt[dst].cnt = 1;
        belt[src].cnt--;

        cout << belt[dst].cnt << endl;
        return;
    }

    // 둘 다 하나씩은 있는 경우
    belt[src].h = dst_h;
    belt[dst].h = src_h;

    if(belt[src].cnt == 1) belt[src].t = dst_h;
    if(belt[dst].cnt == 1) belt[dst].t = src_h;

    node_pool[src_h].next = dst_n;
    node_pool[dst_h].next = src_n;
    node_pool[src_n].prev = dst_h;
    node_pool[dst_n].prev = src_h;

    cout << belt[dst].cnt << endl;
}

void Divide_Product(){
    int src, dst;
    cin >> src >> dst;

    int idx = belt[src].h;
    int num = belt[src].cnt / 2;

    if(num == 0) {
        cout << belt[dst].cnt << endl;
        return;
    }

    for(int i = 1; i < num; i++){
        idx = node_pool[idx].next;
    }

    int src_h = belt[src].h, src_t = idx, src_tn = node_pool[idx].next, dst_h = belt[dst].h;

    belt[src].h = src_tn;
    belt[dst].h = src_h;

    // dst 빈 경우 처리
    if(belt[dst].t == 0) belt[dst].t = src_t;

    node_pool[src_t].next = dst_h;
    node_pool[src_tn].prev = 0;
    node_pool[dst_h].prev = src_t;

    belt[src].cnt -= num;
    belt[dst].cnt += num;

    cout << belt[dst].cnt << endl;
}

void Info_Present(){
    int p_num, a, b;
    cin >> p_num;

    a = (node_pool[p_num].prev == 0) ? -1 : node_pool[p_num].prev;
    b = (node_pool[p_num].next == 0) ? -1 : node_pool[p_num].next;

    cout << a + 2 * b << endl;
}

void Info_Belt(){
    int b_num, a, b, c;
    cin >> b_num;

    c = belt[b_num].cnt;

    if(c == 0){
        cout << -3 << endl;
        return;
    }

    a = belt[b_num].h, b = belt[b_num].t;
    cout << a + (2 * b) + (3 * c) << endl;
}

void run(){
    int cmd;
    cin >> q;

    while(q--){
        cin >> cmd;

        // cout << "cmd: " << cmd << endl;
        switch (cmd) {
            case 100:
                Build_Factory();
                break;

            case 200:
                Move_All();
                break;

            case 300:
                Replace_Front();
                break;

            case 400:
                Divide_Product();
                break;

            case 500:
                Info_Present();
                break;

            case 600:
                Info_Belt();
                break;
        }

        // traverse_node();
        // traverse_belt();
    }
}

int main() {
    // freopen("input.txt", "r", stdin);
    run();
    return 0;
}