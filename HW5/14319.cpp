#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <random>
struct date{
    int year;
    int month;
    int day;
    bool operator>(const date& d) const {
        if(year != d.year) return year > d.year;
        if(month != d.month) return month > d.month;
        return day > d.day;
    }
    bool operator!=(const date& d) const {
        return year != d.year || month != d.month || day != d.day;
    }
};

struct patient {
    std::string name;
    int age;
    int money;
    bool ismember;
    date membership;
    int id;
    bool operator<=(const patient& p) const {
        if(ismember != p.ismember) return p.ismember;
        if(!ismember && !p.ismember) {
            if(name == p.name) return id >= p.id;
            return name >= p.name;
        }
        if(membership != p.membership) return membership > p.membership;
        if(name == p.name) return id >= p.id;
        return name > p.name;
    }
    bool operator<(const patient& p) const {
        if(ismember != p.ismember) return p.ismember;
        if(!ismember && !p.ismember) {
            if(name == p.name) return id > p.id;
            return name > p.name;
        }
        if(membership != p.membership) return membership > p.membership;
        if(name == p.name) return id > p.id;
        return name > p.name;
    }
    bool operator>=(const patient& p) const {
        if(money == p.money) return id <= p.id;
        return money > p.money;
        
    }
    bool operator>(const patient& p) const {
        if(money == p.money) return id < p.id;
        return money > p.money;
    }
};

class maxheap{
    public:
        maxheap();
        virtual void insert(patient p) = 0;
        virtual patient pop() = 0;
        bool isEmpty() { return heap.size() == 1; }
    protected:
        std::vector<patient> heap;
};

class maxheap_panda : public maxheap{
    public:
        maxheap_panda(int n) {}
        void insert(patient p) override;
        patient pop() override;
};

class maxheap_bear : public maxheap{
    public:
        maxheap_bear(int n) {}
        void insert(patient p) override;
        patient pop() override;
};

class hospital {
    public:
        hospital(int n);
        void operate();
    private:
        int id=0;
        std::vector<bool> done;
        maxheap_panda panda;
        maxheap_bear bear;
};

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    hospital h(n);
    for(int i = 0; i < m; i++){
        std::cout << "DAY #" << i+1 << "\n"; 
        h.operate();
    }
    return 0; 
}

hospital::hospital(int n) : panda(1), bear(1){
    for(int i = 0; i < n; i++){
        patient p;
        p.id = id++;
        char t;
        getline(std::cin, p.name);
        getline(std::cin, p.name);
        std::cin >> p.age >> p.money >> p.ismember;
        if(p.ismember){
            std::cin >> p.membership.year;
            std::cin >> t;
            std::cin >> p.membership.month;
            std::cin >> t;
            std::cin >> p.membership.day;
        } 
        else {
            std::string NaN;
            std::cin>>NaN;
        }
        panda.insert(p);
        bear.insert(p);
        done.push_back(false);
    }
}

void hospital::operate(){
    int P, X, K;
    std::cin >> P >> X >> K;
    for(int i = 0; i < P; i++){
        patient p;
        p.id = id++;
        char t;
        getline(std::cin, p.name);
        getline(std::cin, p.name);
        std::cin >> p.age >> p.money >> p.ismember;
        if(p.ismember){
            std::cin >> p.membership.year;
            std::cin >> t;
            std::cin >> p.membership.month;
            std::cin >> t;
            std::cin >> p.membership.day;
        } 
        else {
            std::string NaN;
            std::cin>>NaN;
        }
        panda.insert(p);
        bear.insert(p);
        done.push_back(false);
    }
    long long money = 0;
    for(int i = 0; i < X; i++){
        if(panda.isEmpty()) break;
        patient p = panda.pop();
        if(done[p.id]) {
            i--;
            continue;
        }
        done[p.id] = true;
        money += p.money;
        std::cout << p.name << " " << p.age << "\n";
    }
    money *= 0.9;
    std::cout << "INCOME TODAY: " << money << "\n";
    for(int i = 0; i < K; i++){
        if(bear.isEmpty()) break;
        patient p = bear.pop();
        if(done[p.id]) {
            i--;
            continue;
        }
        done[p.id] = true;
    }
}

maxheap::maxheap(){
    heap.push_back(patient());
}

void maxheap_panda::insert(patient p){
    heap.push_back(p);
    int idx = heap.size()-1;
    while(idx > 1){
        if(heap[idx] <= heap[idx/2]) break;
        std::swap(heap[idx], heap[idx/2]);
        idx /= 2;
    }
}

patient maxheap_panda::pop(){
    patient ret = heap[1];
    heap[1] = heap.back();
    heap.pop_back();
    int idx = 1;
    while(2*idx < heap.size()){
        int next = 2*idx;
        if(next+1 < heap.size() && heap[next] < heap[next+1]) next++;
        if(heap[next] <= heap[idx]) break;
        std::swap(heap[idx], heap[next]);
        idx = next;
    }
    return ret;
}

void maxheap_bear::insert(patient p){
    heap.push_back(p);
    int idx = heap.size()-1;
    while(idx > 1){
        if(heap[idx/2] >= heap[idx]) break;
        std::swap(heap[idx], heap[idx/2]);
        idx /= 2;
    }
}

patient maxheap_bear::pop(){
    patient ret = heap[1];
    heap[1] = heap.back();
    heap.pop_back();
    int idx = 1;
    while(2*idx < heap.size()){
        int next = 2*idx;
        if(next+1 < heap.size() && heap[next+1] > heap[next]) next++;
        if(heap[idx] >= heap[next]) break;
        std::swap(heap[idx], heap[next]);
        idx = next;
    }
    return ret;
}