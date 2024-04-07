#include <iostream>
#include <cstring>
#include <vector>
#include <set>
#include <climits>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;
constexpr size_t MAX_N = 20;
int n, rst;
int arr[MAX_N][MAX_N];
vector<int> v;
set<int> s;

void init(){
    memset(arr, 0, sizeof(arr));
    rst = INT_MAX;
    v.push_back(0);
    s.insert(0);
}

void input(){
    cin >> n;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> arr[i][j];
        }
    }
}

int intensity(const vector<int> & data){
    int res = 0;
    for (int i = 0; i < n / 2; ++i) {
        for (int j = i + 1; j < n / 2; ++j) {
            res += arr[data[i]][data[j]];
            res += arr[data[j]][data[i]];
        }
    }

    return res;
}

void dfs(int idx, int depth){
    if(depth == n / 2){
        vector<int> tmp;
        for(int i = 0; i < n; i++){
            if(s.find(i) == s.end()) tmp.push_back(i);
        }

        rst = min(rst, abs(intensity(v) - intensity(tmp)));
    }

    for (int i = idx + 1; i < n; ++i) {
        s.insert(i);
        v.push_back(i);
        dfs(i, depth + 1);
        s.erase(i);
        v.pop_back();
    }
}

void run(){
    dfs(0, 1);
    cout << rst << endl;
}


int main() {
    fastio
    init();
    input();
    run();
    return 0;
}