#include <iostream>
#include <cstring>
#include <queue>
#define endl '\n'
using namespace std;
constexpr size_t MAX_LAND = 2000, MAX_ID = 30001;
constexpr int INF = 987654321;
int n, m, depart, graph[MAX_LAND][MAX_LAND];
bool erased_id[MAX_ID];

struct Product{
    int id, rev, dest, cost;
};

struct cmp{
    bool operator()(const Product & a, const Product & b){
        if(a.rev - a.cost != b.rev - b.cost) return a.rev - a.cost < b.rev - b.cost;
        return a.id > b.id;
    }
};

vector<Product> vec;
priority_queue<Product, vector<Product>, cmp> pq;

void init(){
}

void build_land(){ // cmd: 100
    memset(erased_id, true, sizeof(erased_id));
    depart = 0;
    cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            graph[i][j] = INF;
            if(i == j) graph[i][j] = 0;
        }
    }

    int v, u, w;
    for (int i = 0; i < m; ++i) {
        cin >> v >> u >> w;
        graph[v][u] = graph[u][v] = min(graph[v][u], w);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                graph[j][k] = min(graph[j][k], graph[j][i] + graph[i][k]);
            }
        }
    }
}

void make_product(int mid, int rev, int dest){ // cmd: 200
    erased_id[mid] = false;
    vec.push_back({mid, rev, dest, -1});
    if(graph[depart][dest] == INF || graph[depart][dest] > rev) return;
    // cout << mid << ' ' << rev << ' ' << dest << ' ' << graph[depart][dest] << endl;
    pq.push({mid, rev, dest, graph[depart][dest]});
}

void cancle_product(int mid){ // cmd:300
    erased_id[mid] = true;
}

void sell_product(){ // cmd: 400
    while(!pq.empty()){
        auto top = pq.top();
        pq.pop();
        if(erased_id[top.id]) continue;
        erased_id[top.id] = true;
        cout << top.id << endl;
        return;
    }

    cout << -1 << endl;
}

void change_depart(int next_depart){ // cmd: 500
    depart = next_depart;
    pq = {};

    for(auto i : vec){
        if(erased_id[i.id]) continue;
        if(graph[depart][i.dest] == INF || graph[depart][i.dest] > i.rev) continue;
        pq.push({i.id, i.rev, i.dest, graph[depart][i.dest]});
    }
}

void solve(){
    int id, rev, dest, cmd, Q; cin >> Q;

    while(Q--){
        cin >> cmd;
        switch (cmd) {
            case 100:
                build_land();
                break;

            case 200:
                cin >> id >> rev >> dest;
                make_product(id, rev, dest);
                break;

            case 300:
                cin >> id;
                cancle_product(id);
                break;

            case 400:
                sell_product();
                break;

            case 500:
                cin >> id;
                change_depart(id);
                break;

            default:
                cout << "exception error.." << endl;
                break;
        }
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}