#include <iostream>
#include <vector>
#include <climits>
#include <sstream>
#include <fstream>

using namespace std;

// 행렬의 차원을 추출하는 함수
pair<int, int> get_matrix_dimensions(const vector<vector<int>>& matrix) {
    return {matrix.size(), matrix[0].size()};
}

// 행렬 곱셈의 최소 횟수를 계산하는 함수 (동적 계획법)
int matrix_chain_order(const vector<pair<int, int>>& dims, int n) {
    vector<vector<int>> dp(n, vector<int>(n, INT_MAX));
    vector<vector<int>> split(n, vector<int>(n, -1));

    // 길이가 1인 경우, 곱셈 횟수는 0
    for (int i = 0; i < n; ++i) {
        dp[i][i] = 0;
    }

    // DP 테이블을 채운다
    for (int len = 2; len <= n; ++len) {  // len은 곱셈 범위의 길이
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            for (int k = i; k < j; ++k) {
                int q = dp[i][k] + dp[k+1][j] + dims[i].first * dims[k].second * dims[j].second;
                if (q < dp[i][j]) {
                    dp[i][j] = q;
                    split[i][j] = k;
                }
            }
        }
    }

    // 결과 출력
    return dp[0][n-1];
}

// 행렬 이름을 인덱스로 변환하는 함수
int matrix_name_to_index(const string& name) {
    return name[1] - '1';
}

// 행렬 파일을 읽고, 행렬을 파싱하는 함수
vector<vector<vector<int>>> parse_matrices_from_file(const string& filename, int& num_matrices) {
    ifstream file(filename);
    string line;
    vector<vector<vector<int>>> matrices;
    vector<vector<int>> current_matrix;
    string current_matrix_name;

    while (getline(file, line)) {
        if (line.empty()) continue;

        // 행렬 이름 (A1, A2 등)
        if (line[0] == 'A') {
            if (!current_matrix.empty()) {
                matrices.push_back(current_matrix);
            }
            current_matrix.clear();

            current_matrix_name = line.substr(0, 2);  // 예: A1, A2 등
        } else {
            // 행렬 데이터 (정수로 변환)
            stringstream ss(line);
            vector<int> row;
            int num;
            while (ss >> num) {
                row.push_back(num);
            }
            current_matrix.push_back(row);
        }
    }
    if (!current_matrix.empty()) {
        matrices.push_back(current_matrix); // 마지막 행렬 추가
    }

    num_matrices = matrices.size();
    return matrices;
}

int main() {
    // 파일 경로 설정
    string filename = "matrix_input.txt";
    int num_matrices;

    // 행렬 파싱
    vector<vector<vector<int>>> matrices = parse_matrices_from_file(filename, num_matrices);

    // 각 행렬의 차원 추출
    vector<pair<int, int>> dims;
    for (const auto& matrix : matrices) {
        dims.push_back(get_matrix_dimensions(matrix));
    }

    // 행렬 곱셈의 최소 횟수 계산
    int min_operations = matrix_chain_order(dims, num_matrices);

    cout << "최소 곱셈 횟수: " << min_operations << endl;

    return 0;
}
