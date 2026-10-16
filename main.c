#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 100   // 정점 최대 개수
#define SPARSE_EDGES 100   // 희소 그래프 간선 수
#define DENSE_EDGES  4000  // 밀집 그래프 간선 수

// 간선 구조체
typedef struct Edge {
    int u;
    int v;
} Edge;

// 인접 행렬 그래프 구조체
typedef struct MatrixGraph {
    int matrix[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
} MatrixGraph;

// 인접 리스트 노드 구조체
typedef struct ListNode {
    int vertex;             // 인접 정점 번호
    struct ListNode* next;  // 다음 노드 포인터
} ListNode;

// 인접 리스트 그래프 구조체
typedef struct ListGraph {
    int num_vertices;
    ListNode* adj_list[MAX_VERTICES]; // 헤드 포인터 배열
    int node_count;                   // 메모리 계산용 노드 총합
} ListGraph;

// ==========================================================
// 1. 초기화 및 메모리 측정
// ==========================================================

// 인접 행렬 초기화
void init_matrix_graph(MatrixGraph* g) {
    g->num_vertices = MAX_VERTICES;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            g->matrix[i][j] = 0;
        }
    }
}

// 인접 리스트 초기화
void init_list_graph(ListGraph* g) {
    g->num_vertices = MAX_VERTICES;
    g->node_count = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->adj_list[i] = NULL;
    }
}

// 인접 행렬 메모리 계산
size_t get_matrix_memory(MatrixGraph* g) {
    return sizeof(MatrixGraph);
}

// 인접 리스트 메모리 계산
size_t get_list_memory(ListGraph* g) {
    size_t base_size = sizeof(ListGraph);
    size_t node_size = (size_t)(g->node_count) * sizeof(ListNode);
    return base_size + node_size;
}

// ==========================================================
// 2. 간선 삽입
// ==========================================================

// 인접 행렬 간선 삽입 (비교 횟수 반환)
int insert_edge_matrix(MatrixGraph* g, int u, int v) {
    int comparisons = 0;
    comparisons++;

    // 중복 간선 방지
    if (g->matrix[u][v] == 1) {
        return comparisons;
    }

    g->matrix[u][v] = 1;
    g->matrix[v][u] = 1;
    return comparisons;
}

// 인접 리스트 단방향 간선 삽입
int insert_one_way_list(ListGraph* g, int from, int to) {
    int comparisons = 0;
    ListNode* curr = g->adj_list[from];

    // 중복 노드 검사
    while (curr != NULL) {
        comparisons++;
        if (curr->vertex == to) {
            return comparisons;
        }
        curr = curr->next;
    }

    // 리스트 맨 앞에 새 노드 삽입
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    new_node->vertex = to;
    new_node->next = g->adj_list[from];
    g->adj_list[from] = new_node;
    g->node_count++;

    return comparisons;
}

// 인접 리스트 무방향 간선 삽입
int insert_edge_list(ListGraph* g, int u, int v) {
    int total_comparisons = 0;
    total_comparisons += insert_one_way_list(g, u, v);
    total_comparisons += insert_one_way_list(g, v, u);
    return total_comparisons;
}

// ==========================================================
// 3. 간선 삭제
// ==========================================================

// 인접 행렬 간선 삭제
int delete_edge_matrix(MatrixGraph* g, int u, int v) {
    int comparisons = 0;
    comparisons++;

    if (g->matrix[u][v] == 1) {
        g->matrix[u][v] = 0;
        g->matrix[v][u] = 0;
    }

    return comparisons;
}

// 인접 리스트 단방향 간선 삭제
int delete_one_way_list(ListGraph* g, int from, int to) {
    int comparisons = 0;
    ListNode* curr = g->adj_list[from];
    ListNode* prev = NULL;

    // 삭제할 노드 탐색
    while (curr != NULL) {
        comparisons++;
        if (curr->vertex == to) {
            if (prev == NULL) {
                g->adj_list[from] = curr->next; // 첫 번째 노드 삭제
            }
            else {
                prev->next = curr->next;        // 중간/마지막 노드 삭제
            }
            free(curr);
            g->node_count--;
            return comparisons;
        }
        prev = curr;
        curr = curr->next;
    }
    return comparisons;
}

// 인접 리스트 무방향 간선 삭제
int delete_edge_list(ListGraph* g, int u, int v) {
    int total_comparisons = 0;
    total_comparisons += delete_one_way_list(g, u, v);
    total_comparisons += delete_one_way_list(g, v, u);
    return total_comparisons;
}

// ==========================================================
// 4. 연결 확인 및 인접 정점 출력
// ==========================================================

// 인접 행렬 연결 확인
int check_connectivity_matrix(MatrixGraph* g, int u, int v) {
    int comparisons = 0;
    comparisons++; // O(1) 탐색
    return comparisons;
}

// 인접 리스트 연결 확인
int check_connectivity_list(ListGraph* g, int u, int v) {
    int comparisons = 0;
    ListNode* curr = g->adj_list[u];

    while (curr != NULL) {
        comparisons++;
        if (curr->vertex == v) {
            return comparisons;
        }
        curr = curr->next;
    }
    return comparisons;
}

// 인접 행렬 인접 정점 출력
int find_adjacent_matrix(MatrixGraph* g, int u, int print_flag) {
    int comparisons = 0;
    if (print_flag) printf("  [행렬] 정점 %d의 인접 정점: ", u);

    for (int v = 0; v < MAX_VERTICES; v++) {
        comparisons++;
        if (g->matrix[u][v] == 1) {
            if (print_flag) printf("%d ", v);
        }
    }
    if (print_flag) printf("\n");
    return comparisons;
}

// 인접 리스트 인접 정점 출력
int find_adjacent_list(ListGraph* g, int u, int print_flag) {
    int comparisons = 0;
    ListNode* curr = g->adj_list[u];
    if (print_flag) printf("  [리스트] 정점 %d의 인접 정점: ", u);

    while (curr != NULL) {
        comparisons++;
        if (print_flag) printf("%d ", curr->vertex);
        curr = curr->next;
    }
    if (print_flag) printf("\n");
    return comparisons;
}

// ==========================================================
// 5. 실험 실행 및 결과 출력
// ==========================================================
void run_experiment(int target_edges, const char* graph_title, int case_num_matrix, int case_num_list) {
    MatrixGraph mat_g;
    ListGraph list_g;
    init_matrix_graph(&mat_g);
    init_list_graph(&list_g);

    // 난수 간선 생성용 방문 배열 및 저장 배열
    int temp_check[MAX_VERTICES][MAX_VERTICES] = { 0 };
    Edge* edges = (Edge*)malloc(sizeof(Edge) * target_edges);

    // 중복 없는 난수 간선 생성
    int count = 0;
    while (count < target_edges) {
        int u = rand() % MAX_VERTICES;
        int v = rand() % MAX_VERTICES;

        if (u != v && temp_check[u][v] == 0) {
            temp_check[u][v] = 1;
            temp_check[v][u] = 1;
            edges[count].u = u;
            edges[count].v = v;
            count++;
        }
    }

    // 그래프 데이터 삽입
    for (int i = 0; i < target_edges; i++) {
        insert_edge_matrix(&mat_g, edges[i].u, edges[i].v);
        insert_edge_list(&list_g, edges[i].u, edges[i].v);
    }

    // [측정 1] 메모리 사용량
    size_t mem_matrix = get_matrix_memory(&mat_g);
    size_t mem_list = get_list_memory(&list_g);

    // 삽입할 비존재 간선 탐색
    int ins_u = 0, ins_v = 0;
    while (1) {
        ins_u = rand() % MAX_VERTICES;
        ins_v = rand() % MAX_VERTICES;
        if (ins_u != ins_v && temp_check[ins_u][ins_v] == 0) {
            break;
        }
    }

    // [측정 2-1] 간선 삽입 비교 횟수
    int ins_comp_matrix = insert_edge_matrix(&mat_g, ins_u, ins_v);
    int ins_comp_list = insert_edge_list(&list_g, ins_u, ins_v);

    // 임시 삽입 간선 복구
    delete_edge_matrix(&mat_g, ins_u, ins_v);
    delete_edge_list(&list_g, ins_u, ins_v);

    // 삭제할 기존 간선 선택
    int del_u = edges[target_edges - 1].u;
    int del_v = edges[target_edges - 1].v;

    // [측정 2-2] 간선 삭제 비교 횟수
    int del_comp_matrix = delete_edge_matrix(&mat_g, del_u, del_v);
    int del_comp_list = delete_edge_list(&list_g, del_u, del_v);

    // 임시 삭제 간선 복구
    insert_edge_matrix(&mat_g, del_u, del_v);
    insert_edge_list(&list_g, del_u, del_v);

    // 연결 확인할 임의 정점 선택
    int conn_u = rand() % MAX_VERTICES;
    int conn_v = rand() % MAX_VERTICES;
    while (conn_u == conn_v) conn_v = rand() % MAX_VERTICES;

    // [측정 3] 연결 여부 확인 비교 횟수
    int conn_comp_matrix = check_connectivity_matrix(&mat_g, conn_u, conn_v);
    int conn_comp_list = check_connectivity_list(&list_g, conn_u, conn_v);

    // [측정 4] 인접 정점 출력 비교 횟수
    int adj_u = rand() % MAX_VERTICES;
    int adj_comp_matrix = find_adjacent_matrix(&mat_g, adj_u, 0);
    int adj_comp_list = find_adjacent_list(&list_g, adj_u, 0);

    // 결과 출력
    printf("Case %d : %s - Adjacency Matrix\n", case_num_matrix, graph_title);
    printf("Memory                         : %zu Bytes\n", mem_matrix);
    printf("Edge insertion comparisons     : %d\n", ins_comp_matrix);
    printf("Edge deletion comparisons      : %d\n", del_comp_matrix);
    printf("Connectivity check comparisons : %d\n", conn_comp_matrix);
    printf("Adjacent vertices comparisons  : %d\n\n", adj_comp_matrix);

    printf("Case %d : %s - Adjacency List\n", case_num_list, graph_title);
    printf("Memory                         : %zu Bytes\n", mem_list);
    printf("Edge insertion comparisons     : %d\n", ins_comp_list);
    printf("Edge deletion comparisons      : %d\n", del_comp_list);
    printf("Connectivity check comparisons : %d\n", conn_comp_list);
    printf("Adjacent vertices comparisons  : %d\n\n", adj_comp_list);

    free(edges);
}

int main(void) {
    srand((unsigned int)time(NULL));

    run_experiment(SPARSE_EDGES, "Sparse Graph", 1, 2);
    run_experiment(DENSE_EDGES, "Dense Graph", 3, 4);

    return 0;
}