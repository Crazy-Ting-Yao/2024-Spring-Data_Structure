#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <queue>
struct mapkey{
    int src, dest;
    mapkey(int s, int d) {
        src = s > d ? d : s;
        dest = s > d ? s : d;
    }
    bool operator<(const mapkey& k) const{
        return (src == k.src) ? dest < k.dest : src < k.src;
    }
};

struct mapvalue{
    int dist, ts;
    mapvalue(int d, int t) : dist(d), ts(t) {}
};

struct edge{
    int dest, dist;
    edge(int d, int di) : dest(d), dist(di) {}
    bool operator>(const edge& e) const{
        return (dist == e.dist) ? dest > e.dest : dist > e.dist;
    }
};

struct path{
    int src;
    int dest;
    int dist;
    std::vector<int> via;
    path(int s, int d, int di) : src(s), dest(d), dist(di) {
        via.clear();
    }
    path(const path& p) : src(p.src), dest(p.dest), dist(p.dist) , via(p.via) {}
    path(const path* p) : src(p->src), dest(p->dest), dist(p->dist) , via(p->via) {}
    bool operator>(const path& p) const{
        if(dist != p.dist) return dist > p.dist;
        if(dest != p.dest) return dest > p.dest;
        std::priority_queue<int> pq1, pq2;
        for(auto v : via) pq1.push(v);
        for(auto v : p.via) pq2.push(v);
        while(!pq1.empty() && !pq2.empty()){
            if(pq1.top() != pq2.top()) return pq1.top() > pq2.top();
            pq1.pop();
            pq2.pop();
        }
        return via.size() > p.via.size();
    }
};

struct order{
    int ts;
    bool dropping = false;
    path *paths;
    order() : ts(0), paths(nullptr) {}
    order(const int &t, path *p) : ts(t), paths(p) {}
    order(const order& o) : ts(o.ts) , dropping(o.dropping){
        paths = o.paths ? new path(*o.paths) : nullptr ;
    }
    ~order(){ if(paths != nullptr) delete paths; }
};

struct node{
    std::vector<edge> edges;
    int deliverymans = 0;
};

class Solve{
    public:
        Solve();
        ~Solve();
        void Order(const int &id, const int &src, const int &ts);
        void Drop(const int &id, const int &dst, bool from_waiting = false);
        void Complete(const int &id);
        std::map<int, order> orders;
    private: 
        node *nodes;
        std::map<mapkey, mapvalue> map;
        int nodes_count;
        void update(const int &src, const std::vector<int> &via, const int &ts);
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int,int>>> waiting;
        bool *visited;
        void waiting_list_check();
};

int main(){
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    Solve solve;
    int ops;
    std::string cmd;
    std::cin >> ops;
    while (ops--){
        std::cin >> cmd;
        if (cmd == "Order"){
            int id, src, ts;
            std::cin >> id >> src >> ts;
            solve.Order(id, src, ts);
        }
        else if (cmd == "Drop"){
            int id, dst;
            std::cin >> id >> dst;
            solve.Drop(id, dst, false);
        }
        else if (cmd == "Complete"){
            int id;
            std::cin >> id;
            solve.Complete(id);
        }
    }
    //solve.print_edges();
    return 0;
}

Solve::Solve(){
    int v, e, d;
    std::cin >> v >> e >> d;
    nodes_count = v+1;
    nodes = new node[v+1];
    visited = new bool[v+1];
    for(int i=0;i<d;i++){
        std::string PLACE;
        int vertex, count;
        std::cin >> PLACE >> vertex >> count;
        nodes[vertex].deliverymans = count;
    }
    std::priority_queue<edge, std::vector<edge>, std::greater<edge>> pq[v+1];
    for(int i=0;i<e;i++){
        std::string EDGE;
        int src, dst, dist, ts;
        std::cin >> EDGE >> src >> dst >> dist >> ts;
        pq[src].push(edge(dst, dist));
        pq[dst].push(edge(src, dist));
        map.insert(std::pair<mapkey, mapvalue>(mapkey(src, dst), mapvalue(dist, ts)));
    }
    for(int i=0;i<v+1;i++){
        int size = pq[i].size();
        for(int j=0;j<size;j++){
            nodes[i].edges.push_back(pq[i].top());
            pq[i].pop();
        }
    }
}

Solve::~Solve(){
    if(nodes != nullptr) delete[] nodes;
    if(visited != nullptr) delete[] visited;
}

void Solve::Order(const int &id, const int &src, const int &ts){
    // Here, the "path" in order is reversed, i.e., the src is the restaurant and the dest is the deliveryman's location
    order neworder(ts, nullptr);
     // if there is a deliveryman at the source, return the order immediately
    if(nodes[src].deliverymans > 0){
        neworder.paths = new path(src, src, 0);
        orders.insert(std::pair<int, order>(id, neworder));
        nodes[src].deliverymans--;
        std::cout << "Order " << id << " from: " << src << std::endl;
        return;
    }
    std::priority_queue<path, std::vector<path>, std::greater<path>> pq;
    for(int i=0; i<nodes_count; i++) visited[i] = false;
    path p(src, src, 0);
    pq.push(p);
    while(!pq.empty()){
        path p = pq.top();
        pq.pop();
        if(visited[p.dest]) continue;
        visited[p.dest] = true;
        //if the current path is already longer than the path found, return the order immediately
        if(neworder.paths != nullptr && p.dist >= neworder.paths->dist) break;
        // get all the edges of the current node
        for(auto e : nodes[p.dest].edges){
            // if the edge's traffic capacity is less than the order's required traffic capacity, skip it
            if(map.find(mapkey(p.dest, e.dest))->second.ts < ts) continue;
            // If there exists a path, check if the current path is shorter than the path found
            if(neworder.paths != nullptr){
                // If the path is longer, skip it
                if(neworder.paths->dist <= p.dist + e.dist) continue;
                // If the destination is a deliveryman, update the path
                if(nodes[e.dest].deliverymans > 0){
                    path newpath(p);
                    newpath.dest = e.dest;
                    newpath.dist += e.dist;
                    newpath.via.push_back(e.dest);
                    delete neworder.paths;
                    neworder.paths = new path(newpath);
                }
                // If the destination is not a deliveryman, push it to the priority queue
                else{
                    path newpath(p);
                    newpath.dest = e.dest;
                    newpath.dist += e.dist;
                    newpath.via.push_back(e.dest);
                    pq.push(newpath);
                }
                continue;
            }
            // First path found: if the destination is a deliveryman, update the path
            if(nodes[e.dest].deliverymans > 0){
                path newpath(p);
                newpath.dest = e.dest;
                newpath.dist += e.dist;
                newpath.via.push_back(e.dest);
                neworder.paths = new path(newpath);
            }
            // If the destination is not a deliveryman, push it to the priority queue
            else{
                path newpath(p);
                newpath.dest = e.dest;
                newpath.dist += e.dist;
                newpath.via.push_back(e.dest);
                pq.push(newpath);
            }
        }
    }
    // If there exists a path, return the order
    if(neworder.paths != nullptr){
        orders.insert(std::pair<int, order>(id, neworder));
        nodes[neworder.paths->dest].deliverymans--;
        update(neworder.paths->src, neworder.paths->via, -ts);
        std::cout << "Order " << id << " from: " << neworder.paths->dest << std::endl;
        return;
    }
    // If none of the deliveryman is reachable, drop the order
    std::cout << "Just walk. T-T" << std::endl;
}

void Solve::update(const int &src, const std::vector<int> &via, const int &ts){
    int from = src;
    if(via.empty()) return;
    if(via.size() == 1 && via[0] == src) return;
    for(int i=0; i<via.size(); i++){
        map.find(mapkey(from, via[i]))->second.ts += ts;
        from = via[i];
    }
}

void Solve::Drop(const int &id, const int &dst, bool from_waiting){
    if(orders.find(id) == orders.end()) return;
    order current_order = orders.find(id)->second;
    const int ts = current_order.ts;
    const int src = current_order.paths->src;
    // Return the traffic capacity
    if(!from_waiting) {
        update(src, current_order.paths->via, ts);
        waiting_list_check();
    }
    // If the destination is the deliveryman's location, return the order immediately
    if(src == dst){
        std::cout << "Order " << id << " distance: " << current_order.paths->dist << std::endl;
        delete orders.find(id)->second.paths;
        orders.find(id)->second.paths = new path(src, src, current_order.paths->dist);
        orders.find(id)->second.dropping = true;
        return;
    }
    path next = path(src, src, current_order.paths->dist);
    delete current_order.paths;
    current_order.paths = nullptr;
    path start(next);
    std::priority_queue<path, std::vector<path>, std::greater<path>> pq;
    pq.push(start);
    for(int i=0; i<nodes_count; i++) visited[i] = false;
    while(!pq.empty()){
        path p = pq.top();
        pq.pop();
        if(visited[p.dest]) continue;
        visited[p.dest] = true;
        // If the current path is already longer than or equal to the path found, return the order immediately
        if(current_order.paths != nullptr && p.dist >= current_order.paths->dist) break;
        // Get all the edges of the current node
        for(auto e : nodes[p.dest].edges){
            // If the edge's traffic capacity is less than the order's required traffic capacity, skip it
            if(map.find(mapkey(p.dest, e.dest))->second.ts < ts) continue;
            // If there exists a path
            if(current_order.paths != nullptr){
                // If the path is longer than or euqal to the path found, skip it
                if(current_order.paths->dist <= p.dist + e.dist) continue;
                // If the path's destination is the customer's location, update the path
                if(e.dest == dst){
                    path newpath(p);
                    newpath.dest = e.dest;
                    newpath.dist += e.dist;
                    newpath.via.push_back(e.dest);
                    delete current_order.paths;
                    current_order.paths = new path(newpath);
                }
                // If the path's destination is not the customer's location, push it to the priority queue
                else{
                    path newpath(p);
                    newpath.dest = e.dest;
                    newpath.dist += e.dist;
                    newpath.via.push_back(e.dest);
                    pq.push(newpath);
                }
                continue;
            }
            // First path found
            if(e.dest == dst){
                path newpath(p);
                newpath.dest = e.dest;
                newpath.dist += e.dist;
                newpath.via.push_back(e.dest);
                current_order.paths = new path(newpath);
            }
            // If the destination is not the customer's location, push it to the priority queue
            else{
                path newpath(p);
                newpath.dest = e.dest;
                newpath.dist += e.dist;
                newpath.via.push_back(e.dest);
                pq.push(newpath);
            }
        }
    }
    // If there exists a path, return the order
    if(current_order.paths != nullptr){
        std::cout << "Order " << id << " distance: " << current_order.paths->dist << std::endl;
        update(current_order.paths->src, current_order.paths->via, -ts);
        delete orders.find(id)->second.paths;
        orders.find(id)->second.paths = new path(current_order.paths);
        orders.find(id)->second.dropping = true;
        
        return;
    }
    // If no route is found, put the order into the waiting queue
    if(!from_waiting) std::cout << "No Way Home" << std::endl;
    // make the path's src and dest the position of the restaurant
    int dist = orders.find(id)->second.paths->dist;
    order neworder(ts, nullptr);
    neworder.paths = new path(src, src, dist);
    orders.erase(id);
    orders.insert(std::pair<int, order>(id, neworder));
    waiting.push(std::pair<int, int>(id, dst));
    return;
}

void Solve::Complete(const int &id){
    order current_order = orders.find(id)->second;
    if (!current_order.dropping) exit(1);
    update(current_order.paths->src, current_order.paths->via, current_order.ts);
    nodes[current_order.paths->dest].deliverymans++;
    orders.erase(id);
    // Try to process the drop operations in the waiting queue
    waiting_list_check();
}

void Solve::waiting_list_check(){
    int size = waiting.size();
    if(size == 0) return;
    std::pair<int, int> temp[size];
    for(int i=0; i<size; i++){
        temp[i] = waiting.top();
        waiting.pop();
    }
    for(auto t : temp){
        Drop(t.first, t.second, true);
    }
    return;
}