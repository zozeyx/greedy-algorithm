#include <iostream>
#include <vector>
#include <limits.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// 2D DP 배열을 출력하는 함수
void printDPTable(const vector<vector<int>>& dp) {
    cout << "DP 테이블:" << endl;
    for (const auto& row : dp) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

// 체이닝 행렬 곱셈 알고리즘
void matrixChainMultiplication(const vector<int>& dims) {
    int n = dims.size() - 1;
    
    // DP 테이블 초기화
    vector<vector<int>> dp(n, vector<int>(n, 0));
    
    // 최소 곱셈 횟수 계산
    for (int len = 2; len <= n; len++) { // len: 체인의 길이
        for (int i = 0; i < n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                int q = dp[i][k] + dp[k+1][j] + dims[i] * dims[k+1] * dims[j+1];
                if (q < dp[i][j]) {
                    dp[i][j] = q;
                }
            }
        }
    }

    // DP 테이블 출력
    printDPTable(dp);

    // 최소 곱셈 횟수 출력
    cout << "최소 곱셈 횟수: " << dp[0][n-1] << endl;
}

int main() {
    string filename = "matrix_input.txt";
    ifstream file(filename);
    vector<int> dims; // 행렬 크기를 저장할 벡터
    
    string line;
    while (getline(file, line)) {
        // 행렬 크기 파싱: 예: A1 = [[ -9  -2 ... ]]
        size_t pos = line.find("=");
        if (pos != string::npos) {
            string matrixPart = line.substr(pos + 1);
            matrixPart = matrixPart.substr(2, matrixPart.size() - 4); // A1 = [[ ]] 부분 제거

            stringstream ss(matrixPart);
            int value;
            while (ss >> value) {
                dims.push_back(value);
            }
        }
    }

    file.close();

    // 행렬의 크기 (dims)는 행렬 A1, A2, ..., An의 차원을 포함해야 합니다.
    // 예를 들어: dims = { 10, 20, 30, 40, 30 } (4개의 행렬, 차원 10x20, 20x30, 30x40, 40x30)
    
    matrixChainMultiplication(dims);

    return 0;
}
