#include<iostream>
#include<cstring>
#define nullptr 0
int maxofthree(const int a, const int b, const int c){
    if(a>b){
        if(a>c) return a;
        else return c;
    }
    else{
        if(b>c) return b;
        else return c;
    }
}

enum InsertType{
    Evolve,
    Attack,
    Neutral
};

struct Node{
    int val;
    int health;
    int level;
    Node *clockwise, *counterClockwise;
};

class Arena{
    public:
        Arena();
        ~Arena();
        void insert(int val, int damage, int health, InsertType type);
        void deleteTarget();
        void reverse(int count);
        void check(int count);
        void Shuffle(char c, int count);
        void print();
    private:
        int num = 0;
        Node *target = nullptr;
        void freeNode(Node *node);
        void attack(Node *node, int damage);
        void evolve(Node *node);
        void reverse(Node *start, Node *end);
};

int main(){
    Arena arena;
    int N;
    std::cin>>N;
    char command[10];
    while(N--){
        std::cin>>command;
        if(!strcmp(command, "Insert")){
            int val, damage, health;
            char type[10];
            std::cin>>val>>damage>>health>>type;
            if(!strcmp(type, "Evolve")) arena.insert(val, damage, health, Evolve);
            else if(!strcmp(type, "Attack")) arena.insert(val, damage, health, Attack);
            else arena.insert(val, damage, health, Neutral);
        }
        else if(!strcmp(command, "Delete")){
            arena.deleteTarget();
        }
        else if(!strcmp(command, "Reverse")){
            int count;
            std::cin>>count;
            arena.reverse(count);
        }
        else if(!strcmp(command, "Shuffle")){
            char c;
            int val;
            std::cin>>c>>val;
            arena.Shuffle(c, val);
        }
        else if(!strcmp(command, "Check")){
            int count;
            std::cin>>count;
            arena.check(count);
        }
    }
    arena.print();
}

Arena::Arena() {}

Arena::~Arena(){
    for(int i = 0; i<num; i++){
        Node* temp = target;
        target = target->clockwise;
        delete temp;
    }
}

void Arena::freeNode(Node *node){
    if(node==nullptr) return;
    if(num==1){
        delete node;
        num--;
        target = nullptr;
        return;
    }
    if(target==node) target = node->clockwise;
    node->counterClockwise->clockwise = node->clockwise;
    node->clockwise->counterClockwise = node->counterClockwise;
    delete node;
    num--;
}

void Arena::insert(int val, int damage, int health, InsertType type){
    Node *newNode = new Node;
    newNode->val = val;
    newNode->health = health;
    newNode->level = 1;
    if(num==0){
        target = newNode;
        target->clockwise = target;
        target->counterClockwise = target;
        num++;
    }
    else{
        target->counterClockwise->clockwise = newNode;
        newNode->counterClockwise = target->counterClockwise;
        newNode->clockwise = target;
        target->counterClockwise = newNode;
        num++;
        if(type==Evolve) evolve(newNode);
        else if(type==Attack) attack(newNode, damage);
    }
}

void Arena::deleteTarget(){
    freeNode(target);
}

void Arena::reverse(int count){
    if(count<=1) return;
    if(count >= num){
        Node* temp = target;
        Node* end = target->counterClockwise;
        for(int i = 0; i<num; i++){
            Node* tempNext = temp->clockwise;
            temp->clockwise = temp->counterClockwise;
            temp->counterClockwise = tempNext;
            temp = tempNext;
        }
        target = end;
        return;
    }
    int t = num / count;
    Node* temp = target;
    Node* last = target->counterClockwise;
    for(int j=0;j<t;j++){
        Node* end = temp;
        for(int i = 1; i<count; i++){
            end = end->clockwise;
        }
        reverse(temp, end);
        temp = temp->clockwise;
        if(!j) target = end;
    }
    if(num%count) reverse(temp, last);
}

void Arena::reverse(Node* start, Node* end){
    if(start==end) return;
    Node* prev = start->counterClockwise;
    Node* next = end->clockwise;
    Node* temp = start;
    while(temp!=end){
        Node* tempNext = temp->clockwise;
        temp->clockwise = temp->counterClockwise;
        temp->counterClockwise = tempNext;
        temp = tempNext;
    }
    end->clockwise = end->counterClockwise;
    end->counterClockwise = prev;
    start->clockwise = next;
    prev->clockwise = end;
    next->counterClockwise = start;
}

void Arena::attack(Node *node, int damage){
    if(num == 1) return;
    if(num > 2){
        node->clockwise->health -= damage;
        if(node->clockwise->health <= 0) {
            if(target == node->clockwise) target = node;
            freeNode(node->clockwise);
        }
        node->counterClockwise->health -= damage;
        if(node->counterClockwise->health <= 0) {
            if(target == node->counterClockwise) target = node;
            freeNode(node->counterClockwise);
        }
    }
    else{
        node->clockwise->health -= damage;
        if(node->clockwise->health <= 0) {
            if(target == node->clockwise) target = node;
            freeNode(node->clockwise);
        }
    }
}

void Arena::evolve(Node *node){
    if(num < 3) return;
    if(node->clockwise->val != node->val || node->counterClockwise->val != node->val) return;
    if(node->clockwise->level == 3 || node->counterClockwise->level == 3) return;
    target = node;
    node->health = maxofthree(node->health, node->clockwise->health, node->counterClockwise->health);
    node->level = maxofthree(node->level, node->clockwise->level, node->counterClockwise->level) + 1;
    freeNode(node->clockwise);
    freeNode(node->counterClockwise);
}

void Arena::Shuffle(char c, int count){
    count %= num;
    Node* shift = target;
    if(c=='c'){
        for(int i = 0; i<count; i++){
            shift = shift->clockwise;
        }
    }
    else{
        for(int i = 0; i<count; i++){
            shift = shift->counterClockwise;
        }
    }
    Node temp;
    temp.val = target->val;
    temp.health = target->health;
    temp.level = target->level;
    target->val = shift->val;
    target->health = shift->health;
    target->level = shift->level;
    shift->val = temp.val;
    shift->health = temp.health;
    shift->level = temp.level;
    target = shift;
}

void Arena::check(int count){
    unsigned short nums[101];
    memset(nums, 0, sizeof(unsigned short)*101);
    Node* temp = target;
    for(int i = 0; i<count; i++){
        nums[temp->val] += 1;
        temp = temp->clockwise;
    }
    temp = target;
    for(int i = 0; i<count; i++){
        temp = temp->clockwise;
        if(nums[temp->counterClockwise->val] > 1){
            freeNode(temp->counterClockwise);
        }
    }
}

void Arena::print(){
    if(num==0) {
        std::cout<<"no Poke Poke ;-;"<<std::endl;
        return;
    }
    Node* temp = target;
    for(int i = 0; i<num; i++){
        std::cout<<"ID: "<<temp->val<<" HP: "<<temp->health<<" LVL: "<<temp->level<<std::endl;
        temp = temp->clockwise;
    }
}