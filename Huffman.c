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

    while(idx > 0 && pq->heap[idx]->frequency < pq->heap[(idx - 1) / 2]->frequency) {
        Node* temp = pq->heap[(idx - 1) / 2];
        pq->heap[(idx - 1) / 2] = pq->heap[idx];
        pq->heap[idx] = temp;
        idx = (idx - 1) / 2;
    }

    pq->size++;
}

Node* dequeue(PriorityQueue* pq) {
    if(pq->size == 0) return NULL;

    Node* min_node = pq->heap[0];
    pq->heap[0] = pq->heap[--pq->size];
    pq->heap[pq->size] = NULL;

    int idx = 0;
    while(idx * 2 + 1 < pq->size) {
        int left_child = idx * 2 + 1;
        int right_child = idx * 2 + 2;
        int min_idx = left_child;

        if(right_child < pq->size && pq->heap[right_child]->frequency < pq->heap[left_child]->frequency) min_idx = right_child;
        if(pq->heap[idx]->frequency < pq->heap[min_idx]->frequency) break;

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
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* input_data = (char*)malloc(file_size + 1);
    fscanf(file, "%s", input_data);
    fclose(file);

    for(int i = 0; i < file_size; i++) {
        char_frequency[input_data[i]]++;
    }

    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->size = 0;

    for(int i = 0; i < ASCII_MAX; i++) {
        if(char_frequency[i] > 0) {
            Node* node = (Node*)malloc(sizeof(Node));
            node->ascii_value = i;
            node->frequency = char_frequency[i];
            node->left = node->right = NULL;

            enqueue(pq, node);
        }
    }

    while(pq->size >= 2) {
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

    if(node->left == NULL && node->right == NULL) {
        strcpy(codes[node->ascii_value], current_code);
        return;
    }
    if(node->left != NULL) {
        strcpy(left_code, current_code);
        strcat(left_code, "0");
        generateCodes(node->left, codes, left_code);
    }
    if(node->right != NULL) {
        strcpy(right_code, current_code);
        strcat(right_code, "1");
        generateCodes(node->right, codes, right_code);
    }
}

void encodeHuffman(Node* root, char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* input_data = (char*)malloc(file_size + 1);
    fscanf(file, "%s", input_data);
    fclose(file);

    char codes[ASCII_MAX][MAX_CODE_LENGTH];
    char current_code[MAX_CODE_LENGTH] = "";
    for(int i = 0; i < ASCII_MAX; i++) {
        codes[i][0] = '\0';
    }
    generateCodes(root, codes, current_code);
    
    // Print encoded values to console
    printf("Encoded Huffman Codes:\n");
    for(int i = 0; i < file_size; i++) {
        printf("%s", codes[input_data[i]]);
    }
    printf("\n"); // Add a new line after printing all codes
}

void decodeHuffman(Node* root, char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* input_data = (char*)malloc(file_size + 1);
    fscanf(file, "%s", input_data);
    fclose(file);

    // Print decoded characters to console
    printf("Decoded Characters:\n");
    Node* current_node = root;
    for(int i = 0; i < file_size; i++) {
        if(current_node->left == NULL && current_node->right == NULL) {
            printf("%c", current_node->ascii_value);
            current_node = root;
        }
        if(input_data[i] == '0') {
            current_node = current_node->left;
        } else if(input_data[i] == '1') {
            current_node = current_node->right;
        }
    }
    // Check if there is a character left to print
    if(current_node->left == NULL && current_node->right == NULL) {
        printf("%c", current_node->ascii_value);
    }
    printf("\n"); // Add a new line after printing all characters
}

int main() {
    Node* root = createHuffmanTree("Huffman_input.txt");
    encodeHuffman(root, "Huffman_input.txt");
    decodeHuffman(root, "Huffman_encoded.txt"); // You may need to adjust the input file name as needed

    return 0;
}
