#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 100
#define SEARCH_SIZE 50
#define MAX_VAL 1000

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *create_node(int data) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 오류\n");
        exit(1);
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 노드를 하나 지날 때마다 비교 1회로 셈. 중복 값은 넣지 않음
TreeNode *insert_bst(TreeNode *root, int data, int *build_cmp) {
    if (root == NULL)
        return create_node(data);

    (*build_cmp)++;
    if (data < root->data)
        root->left = insert_bst(root->left, data, build_cmp);
    else if (data > root->data)
        root->right = insert_bst(root->right, data, build_cmp);

    return root;
}

int sequential_search(const int *arr, int size, int target, int *cmp) {
    *cmp = 0;
    for (int i = 0; i < size; i++) {
        (*cmp)++;
        if (arr[i] == target)
            return 1;
    }
    return 0;
}

int bst_search(const TreeNode *root, int target, int *cmp) {
    *cmp = 0;
    while (root != NULL) {
        (*cmp)++;
        if (target == root->data)
            return 1;
        else if (target < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return 0;
}

void free_tree(TreeNode *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void print_array(const int *arr, int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n\n");
}

int main(void) {
    srand((unsigned int)time(NULL));

    int data_array[DATA_SIZE];
    int search_keys[SEARCH_SIZE];
    int used[MAX_VAL + 1] = {0};
    TreeNode *root = NULL;
    int build_cmp = 0;

    
    for (int i = 0; i < DATA_SIZE; ) {
        int r = rand() % (MAX_VAL + 1);
        if (!used[r]) {
            used[r] = 1;
            data_array[i++] = r;
        }
    }

    for (int i = 0; i < DATA_SIZE; i++)
        root = insert_bst(root, data_array[i], &build_cmp);

    // 탐색 키는 데이터와 상관없이 0~1000에서 생성
    for (int i = 0; i < SEARCH_SIZE; i++)
        search_keys[i] = rand() % (MAX_VAL + 1);

    printf("[생성된 100개 데이터]\n");
    print_array(data_array, DATA_SIZE);

    printf("BST Build Cost (Comparisons) : %d\n\n", build_cmp);

    printf("[생성된 50개 탐색 대상 (Search Keys)]\n");
    print_array(search_keys, SEARCH_SIZE);

    int seq_total = 0;
    int bst_total = 0;

    printf("========================================================\n");
    printf("                  50회 탐색 개별 결과\n");
    printf("========================================================\n");

    for (int i = 0; i < SEARCH_SIZE; i++) {
        int target = search_keys[i];
        int seq_cmp, bst_cmp;

        int seq_found = sequential_search(data_array, DATA_SIZE, target, &seq_cmp);
        int bst_found = bst_search(root, target, &bst_cmp);

        seq_total += seq_cmp;
        bst_total += bst_cmp;

        printf("Search Key : %d\n\n", target);
        printf("Sequential Search\n");
        printf("Result      : %s\n", seq_found ? "Found" : "Not Found");
        printf("Comparisons : %d\n\n", seq_cmp);

        printf("BST Search\n");
        printf("Result      : %s\n", bst_found ? "Found" : "Not Found");
        printf("Comparisons : %d\n", bst_cmp);
        printf("--------------------------------------------------------\n");
    }

    printf("\nNumber of searches: %d\n\n", SEARCH_SIZE);

    printf("Sequential Search\n");
    printf("Total comparisons   : %d\n", seq_total);
    printf("Average comparisons : %.2f\n\n", (double)seq_total / SEARCH_SIZE);

    printf("BST Search\n");
    printf("Total comparisons   : %d\n", bst_total);
    printf("Average comparisons : %.2f\n\n", (double)bst_total / SEARCH_SIZE);

    printf("BST Build Cost\n");
    printf("Total comparisons   : %d\n", build_cmp);

    free_tree(root);
    return 0;
}
