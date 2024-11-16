#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <sstream>

using namespace std;

// 행렬 크기 정보
struct Matrix {
    int rows, cols;
};

// Chained Matrix Multiplications 알고리즘
void matrixChainOrder(const vector<Matrix>& matrices) {
    int n = matrices.size();
    vector<vector<int>> m(n, vector<int>(n, 0)); // 최소 곱셈 횟수를 저장하는 DP 테이블
    vector<vector<int>> s(n, vector<int>(n, 0)); // 분할 지점을 저장하는 테이블

    // l은 체인의 길이
    for (int l = 2; l <= n; l++) {
        for (int i = 0; i <= n - l; i++) {
            int j = i + l - 1;
            m[i][j] = INT_MAX; // 초기화

            // k는 분할 지점
            for (int k = i; k < j; k++) {
                int q = m[i][k] + m[k+1][j] + matrices[i].rows * matrices[k].cols * matrices[j].cols;
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    // DP 테이블 출력
    cout << "DP Table (최소 곱셈 횟수):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }

    // 최고 곱셈 횟수 출력
    cout << "최고 곱셈 횟수: " << m[0][n-1] << endl;
}

// 입력 파일에서 행렬 크기 정보를 읽어들임
vector<Matrix> readMatrices(const string& filename) {
    ifstream file(filename);
    vector<Matrix> matrices;
    string line;
    int rows, cols;

    while (getline(file, line)) {
        stringstream ss(line);
        ss >> rows >> cols;
        matrices.push_back({rows, cols});
    }

    return matrices;
}

int main() {
    string filename = "matrix_input.txt"; // 입력 파일 경로
    vector<Matrix> matrices = readMatrices(filename);

    matrixChainOrder(matrices);

    return 0;
}
