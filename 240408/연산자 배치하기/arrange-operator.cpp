#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;
int n, mini, maxi;
vector<int> nums, ops;

void init(){
    nums.clear();
    ops.clear();
    mini = INT_MAX, maxi = INT_MIN;
}

void input(){
    int tmp;
    cin >> n;

    for(int i = 0; i < n; i++){
        cin >> tmp;
        nums.push_back(tmp);
    }

    for(int i = 0; i < 3; i++){ // 0, 1, 2: +, -, *
        cin >> tmp;
        for (int j = 0; j < tmp; j++){
            ops.push_back(i);
        }
    }
}

void run() {
    do {
        int cnt = nums.front();
        auto iter = nums.begin() += 1;

        for (auto op: ops) {
            switch (op) {
                case 0: // +
                    cnt += *iter;
                    break;

                case 1: // -
                    cnt -= *iter;

                    break;

                case 2: // *
                    cnt *= *iter;
                    break;
            }

            iter++;
        }

        if (cnt > maxi) maxi = cnt;
        if (cnt < mini) mini = cnt;

    } while (next_permutation(ops.begin(), ops.end()));
}


int main() {
    init();
    input();
    run();
    cout << mini << ' ' << maxi;
    return 0;
}