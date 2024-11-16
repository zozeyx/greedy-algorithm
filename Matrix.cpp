#include <iostream>
#include <vector>
#include <climits>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>

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

// 행렬 파일을 읽고, 행렬을 파싱하는 함수
vector<vector<vector<int>>> parse_matrices_from_file(const string& filename, int& num_matrices) {
    ifstream file(filename);
    string line;
    vector<vector<vector<int>>> matrices;
    vector<vector<int>> current_matrix;
    bool inside_matrix = false;

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        exit(1);
    }

    while (getline(file, line)) {
        // 행렬 구분자 처리
        if (line.find("A") != string::npos && line.find("=") != string::npos) {
            // 이전 행렬이 있으면 저장
            if (!current_matrix.empty()) {
                matrices.push_back(current_matrix);
                current_matrix.clear();
            }
            inside_matrix = true;
            continue;
        }

        // 행렬 데이터 읽기
        if (inside_matrix && line.find("[[") != string::npos) {
            // 행렬 시작 부분을 찾으면
            string matrix_data = line.substr(line.find("[[") + 2);
            stringstream ss(matrix_data);
            vector<int> row;
            int num;

            while (ss >> num) {
                row.push_back(num);
            }

            // 행렬의 행 추가
            if (!row.empty()) {
                current_matrix.push_back(row);
            }
        }

        // 행렬 끝나는 부분 찾기
        if (inside_matrix && line.find("]]") != string::npos) {
            matrices.push_back(current_matrix);
            current_matrix.clear();
            inside_matrix = false;
        }
    }

    // 마지막 행렬 처리
    if (!current_matrix.empty()) {
        matrices.push_back(current_matrix);
    }

    num_matrices = matrices.size();
    return matrices;
}

int main() {
    // 파일 경로 설정
    string filename = "matrices.txt";
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
