#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
using namespace std;
struct Node{
    int value;
    Node* left = NULL;
    Node* right = NULL;
};
int n;
Node* trees[20];

void search_insert(Node* root, int parent_value, int value){
    if(root == NULL){
        return;
    }
    if(root->value == parent_value){
        if(root->left == NULL){
            root->left = new Node;
            root->left->value = value;
        }
        else if(root->right == NULL){
            root->right = new Node;
            root->right->value = value;
        }
        return;
    }
    search_insert(root->left, parent_value, value);
    search_insert(root->right, parent_value, value);
}

void insert(int index, int parent_value, int value){
    if(trees[index] == NULL){
        trees[index] = new Node;
        trees[index]->value = value;
        return;
    }
    search_insert(trees[index], parent_value, value);
}

void delete_node(Node* root){
    if(root == NULL){
        return;
    }
    if(root->left != NULL){
        delete_node(root->left);
    }
    if(root->right != NULL){
        delete_node(root->right);
    }
    delete root;
}

void del(Node* root, int value){
    if(root == NULL){
        return;
    }
    if(root->left){
        if(root->left->value == value){
            delete_node(root->left);
            root->left = NULL;
            return;
        }
        del(root->left, value);
    }
    if(root->right){ 
        if(root->right->value == value){
            delete_node(root->right);
            root->right = NULL;
            return;
        }
        del(root->right, value);
    }
}

void preorder(Node* node){
    if(node == NULL){
        return;
    }
    cout << node->value << " ";
    preorder(node->left);
    preorder(node->right);
}

void inorder(Node* node){
    if(node == NULL){
        return;
    }
    inorder(node->left);
    cout << node->value << " ";
    inorder(node->right);
}

void postorder(Node* node){
    if(node == NULL){
        return;
    }
    postorder(node->left);
    postorder(node->right);
    cout << node->value << " ";
}

int max(Node* node){
    if(node == NULL){
        return -1;
    }
    int left = max(node->left);
    int right = max(node->right);
    return node->value > left ? (node->value > right ? node->value : right) : (left > right ? left : right);
}

void Merge(int dest, int from, int val){
    Node* destnode = trees[dest];
    Node* fromnode = trees[from];
    Node* newnode = new Node;
    newnode->value = val;
    trees[dest] = newnode;
    newnode->left = destnode;
    newnode->right = fromnode;
    trees[from] = NULL;
}

Node* findnode(Node* root, int value){
    if(root == NULL){
        return NULL;
    }
    if(root->left){
        if(root->left->value == value){
            return root;
        }
        Node* left = findnode(root->left, value);
        if(left != NULL){
            return left;
        }
    }
    if(root->right){
        if(root->right->value == value){
            return root;
        }
        Node* right = findnode(root->right, value);
        if(right != NULL){
            return right;
        }
    }
    return NULL;
}

void Disjoint(int index, int value){
    if(trees[index] == NULL){
        return;
    }
    if(trees[index]->value == value){
        Node *tmp = trees[index];
        trees[index] = NULL;
        for(int i = 0; i < 20; i++){
            if(trees[i] == NULL){
                trees[i] = tmp;
                if(i>=n) n = i+1;
                return;
            }
        }
    }
    Node* parent = findnode(trees[index], value);
    if(parent == NULL){
        return;
    }
    if(parent->left && parent->left->value == value){
        Node* tmp = parent->left;
        parent->left = NULL;
        for(int i = 0; i < 20; i++){
            if(trees[i] == NULL){
                trees[i] = tmp;
                if(i>=n) n = i+1;
                return;
            }
        }
    }
    else if(parent->right && parent->right->value == value){
        Node* tmp = parent->right;
        parent->right = NULL;
        for(int i = 0; i < 20; i++){
            if(trees[i] == NULL){
                trees[i] = tmp;
                if(i>=n) n = i+1;
                return;
            }
        }
    }
}

int main(){
    int ops;
    cin >> n >> ops;
    for(int i = 0; i < 20; i++){
        trees[i] = NULL;
    }
    while(ops--){
        string op;
        cin >> op;
        if(op == "Insert"){
            int index, parent_value, value;
            cin >> index >> parent_value >> value;
            insert(index, parent_value, value);
        }
        else if(op == "Delete"){
            int index, value;
            cin >> index >> value;
            if(trees[index] != NULL){
                if(trees[index]->value == value){
                    delete_node(trees[index]);
                    trees[index] = NULL;
                }
                else del(trees[index], value);
            }
        }
        else if(op == "Print"){
            int index;
            string order;
            cin >> index >> order;
            if(order == "pre"){
                preorder(trees[index]);
                cout << endl;
            }
            else if(order == "in"){
                inorder(trees[index]);
                cout << endl;
            }
            else if(order == "post"){
                postorder(trees[index]);
                cout << endl;
            }
        }
        else if(op == "Max"){
            int index;
            cin >> index;
            if(trees[index] != NULL) cout << max(trees[index]) << endl;
        }
        else if(op == "Merge"){
            int dest, from, val;
            cin >> dest >> from >> val;
            Merge(dest, from, val);
        }
        else if(op == "Disjoint"){
            int index, value;
            cin >> index >> value;
            Disjoint(index, value);
        }
    }
    for(int i=0; i<n; i++){
        cout << "Tree " << i << endl;
        inorder(trees[i]);
        cout << endl;
    }
    return 0;
}