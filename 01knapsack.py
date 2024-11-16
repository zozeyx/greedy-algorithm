def knapsack(weights, values, capacity):
    n = len(weights)  # 물건의 개수
    dp = [[0 for _ in range(capacity + 1)] for _ in range(n + 1)]  # DP 테이블 초기화

    # DP 배열 채우기
    for i in range(1, n + 1):
        for w in range(capacity + 1):
            if weights[i - 1] <= w:
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1])
            else:
                dp[i][w] = dp[i - 1][w]

    # DP 테이블 출력
    print("테이블:")
    for row in dp:
        print(row)

    # 최적의 가치 반환
    return dp[n][capacity]

# 물건들의 무게와 가치
weights = [5, 4, 6, 3]
values = [10, 40, 30, 50]
capacity = 10

# 0-1 배낭 문제 해결
max_value = knapsack(weights, values, capacity)
print(f"\n최대 가치: {max_value}")
