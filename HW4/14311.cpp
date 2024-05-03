#include <iostream>
#include <string>
#include <vector>
#include <map>

struct route{
    int src;
    int dst;
    int ts;
    route(int s, int d, int t) : ts(t) {
        if(s < d){
            src = s;
            dst = d;
        }
        else{
            src = d;
            dst = s;
        }
    }
    bool operator<(const route& r) const{
        if(src == r.src)
            return dst < r.dst;
        return src < r.src;
    }
};


struct path{
    int src;
    int dest;
    int dist;
    int ts;
    std::vector<int> via;
    path(int s, int d, int di, int t) : ts(t) {
        src = s;
        dest = d;
        dist = di;
    }
    path(const path& p){
        src = p.src;
        dest = p.dest;
        dist = p.dist;
        ts = p.ts;
        via = p.via;
    }
    bool operator>(const path& p) const{
        if (dist == p.dist)
            return dest > p.dest;
        return dist > p.dist;
    }
};

struct order{
    int ts;
    path *paths;
    ~order(){
        if(paths != nullptr)
            delete paths;
    }
};

class myPriorityQueue{
    public:
        myPriorityQueue();
        myPriorityQueue(const myPriorityQueue& pq);
        void push(path data);
        path pop();
        bool isempty();
        std::vector<path> data;
};

struct node{
    int id;
    myPriorityQueue connected_nodes;
    int deliverymans;
};

class myMap{
    public:
        myMap();
        ~myMap();
        void Order(int id, int src, int ts);
        void Drop(int id, int dst);
        void Complete(int id);
    private: 
        node *nodes;
        std::map<route, int> route_dis_check;
        std::map<int, order> orders;
        void update_ts(int src, std::vector<int> via, int ts);
        int nodes_count;
};

int main(){
    myMap map;
    int ops;
    std::string cmd;
    std::cin >> ops;
    while (ops--){
        std::cin >> cmd;
        if (cmd == "Order"){
            int id, src, ts;
            std::cin >> id >> src >> ts;
            map.Order(id, src, ts);
        }
        else if (cmd == "Drop"){
            int id, dst;
            std::cin >> id >> dst;
            map.Drop(id, dst);
        }
        else if (cmd == "Complete"){
            int id;
            std::cin >> id;
            map.Complete(id);
        }
    }
    return 0;
}

myPriorityQueue::myPriorityQueue(){}

myPriorityQueue::myPriorityQueue(const myPriorityQueue& pq){
    data = pq.data;
}

void myPriorityQueue::push(path newdata){
    for(int i=0;i<data.size();i++){
        if(data[i] > newdata){
            data.insert(data.begin()+i, newdata);
            return;
        }
    }
    data.push_back(newdata);
}

path myPriorityQueue::pop(){
    path temp = data[0];
    data.erase(data.begin());
    return temp;
}

bool myPriorityQueue::isempty(){
    return data.empty();
}

myMap::myMap(){
    int v, e, d;
    std::cin >> v >> e >> d;
    nodes_count = v+1;
    nodes = new node[v+1];
    for(int i=0;i<=v;i++){
        nodes[i].id = i;
        nodes[i].deliverymans = 0;
    }
    for(int i=0;i<d;i++){
        std::string PLACE;
        int vertex, count;
        std::cin >> PLACE >> vertex >> count;
        nodes[vertex].deliverymans = count;
    }
    for(int i=0;i<e;i++){
        std::string EDGE;
        int src, dst, dist, ts;
        std::cin >> EDGE >> src >> dst >> dist >> ts;
        route newroute(src, dst, ts);
        route_dis_check.insert(std::pair<route, int>(newroute, dist));
        nodes[src].connected_nodes.push(path(src, dst, dist, ts));
        nodes[dst].connected_nodes.push(path(dst, src, dist, ts));
    }
}

myMap::~myMap(){
    if (nodes != nullptr)
        delete[] nodes;
}

void myMap::Order(int id, int src, int ts){
    order neworder;
    if(nodes[src].deliverymans > 0){
        neworder.ts = ts;
        neworder.paths = nullptr;
        orders[id] = neworder;
        nodes[src].deliverymans--;
        return;
    }
    myPriorityQueue pq;
    path start(src, src, 0, ts);
    pq.push(start);
    bool *passed = new bool[nodes_count];
    for(int i=0;i<nodes_count;i++)
        passed[i] = false;
    passed[src] = true;
    neworder.ts = ts;
    neworder.paths = nullptr;
    while(!pq.isempty()){
        path current = pq.pop();
        for(auto node_paths : nodes[current.dest].connected_nodes.data){
            int node = node_paths.dest;
            if(passed[node]) continue;
            std::cout << node << std::endl;
            passed[node] = true;
            if(neworder.paths != nullptr && current.dist >= neworder.paths->dist){
                delete[] passed;
                std::cout << "Order " << id << " from: " << current.dest << std::endl;
                nodes[neworder.paths->dest].deliverymans--;
                update_ts(src, neworder.paths->via, ts*-1);
                orders.insert(std::pair<int, order>(id, neworder));
                return;
            }
            
        }
    }
    
    delete[] passed;
    if(neworder.paths != nullptr){
        std::cout << "Order " << id << " from: " << neworder.paths->dest << std::endl;
        nodes[neworder.paths->dest].deliverymans--;
        update_ts(src, neworder.paths->via, ts*-1);
        orders.insert(std::pair<int, order>(id, neworder));
        return;
    }
    std::cout << "Just walk. T-T" << std::endl;
}

void myMap::Drop(int id, int dst){

}

void myMap::Complete(int id){

}

void myMap::update_ts(int src, std::vector<int> via, int ts){
    int from = src;
    for(int i=0;i<via.size();i++){
        route newroute(from, via[i], 0);
        routeinfos.find(newroute)->second.ts += ts;
        from = via[i];
    }
}