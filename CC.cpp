#include <iostream>
#include <vector>
#include <climits>  // for INT_MAX
#include <iomanip>  // for setw

using namespace std;

int DPCoinChange(vector<int>& coins, int n) {
    // C[j]는 금액 j를 만들기 위한 최소 동전 개수
    vector<int> C(n + 1, INT_MAX);
    
    C[0] = 0;  // 금액 0을 만들기 위한 최소 동전 개수는 0개

    // 1원부터 n원까지 고려
    for (int j = 1; j <= n; j++) {
        // 각 동전들을 차례대로 고려
        for (int i = 0; i < coins.size(); i++) {
            int coin = coins[i];
            if (coin <= j && C[j - coin] != INT_MAX) {
                C[j] = min(C[j], C[j - coin] + 1);
            }
        }
    }

    // DP 테이블 출력 (위에는 금액, 아래에는 최소 동전 개수)
    cout << "\nDP 테이블:" << endl;
    // 위에 금액 출력
    cout << "j   ";
    for (int i = 1; i <= n; i++) {
        cout << setw(4) << i;  // setw로 간격 맞추기
    }
    cout << endl;

    // 구분선 출력
    cout << "-----------------------------";
    for (int i = 1; i <= n; i++) {
        cout << "----";  // 고정 폭으로 구분선
    }
    cout << endl;

    // 아래에 최소 동전 개수 출력
    cout << "c   ";
    for (int i = 1; i <= n; i++) {
        cout << setw(4) << (C[i] == INT_MAX ? "불가능" : to_string(C[i]));  // 최소 동전 개수 출력
    }
    cout << endl;

    return C[n];  // 최소 동전 개수 반환
}

int main() {
    // 동전 종류 (액면가)
    vector<int> coins = {16, 10, 5, 1};
    // 목표 금액
    int n = 20;

    // 최소 동전 개수 계산
    int result = DPCoinChange(coins, n);

    if (result != -1) {
        cout << "\n거스름돈 " << n << "을 만들기 위한 최소 동전 개수: " << result << endl;
    } else {
        cout << "거스름돈 " << n << "을 만들 수 없습니다." << endl;
    }

    return 0;
}
