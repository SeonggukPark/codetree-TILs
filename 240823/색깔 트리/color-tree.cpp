#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
constexpr size_t MAX_NODE = 100001, MAX_COLOR = 5;

struct Node{
    int pid, color, max_dep, sub_time;
    bool kids_color[MAX_COLOR];
};

Node node_pool[MAX_NODE];
int cur_time;
vector<int> exist_id;

void init(){
    cur_time = 0;
    exist_id.clear();
}

void add_node(int mid, int pid, int color, int max_dep){
    cur_time++;
    int par = pid, dep_cnt = 2;

    while(par != -1){
        if (node_pool[par].max_dep < dep_cnt) return;
        dep_cnt++;
        par = node_pool[par].pid;
    }

    node_pool[mid].pid = pid;
    node_pool[mid].color = color;
    node_pool[mid].max_dep = max_dep;
    node_pool[mid].sub_time = cur_time;
    memset(node_pool[mid].kids_color, false, sizeof(node_pool[mid].kids_color));

    exist_id.push_back(mid);
}

void change_color(int mid, int color){
    cur_time++;
    node_pool[mid].sub_time = cur_time;
    node_pool[mid].color = color;
}

void check_color(int mid){
    cur_time++;
    int cur_color = node_pool[mid].color, latest_time = node_pool[mid].sub_time, par = node_pool[mid].pid;

    while(par != -1) {
        if(node_pool[par].sub_time > latest_time){
            cur_color = node_pool[par].color;
            latest_time = node_pool[par].sub_time;
        }
        par = node_pool[par].pid;
    }

    cout << cur_color << endl;
}

void check_score(){
    cur_time++;
    int acc = 0;

    for(int i : exist_id){
        memset(node_pool[i].kids_color, false, sizeof(node_pool[i].kids_color));
    }

    for(int i : exist_id){
        int node_time = node_pool[i].sub_time, node_color = node_pool[i].color, par = node_pool[i].pid;
        int changed_color, changed_time = node_time;
        bool upper_possible = true;
        while(par != -1){
            if(node_pool[par].sub_time > node_time) {
                upper_possible = false;
                if(node_pool[par].sub_time > changed_time){
                    changed_color = node_pool[par].color;
                    changed_time = node_pool[par].sub_time;
                }
            }
            par = node_pool[par].pid;
        }

        par = i;
        if(upper_possible){
            while(par != -1){
                node_pool[par].kids_color[node_color - 1] = true;
                par = node_pool[par].pid;
            }
        }

        else{
            node_pool[i].kids_color[changed_color - 1] = true;
        }
    }

    for(int i : exist_id){
        int tmp_cnt = 0;
        for(bool k : node_pool[i].kids_color){
            if(k) tmp_cnt++;
        }
        acc += tmp_cnt * tmp_cnt;
    }

    cout << acc << endl;
}

void solve(){
    int Q, cmd, a, b, c, d;
    cin >> Q;
    init();

    for (int i = 0; i < Q; ++i) {
        cin >> cmd;
        switch (cmd) {
            case 100:
                cin >> a >> b >> c >> d;
                add_node(a, b, c, d);
                break;

            case 200:
                cin >> a >> b;
                change_color(a, b);
                break;

            case 300:
                cin >> a;
                check_color(a);
                break;

            case 400:
                check_score();
                break;

            default: // should not be accessed.
                cout << "error occured.. " << endl;
                break;
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}