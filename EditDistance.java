public class EditDistance {

    // Edit Distance 계산 함수
    public static int minDistance(String word1, String word2) {
        int m = word1.length();
        int n = word2.length();
        
        // dp 배열 생성
        int[][] dp = new int[m + 1][n + 1];
        
        // dp 배열 초기화
        for (int i = 0; i <= m; i++) {
            for (int j = 0; j <= n; j++) {
                if (i == 0) {
                    dp[i][j] = j; // 첫 번째 문자열이 비어있을 때는 삽입만 하면 됨
                } else if (j == 0) {
                    dp[i][j] = i; // 두 번째 문자열이 비어있을 때는 삭제만 하면 됨
                } else {
                    int cost = (word1.charAt(i - 1) == word2.charAt(j - 1)) ? 0 : 1; // 문자가 같으면 0, 다르면 1
                    dp[i][j] = Math.min(Math.min(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + cost);
                }
            }
        }
        
        // dp 테이블 출력
        printTable(dp, word1, word2);

        // dp[m][n]에 최종 결과가 저장됨
        return dp[m][n];
    }

    // dp 테이블 출력 함수
    public static void printTable(int[][] dp, String word1, String word2) {
        System.out.println("Edit Distance Table:");
        
        // 열의 헤더 출력
        System.out.print("    ");
        for (int j = 0; j < word2.length(); j++) {
            System.out.print(word2.charAt(j) + "  ");
        }
        System.out.println();
        
        // 각 행 출력
        for (int i = 0; i <= word1.length(); i++) {
            if (i == 0) {
                System.out.print("  ");
            } else {
                System.out.print(word1.charAt(i - 1) + " ");
            }
            
            for (int j = 0; j <= word2.length(); j++) {
                System.out.print(dp[i][j] + "  ");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        String word1 = "strong";
        String word2 = "stone";

        int result = minDistance(word1, word2);
        System.out.println("The minimum edit distance is: " + result);
    }
}
