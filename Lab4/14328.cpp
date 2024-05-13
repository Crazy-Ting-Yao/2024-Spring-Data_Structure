#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <climits>
class Graph{
    public:
    Graph(int v){
        this->v = v;
        adj = new std::list<std::pair<int,int>>[v];
        passed = new bool[v];
    }
    ~Graph(){
        delete[] adj;
        delete[] passed;
    }
    void addEdge(int u, int v, int w){
        adj[u].push_back(std::make_pair(v,w));
    }
    long long First_Dijkstra(int src, int dest){
        for(int i=0; i<v; i++)
            passed[i] = false;
        this->dest = dest;
        std::vector<long long> dist(v, LLONG_MAX);
        std::vector<int> path(v, -1);
        dist[src] = 0;
        std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
        pq.push(src);
        while(!pq.empty()){
            long long u = pq.top();
            pq.pop();
            for(auto i = adj[u].begin(); i != adj[u].end(); i++){
                int v = (*i).first;
                int w = (*i).second;
                if(dist[v] > dist[u] + w){
                    dist[v] = dist[u] + w;
                    pq.push(v);
                    path[v] = u;
                }
            }
        }
        if(dist[dest] == LLONG_MAX) return -1;
        std::vector<int> via;
        for(int i=dest; i!=src; i=path[i]){
            passed[i] = true;
        }
        passed[src] = true;
        return dist[dest];
    }
    long long Second_Dijkstra(int src){
        std::vector<long long> dist(v, LLONG_MAX);
        dist[src] = 0;
        std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
        pq.push(src);
        while(!pq.empty()){
            int u = pq.top();
            pq.pop();
            for(auto i = adj[u].begin(); i != adj[u].end(); i++){
                int v = (*i).first;
                int w = (*i).second;
                if(dist[v] > dist[u] + w){
                    dist[v] = dist[u] + w;
                    pq.push(v);
                }
            }
        }
        if(dist[dest] == LLONG_MAX) return -1;
        long long min = LLONG_MAX;
        for(int i=0; i<v; i++){
            if(passed[i] == true && dist[i]){
                if(min > dist[i]) min = dist[i];
            }
        }
        return min;
    }
    private:
    int v;
    std::list<std::pair<int,int>> *adj;
    int dest;
    bool *passed;
};

int main(){
    int v, e;
    std::cin >> v >> e;
    Graph g(v);
    for(int i=0; i<e; i++){
        int u, v, w;
        std::cin >> u >> v >> w;
        g.addEdge(u,v,w);
    }
    int t;
    std::cin >> t;
    if(t == 1){
        int src, dest;
        std::cin >> src >> dest;
        std::cout << g.First_Dijkstra(src, dest) << std::endl;
    }
    else if(t == 2){
        int src1, src2, dest;
        std::cin >> src1 >> src2 >> dest;
        long long first = g.First_Dijkstra(src1, dest);
        if(first == -1){
            std::cout << -1 << std::endl;
            return 0;
        }
        long long second = g.Second_Dijkstra(src2);
        if(second == -1){
            std::cout << -1 << std::endl;
            return 0;
        }
        long long a = first + second;
        first = g.First_Dijkstra(src2, dest);
        second = g.Second_Dijkstra(src1);
        long long b = first + second;
        std::cout << (a < b ? a : b) << std::endl;
    }
    return 0;
}