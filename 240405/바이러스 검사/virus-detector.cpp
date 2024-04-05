#include <iostream>
#include <cstring>
#define fastio ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
using namespace std;

constexpr size_t MAX_N = 1000000;
int arr[MAX_N];

int main() {
    fastio
    int n, pro, adv;
    long long rst;
    memset(arr, 0, sizeof(arr));
    cin >> n;
    rst = n;

    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }
   
    cin >> pro >> adv;

    for (int i = 0; i < n; ++i) {
        arr[i] -= pro;
        if(arr[i] <= 0) continue;
        
        arr[i] % adv == 0 ? rst += (arr[i] / adv) : rst += ((arr[i] / adv) + 1);
    }

    cout << rst;
    return 0;
}