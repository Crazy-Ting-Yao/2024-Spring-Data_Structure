#include <iostream>
#include <cstring>
#include <climits>
#define nullptr NULL
using namespace std;

class Node{
    public:
        static long long maximum_path_distance;
        static short max_path_root_val;
        Node();
        void set_distance(int distance);
        static long long find_maximum_path(Node *node);
        void add_child(Node *child);
        void set_parent(Node *parent);
        void delete_child(Node *node);
        Node* get_parent();
        short operator*(){ return this->val;}
        void operator=(int val){ this->val = val;}
    private:
        Node *first_child;
        Node *next_sibling;
        Node *parent;
        short val;
        int distance;
};

short Node::max_path_root_val = -1;
long long Node::maximum_path_distance = LLONG_MIN;
Node nodes[10001];

int main(){
    for(short i=0; i<10001; i++) nodes[i] = Node();
    short n, o;
    cin >> n >> o;
    Node *root;
    short parent_num, child_num;
    int distance;
    char operation[8];
    cin >> parent_num >> distance;
    root = &nodes[parent_num];
    *root = parent_num;
    root->set_distance(distance);
    root->set_parent(nullptr);
    for(short i=0; i<n; i++){
        cin >> parent_num >> child_num >> distance;
        nodes[child_num] = child_num;
        nodes[parent_num].add_child(&nodes[child_num]);
        nodes[child_num].set_distance(distance);
        nodes[child_num].set_parent(&nodes[parent_num]);
    }
    while(o--){
        cin >> operation;
        if(!strcmp(operation, "Add")){
            int parent_num, child_num, distance;
            cin >> parent_num >> child_num >> distance;
            nodes[child_num] = child_num;
            nodes[parent_num].add_child(&nodes[child_num]);
            nodes[child_num].set_distance(distance);
            nodes[child_num].set_parent(&nodes[parent_num]);
        }
        else if(!strcmp(operation, "Delete")){
            int child_num;
            cin >> child_num;
            if(*nodes[child_num]==-1) continue;
            nodes[child_num].get_parent()->delete_child(&nodes[child_num]);
        }
        else if(!strcmp(operation, "Check")){
            Node::max_path_root_val = -1;
            Node::maximum_path_distance = INT_MIN;
            Node::find_maximum_path(root);
            cout << "Maximum Value: " << Node::maximum_path_distance << endl;
            cout << "Root of the Path: " << Node::max_path_root_val << endl;
        }
    }
    Node::max_path_root_val = -1;
    Node::maximum_path_distance = INT_MIN;
    Node::find_maximum_path(root);
    cout << "Final Root: " << Node::max_path_root_val << endl;
    return 0;
}

Node::Node(): val(-1), distance(INT_MIN){
    this->first_child = nullptr;
    this->next_sibling = nullptr;
    this->parent = nullptr;
}

void Node::set_distance(int distance){
    this->distance = distance;
}

Node* Node::get_parent(){
    return this->parent;
}

void Node::add_child(Node *child){
    if(this->first_child==nullptr){
        this->first_child = child;
    }
    else{
        child->next_sibling = this->first_child;
        this->first_child = child;
    }
}

void Node::set_parent(Node *parent){
    this->parent = parent;
}

void Node::delete_child(Node *node){
    if(node->first_child!=nullptr){
        Node* current = node->first_child;
        while(current->next_sibling!=nullptr){
            current->set_parent(this);
            current = current->next_sibling;
        }
        current->set_parent(this);
        if(this->first_child==nullptr){
            this->first_child = node->first_child;
        }
        else{
            current->next_sibling = this->first_child;
            this->first_child = node->first_child;
        }
    }
    if(this->first_child==node){
        this->first_child = this->first_child->next_sibling;
        return;
    }
    else{
        Node *current = this->first_child;
        while(current->next_sibling!=node){
            current = current->next_sibling;
        }
        current->next_sibling = node->next_sibling;
    }
    node->val = -1;
    node->distance = INT_MIN;
    node->first_child = nullptr;
    node->next_sibling = nullptr;
    node->parent = nullptr;
}

long long Node::find_maximum_path(Node *node){
    long long max_dis, second_max_dis;
    max_dis = second_max_dis = INT_MIN;
    if(node->first_child==nullptr){
        if(node->distance>=Node::maximum_path_distance){
            Node::maximum_path_distance = node->distance;
            Node::max_path_root_val = node->val;
        }
        return node->distance;
    }
    Node *current = node->first_child;
    long long dis;
    while(current!=nullptr){
        dis = find_maximum_path(current);
        if(dis>max_dis){
            second_max_dis = max_dis;
            max_dis = dis;
        }
        else if(dis>second_max_dis){
            second_max_dis = dis;
        }
        current = current->next_sibling;
    }
    if(max_dis+second_max_dis+node->distance>Node::maximum_path_distance){
        Node::maximum_path_distance = max_dis+second_max_dis+node->distance;
        Node::max_path_root_val = node->val;
    }
    if(max_dis+node->distance>Node::maximum_path_distance){
        Node::maximum_path_distance = max_dis+node->distance;
        Node::max_path_root_val = node->val;
    }
    if(node->distance>Node::maximum_path_distance){
        Node::maximum_path_distance = node->distance;
        Node::max_path_root_val = node->val;
    }
    return (max_dis>0)?max_dis+node->distance:node->distance;
}