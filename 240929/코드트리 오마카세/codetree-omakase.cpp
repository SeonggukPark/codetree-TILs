#include <unordered_map>
#include <iostream>
#include <queue>
#define ll long long
#define pii pair<int, int>
#define pll pair<ll, ll>
#define endl '\n'
using namespace std;
constexpr size_t MAX_NAME = 15000;
int L, cur_idx, cur_sushi, cur_guest; // 초기화 필요
unordered_map<string, int> idx_alloc;
int left_sushi[MAX_NAME], guest_pos[MAX_NAME];
vector<int> wait_vec[MAX_NAME];

struct node{
    string name;
    int time;
};

struct cmp{
    bool operator()(const node & a, const node & b){
        if(a.time != b.time) return a.time > b.time;
        return a.name > b.name;
    }
};

priority_queue<node, vector<node>, cmp> pq;

int relate_pos(int t, int x){
    t %= L;
    if(t <= x) return x - t;
    x += L;
    return x - t;
}

void make_sushi(int t, int x, string & name){
    if(idx_alloc.find(name) == idx_alloc.end()) {
        idx_alloc[name] = cur_idx;
        wait_vec[cur_idx].clear();
        left_sushi[cur_idx++] = -1;
    }

    cur_sushi++;
    int idx = idx_alloc[name], pos = relate_pos(t, x);

    if(left_sushi[idx] < 0){ // 아직 손님 안온경우
        wait_vec[idx].push_back(pos);
        return;
    }

    // 온 경우 바로 pq로
    int time = (guest_pos[idx] >= x) ? t + guest_pos[idx] - x : t + guest_pos[idx] - x + L;
    pq.push({name, time});
}

void enter_guest(int t, int x, string & name, int n){
    if(idx_alloc.find(name) == idx_alloc.end()) {
        idx_alloc[name] = cur_idx++;
    }

    int idx = idx_alloc[name];
    guest_pos[idx] = x, left_sushi[idx] = n;
    cur_guest++;

    // wait_vec[idx] 순환하면서 pq 삽입
    for(auto i : wait_vec[idx]){
        int wait_pos = (i + t) % L, time = (x >= wait_pos) ? t + x - wait_pos : t + x - wait_pos + L;
        pq.push({name, time});
    }
}

void take_photo(int t){
    while(!pq.empty() && pq.top().time <= t){
        node top = pq.top();
        pq.pop();

        // pq top 이름 사람 초밥 -1
        // 해당 사람 초밥 0 되면 cur_guest--;
        int idx = idx_alloc[top.name];
        left_sushi[idx]--, cur_sushi--;
        if(left_sushi[idx] == 0) {
            cur_guest--;
            idx_alloc.erase(top.name);
        }
    }

    cout << cur_guest << ' ' << cur_sushi << endl;
}


void solve(){
    cur_idx = cur_guest = cur_sushi = 0;
    int Q, cmd, t, x, n;
    cin >> L >> Q;
    string name;
    while(Q--){
        cin >> cmd;
        switch (cmd) {
            case 100: // 초밥 만들기
                cin >> t >> x >> name;
                make_sushi(t, x, name);
                break;

            case 200: // 손님 입장
                cin >> t >> x >> name >> n;
                enter_guest(t, x, name, n);
                break;

            case 300: // 사진 촬영
                cin >> t;
                take_photo(t);
                break;

            default: // 에러
                cout << "error occured.. " << endl;
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