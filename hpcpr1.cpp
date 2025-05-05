#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>  // Include OpenMP header
using namespace std;

class Graph {
private:
    int V;
    vector<int>* adj;

public:
    Graph(int V) {
        this->V = V;
        adj = new vector<int>[V];
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    void bfs(int s) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[s] = true;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";

            // OpenMP parallel for loop for exploring neighbors
            #pragma omp parallel for
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                if (!visited[v]) {
                    visited[v] = true;
                    // We cannot directly push to queue in parallel, so do it sequentially
                    #pragma omp critical
                    {
                        q.push(v);
                    }
                }
            }
        }
    }

    void dfs(int s) {
        vector<bool> visited(V, false);
        dfs_helper(s, visited);
    }

private:
    void dfs_helper(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";

        // OpenMP parallel for loop for recursive DFS (in each level of recursion)
        #pragma omp parallel for
        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];
            if (!visited[v]) {
                dfs_helper(v, visited);
            }
        }
    }
};

int main() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    g.addEdge(3, 5);

    cout << "BFS starting from vertex 3: ";
    g.bfs(3);
    cout << endl;

    cout << "DFS starting from vertex 5: ";
    g.dfs(5);
    cout << endl;

    return 0;
}

