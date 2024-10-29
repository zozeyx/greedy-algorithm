#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_MAX 128
#define MAX_CODE_LENGTH 8

typedef struct Node {
    struct Node *left, *right;
    int ascii_value;
    int frequency;
} Node;

typedef struct {
    Node *heap[ASCII_MAX];
    int size;
} PriorityQueue;

void enqueue(PriorityQueue* pq, Node* node) {
    pq->heap[pq->size] = node;
    int idx = pq->size;

    while (idx > 0 && pq->heap[idx]->frequency < pq->heap[(idx - 1) / 2]->frequency) {
        Node* temp = pq->heap[(idx - 1) / 2];
        pq->heap[(idx - 1) / 2] = pq->heap[idx];
        pq->heap[idx] = temp;
        idx = (idx - 1) / 2;
    }

    pq->size++;
}

Node* dequeue(PriorityQueue* pq) {
    if (pq->size == 0) return NULL;

    Node* min_node = pq->heap[0];
    pq->heap[0] = pq->heap[--pq->size];
    pq->heap[pq->size] = NULL;

    int idx = 0;
    while (idx * 2 + 1 < pq->size) {
        int left_child = idx * 2 + 1;
        int right_child = idx * 2 + 2;
        int min_idx = left_child;

        if (right_child < pq->size && pq->heap[right_child]->frequency < pq->heap[left_child]->frequency) min_idx = right_child;
        if (pq->heap[idx]->frequency < pq->heap[min_idx]->frequency) break;

        Node* temp = pq->heap[idx];
        pq->heap[idx] = pq->heap[min_idx];
        pq->heap[min_idx] = temp;

        idx = min_idx;
    }

    return min_node;
}

Node* createHuffmanTree(char* filename) {
    int char_frequency[ASCII_MAX] = {0};

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return NULL;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        char_frequency[(int)ch]++;
    }
    fclose(file);

    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->size = 0;

    for (int i = 0; i < ASCII_MAX; i++) {
        if (char_frequency[i] > 0) {
            Node* node = (Node*)malloc(sizeof(Node));
            node->ascii_value = i;
            node->frequency = char_frequency[i];
            node->left = node->right = NULL;

            enqueue(pq, node);
        }
    }

    while (pq->size >= 2) {
        Node* left_node = dequeue(pq);
        Node* right_node = dequeue(pq);

        Node* parent_node = (Node*)malloc(sizeof(Node));
        parent_node->frequency = left_node->frequency + right_node->frequency;
        parent_node->left = left_node;
        parent_node->right = right_node;

        enqueue(pq, parent_node);
    }

    return pq->heap[0];
}

void generateCodes(Node* node, char codes[][MAX_CODE_LENGTH], char current_code[]) {
    char left_code[MAX_CODE_LENGTH];
    char right_code[MAX_CODE_LENGTH];

    if (node->left == NULL && node->right == NULL) {
        strcpy(codes[node->ascii_value], current_code);
        return;
    }

    if (node->left != NULL) {
        strcpy(left_code, current_code);
        strcat(left_code, "0");
        generateCodes(node->left, codes, left_code);
    }

    if (node->right != NULL) {
        strcpy(right_code, current_code);
        strcat(right_code, "1");
        generateCodes(node->right, codes, right_code);
    }
}

void printHuffmanCodes(char codes[][MAX_CODE_LENGTH]) {
    printf("Huffman Codes:\n");
    for (int i = 0; i < ASCII_MAX; i++) {
        if (codes[i][0] != '\0') {
            printf("'%c' = %s\n", i, codes[i]);
        }
    }
}

void decodeHuffman(Node* root, const char* encoded) {
    Node* current_node = root;
    printf("Decoded Output: ");
    for (int i = 0; encoded[i] != '\0'; i++) {
        if (encoded[i] == '0') {
            current_node = current_node->left;
        } else if (encoded[i] == '1') {
            current_node = current_node->right;
        }

        if (current_node->left == NULL && current_node->right == NULL) {
            printf("%c", current_node->ascii_value);
            current_node = root;
        }
    }
    printf("\n");
}

int main() {
    Node* root = createHuffmanTree("Huffman_input.txt");

    char codes[ASCII_MAX][MAX_CODE_LENGTH] = {{0}};
    char current_code[MAX_CODE_LENGTH] = "";

    generateCodes(root, codes, current_code);
    printHuffmanCodes(codes);

    const char* encoded_sequence = "10110010001110101010100";
    decodeHuffman(root, encoded_sequence);

    return 0;
}
