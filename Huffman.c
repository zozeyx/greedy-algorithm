#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_MAX 128
#define MAX_LEN 8

typedef struct NODE {
    struct NODE *left, *right;
    int ascii;
    int cnt;
} NODE;

typedef struct {
    NODE *heap[ASCII_MAX];
    int size;
} PriorityQueue;

void insert_node(PriorityQueue* pq, NODE* node) {
    pq->heap[pq->size] = node;
    int i = pq->size;

    while(i > 0 && pq->heap[i]->cnt < pq->heap[(i-1)/2]->cnt) {
        NODE* temp = pq->heap[(i-1)/2];
        pq->heap[(i-1)/2] = pq->heap[i];
        pq->heap[i] = temp;
        i = (i-1)/2;
    }

    pq->size++;
}

NODE* PriorityQueue_pop(PriorityQueue* pq) {
    if(pq->size == 0) return NULL;

    NODE* min_node = pq->heap[0];
    pq->heap[0] = pq->heap[--pq->size];
    pq->heap[pq->size] = NULL;

    int i = 0;
    while(i*2+1 < pq->size) {
        int left = i*2+1;
        int right = i*2+2;
        int min = left;

        if(right < pq->size && pq->heap[right]->cnt < pq->heap[left]->cnt) min = right;
        if(pq->heap[i]->cnt < pq->heap[min]->cnt) break;

        NODE* temp = pq->heap[i];
        pq->heap[i] = pq->heap[min];
        pq->heap[min] = temp;

        i = min;
    }

    return min_node;
}

NODE* HuffmanCoding(char* file_name) {
    int input[ASCII_MAX] = {0};

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("%s 파일을 열 수 없음.\n", file_name);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* inputs = (char*)malloc(file_size+1);
    fscanf(file, "%s", inputs);
    fclose(file);

    for(int i = 0; i < file_size; i++) {
        input[inputs[i]]++;
    }

    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));

    for(int i = 0; i < ASCII_MAX; i++) {
        if(input[i] > 0 ) {
           NODE* node = (NODE*)malloc(sizeof(NODE));
           node->ascii = i;
           node->cnt = input[i];
           node->left = node->right = NULL;

           insert_node(pq, node);
        }
    }

    while(pq->size >= 2) {
        NODE* left = PriorityQueue_pop(pq);
        NODE* right = PriorityQueue_pop(pq);

        NODE* node = (NODE*)malloc(sizeof(NODE));
        node->cnt = left->cnt + right->cnt;
        node->left = left;
        node->right = right;

        insert_node(pq, node);
    }

    return pq->heap[0];
}

void getCode(NODE* node, char code[][MAX_LEN], char c[]) {
    char left[MAX_LEN];
    char right[MAX_LEN];

    if(node->left == NULL && node->right == NULL) {
        strcpy(code[node->ascii], c);
        return ;
    }
    if(node->left != NULL) {
        strcpy(left, c);
        strcat(left, "0");
        getCode(node->left, code, left);
    }
    if(node->right != NULL){
        strcpy(right, c);
        strcat(right, "1");
        getCode(node->right, code, right);
    }
}

void HuffmanEncoding(NODE* root, char* file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("%s 파일을 열 수 없음.\n", file_name);
        return ;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* inputs = (char*)malloc(file_size+1);
    fscanf(file, "%s", inputs);
    fclose(file);

    char code[ASCII_MAX][MAX_LEN];
    char c[MAX_LEN] = "";
    for(int i = 0; i < ASCII_MAX; i++) {
        code[i][0] = '\0';
    }
    getCode(root, code, c);
    
    // Console 출력
    for(int i = 0; i < file_size; i++) {
        printf("%s", code[inputs[i]]);
    }
    printf("\n"); // 출력 후 개행 추가
}

void HuffmanDecoding(NODE* root, char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("%s 파일을 열 수 없음.\n", file_name);
        return ;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* inputs = (char*)malloc(file_size+1);
    fscanf(file, "%s", inputs);
    fclose(file);

    // Console 출력
    NODE* node = root;
    for(int i = 0; i < file_size; i++) {
        if(node->left == NULL && node->right == NULL) {
            printf("%c", node->ascii);
            node = root;
        }
        if(inputs[i] == '0') {
            node = node->left;
        }
        else if(inputs[i] == '1') {
            node = node->right;
        }
    }
    if(node->left == NULL && node->right == NULL) {
        printf("%c", node->ascii);
    }
    printf("\n"); // 출력 후 개행 추가
}

int main() {
    NODE* root = HuffmanCoding("Huffman_input.txt");
    HuffmanEncoding(root, "Huffman_input.txt");
    HuffmanDecoding(root, "Huffman_encoded.txt"); // 허프만 인코딩 결과 파일에서 디코딩

    return 0;
}
