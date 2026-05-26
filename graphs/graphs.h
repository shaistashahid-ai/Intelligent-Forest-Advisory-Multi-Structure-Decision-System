#ifndef GRAPHS_H
#define GRAPHS_H

struct GNode {
    int dest; 
    int weight; 
    GNode* next; 
};

class Graphs {
    static const int MAX_V = 20;
    GNode* adj[MAX_V];
    int V;

public:
    Graphs(int vertices);
    ~Graphs();

    void addEdge(int src, int dest, int weight = 1);
    void bfs(int start);
    void dfsUtil(int v, bool visited[]);
    void printAdjList();
    void printAdjMatrix();
    int  safePath(int src, int dest);

    static void demo();
    static void fireMenu(int choice);
    static void routingMenu(int choice);
};

#endif
