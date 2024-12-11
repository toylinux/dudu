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
            printf("%s - %s: %d0万元\n", villages[u], villages[v], cost);
        }
    }

    printf("\n总预算成本: %d0万元\n", totalCost);
}

void initializeData() {
    villageCount = 7;
    strcpy(villages[0], "黄宙坑");
    strcpy(villages[1], "徐洋");
    strcpy(villages[2], "后村");
    strcpy(villages[3], "塘堀");
    strcpy(villages[4], "钵下");
    strcpy(villages[5], "龙洋");
    strcpy(villages[6], "富楼源");

    edgeCount = 14;
    edges[0] = (Edge){0, 1, 12};
    edges[1] = (Edge){0, 2, 12};
    edges[2] = (Edge){0, 3, 16};
    edges[3] = (Edge){0, 5, 19};
    edges[4] = (Edge){1, 2, 6};
    edges[5] = (Edge){1, 4, 8};
    edges[6] = (Edge){1, 5, 8};
    edges[7] = (Edge){1, 6, 14};
    edges[8] = (Edge){2, 3, 5};
    edges[9] = (Edge){2, 4, 6};
    edges[10] = (Edge){3, 4, 5};
    edges[11] = (Edge){3, 6, 12};
    edges[12] = (Edge){4, 6, 7};
    edges[13] = (Edge){5, 6, 13};
}

int main() {
    initializeData();

    kruskal();

    return 0;
}
