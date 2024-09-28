#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

// 변수 선언
int L, Q;

class Query {
    public:
        int cmd, t, x, n;
        string name;

        Query(int cmd, int t, int x, string name, int n) {
            this->cmd = cmd;
            this->t = t;
            this->x = x;
            this->name = name;
            this->n = n;
        }
};

// 명령들을 관리합니다.
vector<Query> queries;

// 등장한 사람 목록을 관리합니다.
set<string> names;

// 각 사람마다 주어진 초밥 명령만을 관리합니다.
map<string, vector<Query> > p_queries;

// 각 사람마다 입장 시간을 관리합니다.
map<string, int> entry_time;

// 각 손님의 위치를 관리합니다.
map<string, int> position;

// 각 사람마다 퇴장 시간을 관리합니다.
map<string, int> exit_time;

// Query를 (t, cmd) 순으로 정렬합니다.
bool cmp(Query q1, Query q2) {
    if(q1.t != q2.t)
        return q1.t < q2.t;
    return q1.cmd < q2.cmd;
}

int main() {
    // 입력:
    cin >> L >> Q;
    for(int i = 0; i < Q; i++) {
        int cmd = -1;
        int t = -1, x = -1, n = -1;
        string name;
        cin >> cmd;
        if(cmd == 100)
            cin >> t >> x >> name;
        else if(cmd == 200)
            cin >> t >> x >> name >> n;
        else cin >> t;

        queries.push_back(Query(cmd, t, x, name, n));
        
        // 사람별 주어진 초밥 목록을 관리합니다.
        if(cmd == 100)
            p_queries[name].push_back(Query(cmd, t, x, name, n));
        // 손님이 입장한 시간과 위치를 관리합니다.
        else if(cmd == 200) {
            names.insert(name);
            entry_time[name] = t;
            position[name] = x;
        }
    }

    // 각 사람마다 자신의 이름이 적힌 조합을 언제 먹게 되는지를 계산하여 해당 정보를 기존 Query에 추가합니다. (111번 쿼리)
    for(string name : names) {
        // 해당 사람의 퇴장 시간을 관리합니다.
        // 이는 마지막으로 먹는 초밥 시간 중 가장 늦은 시간이 됩니다.
        exit_time[name] = 0;

        for(Query q: p_queries[name]) {
            // 만약 초밥이 사람이 등장하기 전에 미리 주어진 상황이라면
            int time_to_removed = 0;
            if(q.t < entry_time[name]) {
                // entry_time때의 스시 위치를 구합니다.
                int t_sushi_x = (q.x + (entry_time[name] - q.t)) % L;
                // 몇 초가 더 지나야 만나는지를 계산합니다.
                int additionl_time = (position[name] - t_sushi_x + L) % L;

                time_to_removed = entry_time[name] + additionl_time;
            }
            // 초밥이 사람이 등장한 이후에 주어졌다면
            else {
                // 몇 초가 더 지나야 만나는지를 계산합니다.
                int additionl_time = (position[name] - q.x + L) % L;
                time_to_removed = q.t + additionl_time;
            }

            // 초밥이 사라지는 시간 중 가장 늦은 시간을 업데이트 합니다.
            exit_time[name] = max(exit_time[name], time_to_removed);

            // 초밥이 사라지는 111번 쿼리를 추가합니다.
            queries.push_back(Query(111, time_to_removed, -1, name, -1));
        }
    }

    // 사람마다 초밥을 마지막으로 먹은 시간 t를 계산하여 그 사람이 해당 t 때 코드트리 오마카세를 떠났다는 Query를 추가합니다. (222번 쿼리)
    for(string name : names)
        queries.push_back(Query(222, exit_time[name], -1, name, -1));

    // 전체 Query를 시간순으로 정렬하되 t가 일치한다면 문제 조건상 사진 촬영에 해당하는 300이 가장 늦게 나오도록 cmd 순으로 오름차순 정렬을 합니다.
    // 이후 순서대로 보면서 사람, 초밥 수를 count하다가 300이 나오면 현재 사람, 초밥 수를 출력합니다.
    sort(queries.begin(), queries.end(), cmp);
    
    int people_num = 0, sushi_num = 0;
    for(int i = 0; i < (int) queries.size(); i++) {
        if(queries[i].cmd == 100) // 초밥 추가
            sushi_num++;
        else if(queries[i].cmd == 111) // 초밥 제거
            sushi_num--;
        else if(queries[i].cmd == 200) // 사람 추가
            people_num++;
        else if(queries[i].cmd == 222) // 사람 제거
            people_num--;
        else // 사진 촬영시 답을 출력하면 됩니다.
            cout << people_num << " " << sushi_num << "\n";
    }
    return 0;
}