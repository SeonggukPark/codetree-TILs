#include <iostream>
#include <queue>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <vector>

#define endl '\n'
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_N = 50001;
int N;
unordered_set<string> url_set;
unordered_set<string> domain_set;
unordered_map<int, string> um;
unordered_map<string, pair<int, int>> domain_time;

set<int> judge;

struct task{
    int t, p;
    string url;
};

struct cmp{
    bool operator()(const task & a, const task & b){
        if(a.p != b.p) return a.p > b.p;
        return a.t > b.t;
    }
};

priority_queue<task, vector<task>, cmp> pq;

void init(){
    pq = {};
    url_set.clear();
    judge.clear();
    domain_set.clear();
}


// 1. 채점기 준비
void prepare(){
    string u;
    cin >> N >> u;

    // 초기 문제 채점 대기 큐에 삽입
    task tmp;
    tmp.p = 1, tmp.t = 0;
    tmp.url = u;

    pq.push(tmp);
    url_set.insert(u);

    // 채점기 생성
    for(int i = 1; i <= N; i++){
        judge.insert(i);
    }
}

// 2. 채점 요청
void request(){
    int t, p;
    string u;

    cin >> t >> p >> u;

    // if set(url_set)에 u 존재하지 않으면, (t, p, u) ~ task pq에 삽입
    if(url_set.find(u) == url_set.end()){
        task tmp;
        tmp.t = t, tmp.p = p;
        tmp.url = u;

        pq.push(tmp);
        url_set.insert(u);
    }
}

// 3. 채점 시도
void grading(){
    int t;
    cin >> t;
    bool found = false;

    // if 쉬고 있는 채점기 없으면 -> continue;
    if(judge.empty()) return;

    string url, domain;
    // pq search
    vector<task> tmp_v;

    while(!pq.empty()){
        url = pq.top().url;
        tmp_v.push_back(pq.top());
        pq.pop();

        domain = url.substr(0, url.find('/'));

        // if pq.top()의 도메인이 현재 채점 중 -> continue;
        if(domain_set.find(domain) != domain_set.end()) continue;

        // if pq.top()의 도메인의 가장 최근 채점 gap (end - start 시간)에서, 현재 t < start + 3 * gap -> continue;
        if(domain_time.find(domain) != domain_time.end()){
            int start = domain_time[domain].first, end = domain_time[domain].second;
            if(t < start + 3 * (end - start)) continue;
        }

        found = true;
        tmp_v.pop_back();
        break;
    }


    while(!tmp_v.empty()){
        pq.push(tmp_v.back());
        tmp_v.pop_back();
    }

    if(!found) return;

    // 셋 다 아니면 해당 pq.top() -> 쉬는 채점기 중 가장 작은 채점기 작동 시작
    url_set.erase(url);
    um[*judge.begin()] = domain;
    domain_time[domain] = {t, -1};
    judge.erase(judge.begin());
    domain_set.insert(domain);
}

// 4. 채점 종료
void finish(){
    int t, j_id;
    cin >> t >> j_id;

    // if j_id working -> 쉬는 상태
    if(judge.find(j_id) == judge.end()) {
        domain_time[um[j_id]].second = t;
        domain_set.erase(um[j_id]);
        judge.insert(j_id);
    }
}

// 5. 채점 대기 큐 조회
void inquiry(){
    int t;
    cin >> t;

    // pq size 출력
    cout << pq.size() << endl;
}

void run(){
    int Q, cmd;
    cin >> Q;

    while(Q--){
        cin >> cmd;

        switch(cmd) {
            case 100:
                // 1. 채점기 준비
                prepare();
                break;

            case 200:
                // 2. 채점 요청
                request();
                break;

            case 300:
                // 3. 채점 시도
                grading();
                break;

            case 400:
                // 4. 채점 종료
                finish();
                break;

            case 500:
                // 5. 채점 대기 큐 조회
                inquiry();
                break;
        }
    }

}


int main() {
    fastio

    init();
    run();
    return 0;
}