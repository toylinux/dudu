#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// 定义边结构
typedef struct {
    int u, v;     // 两个端点
    int cost;     // 边的权重
} Edge;

// 定义村庄名称数组和边数组
char villages[MAX][50];
Edge edges[MAX];
int parent[MAX];

// 边数组和村庄数量
int edgeCount = 0, villageCount = 0;

// 辅助函数：查找集合代表（用于并查集）
int find(int x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]);
    return parent[x];
}

// 辅助函数：合并两个集合
void unionSets(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY)
        parent[rootX] = rootY;
}

// Kruskal算法核心实现
void kruskal() {
    // 按成本升序排序边
    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = 0; j < edgeCount - i - 1; j++) {
            if (edges[j].cost > edges[j + 1].cost) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    // 初始化并查集
    for (int i = 0; i < villageCount; i++)
        parent[i] = i;

    int totalCost = 0;
    int selectedEdges = 0;

    printf("\n需要建设的公路及其预算成本如下：\n");

    for (int i = 0; i < edgeCount && selectedEdges < villageCount - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int cost = edges[i].cost;

        if (find(u) != find(v)) {
            unionSets(u, v);
            totalCost += cost;
            selectedEdges++;
            printf("%s - %s: %d万元\n", villages[u], villages[v], cost);
        }
    }

    printf("\n总预算成本: %d0万元\n", totalCost);
}

int main() {
    printf("请输入村庄数量: ");
    scanf("%d", &villageCount);

    printf("请输入各村庄名称:\n");
    for (int i = 0; i < villageCount; i++) {
        scanf("%s", villages[i]);
    }

    printf("请输入公路数量: ");
    scanf("%d", &edgeCount);

    printf("请输入每条公路的信息（格式: 村庄1 村庄2 成本）:\n");
    for (int i = 0; i < edgeCount; i++) {
        char village1[50], village2[50];
        int cost;
        scanf("%s %s %d", village1, village2, &cost);

        int u = -1, v = -1;
        for (int j = 0; j < villageCount; j++) {
            if (strcmp(village1, villages[j]) == 0)
                u = j;
            if (strcmp(village2, villages[j]) == 0)
                v = j;
        }

        edges[i].u = u;
        edges[i].v = v;
        edges[i].cost = cost;
    }

    kruskal();

    return 0;
}
