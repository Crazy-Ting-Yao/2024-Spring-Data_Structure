#include <iostream>
#include <string>
#include <vector>
#include <map>

struct route{
    int src;
    int dst;
    route(int s, int d){
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

struct routeinfo{
    int dist;
    int ts;
    routeinfo(int d, int t){
        dist = d;
        ts = t;
    }
};

struct path{
    int src;
    int dest;
    int dist;
    bool operator>(const path& p) const{
        if (dist == p.dist)
            return dest > p.dest;
        return dist > p.dist;
    }
};

struct order{
    int id;
    int src;
    int ts;
    std::vector<path> paths;
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
    std::vector<int> connected_nodes;
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
        std::vector<order> orders;
        node* nodes;
        std::vector<path> DijsktraForOrder(int src, int ts);
        std::vector<path> Route(int src, int dst);
        void DijkstraForDrop(int src, int dst);
        std::map<route, routeinfo> routeinfos;
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
    nodes = new node[v];
    for (int i = 0; i < v; i++){
        nodes[i].id = i;
        nodes[i].deliverymans = 0;
    }
    for(int i=0;i<d;i++){
        int id, num;
        std::cin >> id >> num;
        nodes[id].deliverymans = num;
    }
    for(int i=0;i<e;i++){
        int src, dst, dist, ts;
        std::cin >> src >> dst >> dist >> ts;
        route r(src, dst);
        routeinfos[r] = routeinfo(dist, ts);
        nodes[src].connected_nodes.push_back(dst);
        nodes[dst].connected_nodes.push_back(src);
    }
}

myMap::~myMap(){
    if (nodes != nullptr)
        delete[] nodes;
}

void myMap::Order(int id, int src, int ts){
    order neworder;
    neworder.id = id;
    neworder.src = src;
    neworder.ts = ts;
    neworder.paths = DijsktraForOrder(src, ts);
    orders.push_back(neworder);
}

void myMap::Drop(int id, int dst){
    //
}

void myMap::Complete(int id){
    //
}
