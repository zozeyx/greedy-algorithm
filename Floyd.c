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

void displayDistances(char LocationArray[][20], int distanceMatrix[][MAX_SIZE], int locationCount) {
    printf("       ");
    for (int i = 0; i < locationCount; i++) printf("%-10s", LocationArray[i]);
    printf("\n");

    for (int i = 0; i < locationCount; i++) {
        printf("%-10s", LocationArray[i]);
        for (int j = 0; j < locationCount; j++) {
            if (distanceMatrix[i][j] == INF_VALUE)
                printf("%-10s", "INF");
            else
                printf("%-10d", distanceMatrix[i][j]);
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
    clock_t startTime = clock();
    floydWarshall(locationCount, distanceMatrix);
    clock_t endTime = clock();

    printf("\nFloyd-Warshall Algorithm Distance Matrix:\n");
    displayDistances(LocationArray, distanceMatrix, locationCount);
    printf("Running time (ms): %lu\n", (endTime - startTime));

    return 0;
}
