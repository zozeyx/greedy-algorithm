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

int checkUniqueness(Path DistanceArray[], char* loc, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(DistanceArray[i].endLocation, loc) == 0) return 0;
    }
    return 1;
}

int isLocationUnique(char LocationArray[][20], char* loc, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(LocationArray[i], loc) == 0) return 0;
    }
    return 1;
}

int calculateDistance(Path paths[], int numPaths, char origin[], char destination[]) {
    for (int i = 0; i < numPaths; i++) {
        if (!(strcmp(paths[i].startLocation, origin) || strcmp(paths[i].endLocation, destination))) {
            return paths[i].distance;
        } else if (!(strcmp(paths[i].endLocation, origin) || strcmp(paths[i].startLocation, destination))) {
            return paths[i].distance;
        }
    }
    return INF_VALUE;
}

void updateDistances(Path paths[], int numPaths, Path DistanceArray[], int pathCount, char origin[], int minDist) {
    for (int i = 0; i < pathCount; i++) {
        int dist = calculateDistance(paths, numPaths, origin, DistanceArray[i].endLocation);
        if (dist < INF_VALUE) dist += minDist;

        if (dist < DistanceArray[i].distance) DistanceArray[i].distance = dist;
    }
}

Path* findShortestPath(Path paths[], int numPaths, char start[], int* pathCount) {
    Path* DistanceArray = (Path*)malloc(numPaths * sizeof(Path));
    int visited[MAX_SIZE] = {0};
    
    for (int i = 0; i < numPaths; i++) {
        if (checkUniqueness(DistanceArray, paths[i].startLocation, *pathCount)) {
            strcpy(DistanceArray[*pathCount].startLocation, start);
            strcpy(DistanceArray[*pathCount].endLocation, paths[i].startLocation);
            DistanceArray[*pathCount].distance = INF_VALUE;
            (*pathCount)++;
        } else if (checkUniqueness(DistanceArray, paths[i].endLocation, *pathCount)) {
            strcpy(DistanceArray[*pathCount].startLocation, start);
            strcpy(DistanceArray[*pathCount].endLocation, paths[i].endLocation);
            DistanceArray[*pathCount].distance = INF_VALUE;
            (*pathCount)++;
        }
    }
    
    for (int i = 0; i < *pathCount; i++) {
        if (!strcmp(start, DistanceArray[i].endLocation)) {
            DistanceArray[i].distance = 0;
            break;
        }
    }

    for (int i = 0; i < *pathCount; i++) {
        int minDist = INF_VALUE;
        int minIndex = -1;

        for (int j = 0; j < *pathCount; j++) {
            if (!visited[j] && DistanceArray[j].distance < minDist) {
                minDist = DistanceArray[j].distance;
                minIndex = j;
            }
        }
        if (minIndex == -1) break;
        visited[minIndex] = 1;
        
        updateDistances(paths, numPaths, DistanceArray, *pathCount, DistanceArray[minIndex].endLocation, DistanceArray[minIndex].distance);
    }

    return DistanceArray;
}

void displayDistances(char LocationArray[][20], int distanceMatrix[][MAX_SIZE], int locationCount) {
    printf("       ");
    for (int i = 0; i < locationCount; i++) printf("%-10s", LocationArray[i]);
    printf("\n");

    for (int i = 0; i < locationCount; i++) {
        printf("%-10s", LocationArray[i]);
        for (int j = 0; j < locationCount; j++) {
            if (j < i) printf("        ");
            else printf("%-8d", distanceMatrix[i][j]);
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

    for (int i = 0; i < numPaths; i++) {
        if (isLocationUnique(LocationArray, paths[i].startLocation, locationCount)) {
            strcpy(LocationArray[locationCount++], paths[i].startLocation);
        } else if (isLocationUnique(LocationArray, paths[i].endLocation, locationCount)) {
            strcpy(LocationArray[locationCount++], paths[i].endLocation);
        }
    }
    
    clock_t totalRunTime = 0;
    for (int i = 0; i < locationCount; i++) {
        int pathCount = 0;
        clock_t startTime = clock();
        Path* shortestPath = findShortestPath(paths, numPaths, LocationArray[i], &pathCount);
        clock_t endTime = clock();
        totalRunTime += (endTime - startTime);

        for (int j = 0; j < locationCount; j++) {
            distanceMatrix[i][j] = INF_VALUE;
        }
        
        for (int j = 0; j < pathCount; j++) {
            for (int k = 0; k < locationCount; k++) {
                if (!strcmp(shortestPath[j].endLocation, LocationArray[k])) {
                    distanceMatrix[i][k] = shortestPath[j].distance;
                    break;
                }
            }
        }
        free(shortestPath);
    }
    displayDistances(LocationArray, distanceMatrix, locationCount);
    printf("Running time (ms): %lu\n", totalRunTime);

    return 0;
}
