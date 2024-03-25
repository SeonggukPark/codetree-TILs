#include <bits/stdc++.h>
#define endl '\n'
#define fastio ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;
constexpr size_t MAX_N = 15001;

struct CMD {
    int cmd, x, t, n;
    string name;
};

struct custom{
    int time, pos, num;
};

unordered_map<string, custom> um; // index maker
unordered_set<string> left_cus;
vector< CMD > cmds, v;
int cnt_per, cnt_susi;
int L, Q;

bool mcmp(const CMD& left, const CMD& right){
    if(left.t != right.t) return left.t < right.t;
    return left.cmd < right.cmd;
}

void init(){
    um.clear();
    left_cus.clear();
    cmds.clear();
    v.clear();
    cnt_per = cnt_susi = 0;
}

void input(){
    int cmd, x, t, n;
    string name;
    CMD tmp;

    cin >> L >> Q;
    while(Q--){
        cin >> cmd;

        switch (cmd) {
            case 100:
                cin >> t >> x >> name;
                break;

            case 200:
                cin >> t >> x >> name >> n;
                um[name].time = t;
                um[name].pos = x;
                um[name].num = n;
                left_cus.insert(name);
                break;

            case 300:
                cin >> t;
                break;
        }

        tmp.cmd = cmd;
        tmp.t = t;
        tmp.x = x;
        tmp.name = name;
        tmp.n = n;
        cmds.push_back(tmp);
    }
}

void run(){ // 초밥 만들기
    for (int i = 0; i < cmds.size(); ++i) {
        if(cmds[i].cmd != 100) continue;
        CMD susi = cmds[i];

        int meet = 0, eat = 0;
        string name = susi.name;
        int cur_susi = susi.x, time_diff = um[name].time - susi.t;

        if(susi.t < um[name].time){
            cur_susi = ((cur_susi + time_diff) % L);
            um[name].pos >= cur_susi ? meet = um[name].pos - cur_susi : meet = um[name].pos + L - cur_susi;
            eat = um[name].time + meet;
        }

        else{
            um[name].pos >= cur_susi ? meet = um[name].pos - cur_susi : meet = um[name].pos + L - cur_susi;
            eat = susi.t + meet;
        }

        CMD tmp;
        tmp.cmd = susi.cmd + 1;
        tmp.t = eat;
        tmp.x = -1;
        tmp.name = susi.name;
        tmp.n = -1;
        v.push_back(tmp);
    }

    cmds.insert(cmds.end(), v.begin(), v.end());
    sort(cmds.begin(), cmds.end(), mcmp);

    for (int i = 0; i < cmds.size(); ++i) {
        if(cmds[i].cmd == 100) ++cnt_susi;

        else if(cmds[i].cmd == 101){
            --cnt_susi;
            string new_name = cmds[i].name;
            um[new_name].num -= 1;
            if(um[new_name].num <= 0) --cnt_per;
        }

        else if(cmds[i].cmd == 200){
            ++cnt_per;
        }

        else if(cmds[i].cmd == 300){
            cout << cnt_per << ' '<< cnt_susi << endl;
        }
    }
}


int main(){
    fastio;
    freopen("input.txt", "r", stdin);
    init();
    input();
    run();

    return 0;
}