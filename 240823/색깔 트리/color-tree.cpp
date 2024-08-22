#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
constexpr size_t MAX_NODE = 100001;

struct Node{
    int pid, color, max_dep, sub_time;
    set<int> kids_color;
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
    node_pool[mid].kids_color.clear();

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
        node_pool[i].kids_color.clear();
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
                node_pool[par].kids_color.insert(node_color);
                par = node_pool[par].pid;
            }
        }

        else{
            node_pool[i].kids_color.insert(changed_color);
        }
    }

    for(int i : exist_id){
        acc += (int)node_pool[i].kids_color.size() * (int)node_pool[i].kids_color.size();
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