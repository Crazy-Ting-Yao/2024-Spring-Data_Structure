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
    routeinfo(const routeinfo& ri){
        dist = ri.dist;
        ts = ri.ts;
    }
};

struct path{
    int src;
    int dest;
    int dist;
    std::vector<int> via;
    path(int s, int d, int di){
        src = s;
        dest = d;
        dist = di;
    }
    path(const path& p){
        src = p.src;
        dest = p.dest;
        dist = p.dist;
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
        node *nodes;
        std::map<route, routeinfo> routeinfos;
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
        route newroute(src, dst);
        routeinfos.insert(std::pair<route, routeinfo>(newroute, routeinfo(dist, ts)));
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
    if(nodes[src].deliverymans > 0){
        neworder.ts = ts;
        neworder.paths = nullptr;
        orders[id] = neworder;
        nodes[src].deliverymans--;
        return;
    }
    myPriorityQueue pq;
    path start(src, src, 0);
    pq.push(start);
    bool *passed = new bool[nodes_count];
    for(int i=0;i<nodes_count;i++)
        passed[i] = false;
    passed[src] = true;
    while(!pq.isempty()){
        path current = pq.pop();
        if(nodes[current.dest].deliverymans > 0){
            order neworder;
            neworder.ts = ts;
            neworder.paths = new path(current);
            orders[id] = neworder;
            nodes[current.dest].deliverymans--;
            update_ts(current.src, current.via, ts * -1);
            delete[] passed;
            std::cout << "Order " << id << " from: " << current.dest << std::endl;
            return;
        }
        for(auto node : nodes[current.dest].connected_nodes){
            if(passed[node]) continue;
            passed[node] = true;
            route currentroute(current.dest, node);
            routeinfo currentrouteinfo(routeinfos.find(currentroute)->second);
            if(currentrouteinfo.ts >= ts){
                path newpath(current);
                newpath.dest = node;
                newpath.dist += currentrouteinfo.dist;
                newpath.via.push_back(current.dest);
                pq.push(newpath);
                
            }
        }
    }
    delete[] passed;
    std::cout << "Just walk. T-T" << std::endl;
}

void myMap::Drop(int id, int dst){

}

void myMap::Complete(int id){

}

void myMap::update_ts(int src, std::vector<int> via, int ts){
    int from = src;
    for(int i=0;i<via.size();i++){
        route newroute(from, via[i]);
        routeinfos.find(newroute)->second.ts += ts;
        from = via[i];
    }
}