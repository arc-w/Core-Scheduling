#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>
#include <queue>
// http://www.cplusplus.com/reference/queue/queue/

template<typename TnodeInfo, typename TedgeInfo>
class Graph {
public:
    int N;
    char** A;
    TnodeInfo* nodeInfo;
    TedgeInfo** edgeInfo;

    Graph(int numNodes) {
        int i, j;

        N = numNodes;

        // allocate the adjacency matrix
        A = new char* [N];
        for (i = 0; i < N; i++)
            A[i] = new char[N];
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++) A[i][j] = 0;

        // allocate the array with node information
        nodeInfo = new TnodeInfo[N];

        // allocate the matrix of edge information
        edgeInfo = new TedgeInfo * [N];
        for (i = 0; i < N; i++)
            edgeInfo[i] = new TedgeInfo[N];
    }

    void setNodeInfo(int i, TnodeInfo info) {
        nodeInfo[i] = info;
    }

    TnodeInfo getNodeInfo(int i) {
        return nodeInfo[i];
    }

    void addEdge(int i, int j) {
        A[i][j] = A[j][i] = 1;
    }
    void removeEdge(int i, int j) {
        A[i][j] = A[j][i] = 0;
    }

    void setEdgeInfo(int i, int j, TedgeInfo info) {
        edgeInfo[i][j] = edgeInfo[j][i] = info;
    }

    TedgeInfo getEdgeInfo(int i, int j) {
        return edgeInfo[i][j];
    }

    ~Graph() {
        int i;
        for (i = 0; i < N; i++) {
            delete A[i];
            delete edgeInfo[i];
        }
        delete A;
        delete edgeInfo;
        delete nodeInfo;
    }
};
#endif // GRAPH_H