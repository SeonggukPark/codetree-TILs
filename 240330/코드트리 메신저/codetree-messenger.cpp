#include <iostream>
#define endl '\n'
#define fastio ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_N = 100001, MAX_D = 22;

int n, q;
int auth[MAX_N], par[MAX_N], val[MAX_N];
bool alarm[MAX_N];
int nx[MAX_N][MAX_D];

void init() {
    for (int i = 1; i <= n; i++) {
        cin >> par[i];
    }
    for (int i = 1; i <= n; i++) {
        cin >> auth[i];
        if (auth[i] > 20) auth[i] = 20;
    }

    // nx 배열과 val 값을 초기화합니다.
    for (int i = 1; i <= n; i++) {
        int cur = i;
        int x = auth[i];
        nx[cur][x]++;

        while (par[cur] && x) {
            cur = par[cur];
            x--;
            if (x) nx[cur][x]++;
            val[cur]++;
        }
    }

}

void toggle_alarm(int chat) {
    if(alarm[chat]) {
        int cur = par[chat];
        int num = 1;
        // 상위 채팅으로 이동하며 noti 값에 따라 nx와 val 값을 갱신합니다.
        while(cur) {
            for(int i = num; i <= 21; i++) {
                val[cur] += nx[chat][i];
                if(i > num) nx[cur][i - num] += nx[chat][i];
            }
            if(alarm[cur]) break;
            cur = par[cur];
            num++;
        }
        alarm[chat] = false;
    } else {
        int cur = par[chat];
        int num = 1;
        // 상위 채팅으로 이동하며 noti 값에 따라 nx와 val 값을 갱신합니다.
        while(cur) {
            for(int i = num; i <= 21; i++) {
                val[cur] -= nx[chat][i];
                if(i > num) nx[cur][i - num] -= nx[chat][i];
            }
            if(alarm[cur]) break;
            cur = par[cur];
            num++;
        }
        alarm[chat] = true;
    }
}

void change_auth(int chat, int power){
    int bef_power = auth[chat];
    power = min(power, 20);  // 권한의 크기를 20으로 제한합니다.
    auth[chat] = power;

    nx[chat][bef_power]--;
    if(!alarm[chat]) {
        int cur = par[chat];
        int num = 1;
        // 상위 채팅으로 이동하며 nx와 val 값을 갱신합니다.
        while(cur) {
            if(bef_power >= num) val[cur]--;
            if(bef_power > num) nx[cur][bef_power - num]--;
            if(alarm[cur]) break;
            cur = par[cur];
            num++;
        }
    }

    nx[chat][power]++;
    if(!alarm[chat]) {
        int cur = par[chat];
        int num = 1;
        // 상위 채팅으로 이동하며 nx와 val 값을 갱신합니다.
        while(cur) {
            if(power >= num) val[cur]++;
            if(power > num) nx[cur][power - num]++;
            if(alarm[cur]) break;
            cur = par[cur];
            num++;
        }
    }
}

void change_parent(int chat1, int chat2) {
    bool bef_noti1 = alarm[chat1];
    bool bef_noti2 = alarm[chat2];

    if(!alarm[chat1]) toggle_alarm(chat1);
    if(!alarm[chat2]) toggle_alarm(chat2);

    swap(par[chat1], par[chat2]);

    if(!bef_noti1) toggle_alarm(chat1);
    if(!bef_noti2) toggle_alarm(chat2);
}

void serach_chat(int chat) {
    cout << val[chat] << "\n";
}


int main() {
    cin >> n >> q;
    while(q--) {
        int query;
        cin >> query;
        if(query == 100) {
            init();
        }

        else if(query == 200) {
            int chat;
            cin >> chat;
            toggle_alarm(chat);
        }


        else if(query == 300) {
            int chat, power;
            cin >> chat >> power;
            change_auth(chat, power);
        }

        else if(query == 400) {
            int chat1, chat2;
            cin >> chat1 >> chat2;
            change_parent(chat1, chat2);
        }


        else if(query == 500) {
            int chat;
            cin >> chat;
            serach_chat(chat);
        }

    }

    return 0;
}