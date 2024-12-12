#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 128
#define MAX_CODE_LEN 128

// 定义哈夫曼树节点结构
typedef struct Node {
    char symbol;
    int weight;
    struct Node *left, *right;
} Node;

// 定义全局变量
Node *huffmanTree = NULL;
char codes[MAX_NODES][MAX_CODE_LEN];
int weights[MAX_NODES];

// 优先队列节点
typedef struct PriorityQueue {
    Node *nodes[MAX_NODES];
    int size;
} PriorityQueue;

// 优先队列操作
void enqueue(PriorityQueue *pq, Node *node) {
    pq->nodes[pq->size++] = node;
    for (int i = pq->size - 1; i > 0 && pq->nodes[i]->weight < pq->nodes[i - 1]->weight; i--) {
        Node *temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[i - 1];
        pq->nodes[i - 1] = temp;
    }
}

Node *dequeue(PriorityQueue *pq) {
    return pq->nodes[--pq->size];
}

// 创建哈夫曼树
Node *buildHuffmanTree(char symbols[], int weights[], int n) {
    PriorityQueue pq = { .size = 0 };

    for (int i = 0; i < n; i++) {
        Node *node = (Node *)malloc(sizeof(Node));
        node->symbol = symbols[i];
        node->weight = weights[i];
        node->left = node->right = NULL;
        enqueue(&pq, node);
    }

    while (pq.size > 1) {
        Node *left = dequeue(&pq);
        Node *right = dequeue(&pq);

        Node *parent = (Node *)malloc(sizeof(Node));
        parent->symbol = '\0';
        parent->weight = left->weight + right->weight;
        parent->left = left;
        parent->right = right;
        enqueue(&pq, parent);
    }

    return dequeue(&pq);
}

// 生成哈夫曼编码
void generateCodes(Node *root, char *code, int depth) {
    if (!root) return;

    if (!root->left && !root->right) {
        code[depth] = '\0';
        strcpy(codes[(int)root->symbol], code);
        return;
    }

    code[depth] = '0';
    generateCodes(root->left, code, depth + 1);

    code[depth] = '1';
    generateCodes(root->right, code, depth + 1);
}

// 打印编码
void printCodes(char symbols[], int n) {
    printf("字符\t权值\t编码\n");
    for (int i = 0; i < n; i++) {
        printf("%c\t%d\t%s\n", symbols[i], weights[i], codes[(int)symbols[i]]);
    }
}

// 编码输入的字符串
void encodeString(char *input, char *output) {
    output[0] = '\0';
    for (int i = 0; input[i] != '\0'; i++) {
        // 检查字符是否在编码表中
        if (strlen(codes[(int)input[i]]) == 0) {
            printf("警告: 字符 '%c' 没有对应的编码\n", input[i]);
            continue;
        }
        strcat(output, codes[(int)input[i]]);
    }
}

// 译码
void decode(Node *root, const char *encoded) {
    Node *current = root;
    printf("译码结果: ");
    for (int i = 0; encoded[i] != '\0'; i++) {
        current = (encoded[i] == '0') ? current->left : current->right;

        if (!current->left && !current->right) {
            printf("%c", current->symbol);
            current = root;
        }
    }
    printf("\n");
}

// 打印哈夫曼树
void printTree(Node *root, int depth) {
    if (!root) return;

    for (int i = 0; i < depth; i++) printf("  ");

    if (root->symbol) printf("%c (%d)\n", root->symbol, root->weight);
    else printf("* (%d)\n", root->weight);

    printTree(root->left, depth + 1);
    printTree(root->right, depth + 1);
}

// 从文件读取字符和权值
int readFromFile(const char *filename, char symbols[], int weights[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开文件");
        return 0;
    }

    int n = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // 确保第一个字符存在
        if (line[0] == '\0' || line[0] == '\n') continue;
        
        symbols[n] = line[0];
        char *weight_str = line + 1;  // 跳过第一个字符
        
        // 转换权值
        weights[n] = atoi(weight_str);
        n++;
    }

    fclose(file);
    return n;
}

// 主程序
int main() {
    char symbols[MAX_NODES];

    int n = readFromFile("dict.txt", symbols, weights);
    if (n == 0) {
        printf("文件读取失败或文件为空\n");
        return 1;
    }

    huffmanTree = buildHuffmanTree(symbols, weights, n);

    char code[MAX_CODE_LEN];
    generateCodes(huffmanTree, code, 0);

    int choice;
    do {
        printf("\n菜单:\n");
        printf("1. 显示哈夫曼编码\n");
        printf("2. 编码字符串\n");
        printf("3. 译码\n");
        printf("4. 显示哈夫曼树\n");
        printf("5. 退出\n");
        printf("请输入选项: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printCodes(symbols, n);
                break;
            case 2: {
                char input[MAX_CODE_LEN], encoded[MAX_CODE_LEN];
                printf("请输入要编码的字符串: ");
                getchar();  // 清除之前的换行符
                fgets(input, MAX_CODE_LEN, stdin);
                input[strcspn(input, "\n")] = 0;  // 移除换行符
                encodeString(input, encoded);
                printf("编码结果: %s\n", encoded);
                break;
            }
            case 3: {
                char encoded[MAX_CODE_LEN];
                printf("请输入要译码的字符串: ");
                scanf("%s", encoded);
                decode(huffmanTree, encoded);
                break;
            }
            case 4:
                printf("哈夫曼树结构:\n");
                printTree(huffmanTree, 0);
                break;
            case 5:
                printf("退出程序\n");
                break;
            default:
                printf("无效选项，请重新输入\n");
                break;
        }
    } while (choice != 5);

    return 0;
}
