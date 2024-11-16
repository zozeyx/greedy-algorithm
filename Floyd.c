#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_SIZE 100
#define INF_VALUE INT_MAX

typedef struct {
    char startLocation[20];
    char endLocation[20];
    int distance;
} Path;

int isLocationUnique(char LocationArray[][20], char* loc, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(LocationArray[i], loc) == 0) return 0;
    }
    return 1;
}

void floydWarshall(int locationCount, int distanceMatrix[MAX_SIZE][MAX_SIZE]) {
    for (int k = 0; k < locationCount; k++) {
        for (int i = 0; i < locationCount; i++) {
            for (int j = 0; j < locationCount; j++) {
                if (distanceMatrix[i][k] != INF_VALUE && distanceMatrix[k][j] != INF_VALUE) {
                    int newDist = distanceMatrix[i][k] + distanceMatrix[k][j];
                    if (newDist < distanceMatrix[i][j]) {
                        distanceMatrix[i][j] = newDist;
                    }
                }
            }
        }
    }
}

void dijkstra(int locationCount, int distanceMatrix[MAX_SIZE][MAX_SIZE], int start, int result[MAX_SIZE]) {
    int visited[MAX_SIZE] = {0};
    for (int i = 0; i < locationCount; i++) result[i] = INF_VALUE;
    result[start] = 0;

    for (int i = 0; i < locationCount; i++) {
        int minDist = INF_VALUE, u = -1;
        for (int j = 0; j < locationCount; j++) {
            if (!visited[j] && result[j] < minDist) {
                minDist = result[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < locationCount; v++) {
            if (!visited[v] && distanceMatrix[u][v] != INF_VALUE && result[u] + distanceMatrix[u][v] < result[v]) {
                result[v] = result[u] + distanceMatrix[u][v];
            }
        }
    }
}

void displayDiagonalUpperTable(char LocationArray[][20], int distanceMatrix[][MAX_SIZE], int locationCount) {
    printf("       ");
    for (int i = 0; i < locationCount; i++) printf("%-10s", LocationArray[i]);
    printf("\n");

    for (int i = 0; i < locationCount; i++) {
        printf("%-10s", LocationArray[i]);
        for (int j = 0; j < locationCount; j++) {
            if (i <= j) {
                if (distanceMatrix[i][j] == INF_VALUE)
                    printf("%-10s", "INF");
                else
                    printf("%-10d", distanceMatrix[i][j]);
            } else {
                printf("%-10s", "");
            }
        }
        printf("\n");
    }
}

int main() {
    Path paths[] = {
        {"서울", "원주", 15}, {"서울", "천안", 12}, {"원주", "강릉", 21}, {"원주", "대구", 7},
        {"천안", "논산", 4}, {"천안", "대전", 10}, {"논산", "대전", 3}, {"논산", "광주", 13},
        {"강릉", "포항", 25}, {"대전", "대구", 10}, {"광주", "부산", 15}, {"대구", "포항", 19},
        {"대구", "부산", 9}, {"부산", "포항", 5}
    };
    int numPaths = sizeof(paths) / sizeof(paths[0]);

    char LocationArray[MAX_SIZE][20];
    int locationCount = 0;
    int distanceMatrix[MAX_SIZE][MAX_SIZE];

    // 초기화
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            distanceMatrix[i][j] = (i == j) ? 0 : INF_VALUE;
        }
    }

    for (int i = 0; i < numPaths; i++) {
        if (isLocationUnique(LocationArray, paths[i].startLocation, locationCount)) {
            strcpy(LocationArray[locationCount++], paths[i].startLocation);
        }
        if (isLocationUnique(LocationArray, paths[i].endLocation, locationCount)) {
            strcpy(LocationArray[locationCount++], paths[i].endLocation);
        }

        // 거리 업데이트
        int startIdx = -1, endIdx = -1;
        for (int j = 0; j < locationCount; j++) {
            if (strcmp(LocationArray[j], paths[i].startLocation) == 0) startIdx = j;
            if (strcmp(LocationArray[j], paths[i].endLocation) == 0) endIdx = j;
        }
        distanceMatrix[startIdx][endIdx] = paths[i].distance;
        distanceMatrix[endIdx][startIdx] = paths[i].distance; // 무방향 그래프
    }

    // 플로이드-와샬 실행 시간 측정
    clock_t startTimeFloyd = clock();
    floydWarshall(locationCount, distanceMatrix);
    clock_t endTimeFloyd = clock();

    printf("\nFloyd-Warshall Algorithm Distance Matrix (Diagonal Upper Only):\n");
    displayDiagonalUpperTable(LocationArray, distanceMatrix, locationCount);
    printf("Running time (ms): %lu\n", (endTimeFloyd - startTimeFloyd));

    // 다익스트라 실행 시간 측정
    clock_t startTimeDijkstra = clock();
    int dijkstraResult[MAX_SIZE];
    for (int i = 0; i < locationCount; i++) {
        dijkstra(locationCount, distanceMatrix, i, dijkstraResult);
    }
    clock_t endTimeDijkstra = clock();
    printf("\nDijkstra Algorithm Running time (ms): %lu\n", (endTimeDijkstra - startTimeDijkstra));

    return 0;
}
