#include<list>
#include<iostream>
#include<cstdio>
#include<string>
using namespace std;

struct Node {
    int ID;
    int level;
    bool const operator<(const Node &node) const{
        if(level == node.level){
            return ID < node.ID;
        }
        return level < node.level;
    }
};

class Gym{
    public:
        Gym(int n);
        ~Gym();
        void insert(int index, int ID, int level);
        void remove(int index, int ID);
        void rotate(int index, int times);
        void reorder(int index);
        void reverse(int index, int start, int end);
        void mergeListsPreserveOrder(int index1, int index2);
        void print();
    private:
        list<Node> *gym;
        int size;
};

int main(){
    int n;
    cin >> n;
    Gym gym(n);
    string command;
    int t;
    cin >> t;
    while(t--){
        cin >> command;
        if(command == "Insert"){
            int index, ID, level;
            cin >> index >> ID >> level;
            gym.insert(index, ID, level);
        }
        else if(command == "Remove"){
            int index, ID;
            cin >> index >> ID;
            gym.remove(index, ID);
        }
        else if(command == "Rotate"){
            int index, times;
            cin >> index >> times;
            gym.rotate(index, times);
        }
        else if(command == "Reorder"){
            int index;
            cin >> index;
            gym.reorder(index);
        }
        else if(command == "Reverse"){
            int index, start, end;
            cin >> index >> start >> end;
            gym.reverse(index, start, end);
        }
        else if(command == "MergeListsPreserveOrder"){
            int index1, index2;
            cin >> index1 >> index2;
            gym.mergeListsPreserveOrder(index1, index2);
        }
    }
    gym.print();
}

Gym::Gym(int n){
    size = n;
    gym = new list<Node>[size];
}

Gym::~Gym(){
    delete[] gym;
}

void Gym::insert(int index, int ID, int level){
    Node node;
    node.ID = ID;
    node.level = level;
    gym[index].push_back(node);
}

void Gym::remove(int index, int ID){
    list<Node>::iterator it = gym[index].begin();
    it++;
    for(;it != gym[index].end(); it++){
        if(it->ID == ID){
            it = gym[index].erase(it);
            it--;
        }
    }
    if(gym[index].front().ID == ID){
        gym[index].pop_front();
    }
}

void Gym::rotate(int index, int times){
    for(int i = 0; i < times; i++){
        Node node = *gym[index].rbegin();
        gym[index].pop_back();
        gym[index].push_front(node);
    }
}

void Gym::reorder(int index){
    list<Node>::iterator it = gym[index].begin();
    list<Node> temp[2];
    for(int i=0; i < gym[index].size(); i++){
        if(i % 2 == 0){
            temp[0].push_back(*it);
        }
        else{
            temp[1].push_back(*it);
        }
        it++;
    }
    gym[index].clear();
    gym[index] = temp[0];
    gym[index].insert(gym[index].end(), temp[1].begin(), temp[1].end());
}

void Gym::reverse(int index, int start, int end){
    list<Node>::iterator it = gym[index].begin();
    list<Node> temp;
    for(int i = 0; i < start; i++){
        it++;
    }
    for(int i = start; i <= end; i++){
        temp.push_front(*it);
        it = gym[index].erase(it);
    }
    gym[index].insert(it, temp.begin(), temp.end());
}

void Gym::mergeListsPreserveOrder(int index1, int index2){
    list<Node>::iterator it1 = gym[index1].begin();
    list<Node>::iterator it2 = gym[index2].begin();
    list<Node> temp;
    while(it1 != gym[index1].end() && it2 != gym[index2].end()){
        if(*it1 < *it2){
            temp.push_back(*it1);
            it1++;
        }
        else{
            temp.push_back(*it2);
            it2++;
        }
    }
    if(it1 == gym[index1].end()){
        temp.insert(temp.end(), it2, gym[index2].end());
    }
    else{
        temp.insert(temp.end(), it1, gym[index1].end());
    }
    gym[index1].clear();
    gym[index1].merge(temp);
    gym[index2].clear();
}

void Gym::print(){
    for(int i = 0; i < size; i++){
        cout << "List " << i << endl;
        if(gym[i].empty()){
            cout << "Empty" << endl;
            continue;
        }
        for(list<Node>::iterator it = gym[i].begin(); it != gym[i].end(); it++){
            cout << "ID: " << it->ID << " Level: " << it->level << endl;
        }
    }
}