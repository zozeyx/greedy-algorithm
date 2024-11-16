#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <sstream>

using namespace std;

// 행렬을 저장하기 위한 구조체
struct Matrix {
    int rows, cols;
    vector<vector<int>> data;
};

// 파일에서 행렬을 읽어오는 함수
Matrix readMatrixFromFile(ifstream& file) {
    Matrix matrix;
    string line;
    
    // 행렬의 각 행을 읽어서 저장
    while (getline(file, line)) {
        stringstream ss(line);
        int val;
        vector<int> row;
        
        while (ss >> val) {
            row.push_back(val);
            if (ss.peek() == ',') ss.ignore();  // comma를 건너뛰기
        }

        if (row.size() > 0) {
            if (matrix.rows == 0) {
                matrix.cols = row.size();
            }
            matrix.data.push_back(row);
            matrix.rows++;
        }
    }

    return matrix;
}

// Chained Matrix Multiplication 동적 계획법 구현
int matrixChainOrder(const vector<Matrix>& matrices, vector<vector<int>>& m, vector<vector<int>>& s) {
    int n = matrices.size();
    
    // m[i][j]는 A_i부터 A_j까지 곱하는 최소 비용을 저장
    // s[i][j]는 분할 지점을 저장
    for (int i = 0; i < n; ++i) {
        m[i][i] = 0;  // 한 행렬 자체는 곱할 필요가 없으므로 0
    }

    // 두 개 이상의 행렬을 곱할 때의 최적화
    for (int length = 2; length <= n; ++length) {  // 곱할 행렬의 수
        for (int i = 0; i <= n - length; ++i) {
            int j = i + length - 1;
            m[i][j] = INT_MAX;  // 최소값을 찾기 위한 초기화

            // 행렬 분할 지점 k 찾기
            for (int k = i; k < j; ++k) {
                int q = m[i][k] + m[k+1][j] + matrices[i].rows * matrices[k].cols * matrices[j].cols;
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    return m[0][n-1];  // 전체 행렬 곱셈의 최소 비용 반환
}

// DP 테이블 출력 함수
void printTable(const vector<vector<int>>& m) {
    int n = m.size();
    cout << "DP 테이블 (m):" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << m[i][j] << "\t";
        }
        cout << endl;
    }
}

int main() {
    ifstream file("matrix_input.txt");
    
    // 행렬 입력받기
    vector<Matrix> matrices;
    while (file) {
        string line;
        getline(file, line);
        if (line.find("A") == 0) {  // 행렬 이름 (A1, A2 등)
            matrices.push_back(readMatrixFromFile(file));
        }
    }
    
    int n = matrices.size();  // 행렬의 수
    vector<vector<int>> m(n, vector<int>(n, 0));  // 최소 계산 비용을 위한 DP 테이블
    vector<vector<int>> s(n, vector<int>(n, 0));  // 최적 분할 지점 테이블
    
    // Chained Matrix Multiplication 실행
    int minCost = matrixChainOrder(matrices, m, s);
    
    // 최소 곱셈 횟수 출력
    cout << "최소 곱셈 횟수: " << minCost << endl;
    
    // DP 테이블 출력
    printTable(m);

    return 0;
}
