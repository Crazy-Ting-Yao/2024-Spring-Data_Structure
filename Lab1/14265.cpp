#include <iostream>
#include <string>
#include <deque>
using namespace std;

enum ItemType{
    NONE = 0,
    DIAMOND,
    GOLD,
    SHOVEL,
    VIRUS,
    COIN,
    PIG,
    STRANGE
};

class Mine{
    public:
        Mine(int col, int row);
        bool DIG(int r);
        bool USE();
        void print() const;
        ~Mine() { delete[] map; }
    private:
        deque<ItemType> *map;
        deque<ItemType> backpack;
        deque<ItemType> inventory;
        int row;
        bool has_coin = false;
        int prev_r = -1;
        bool Shovel();
        void Strange();
        void Pig();
};


int main(){
    int col, row;
    cin >> col >> row;
    int ops;
    cin >> ops;
    Mine mine(col, row);
    string command;
    while(ops--){
        cin >> command;
        if(command == "DIG"){
            int r;
            cin >> r;
            if(mine.DIG(r)) break;
        }
        else if(command == "USE"){
            if(mine.USE()) break;
        }
    }
    mine.print();
    return 0;
}

Mine::Mine(int col, int row) : row(row) {
    map = new deque<ItemType>[row];
    for(int i = 0; i < row; i++){
        for(int j = 0; j<col; j++){
            char c;
            cin>>c;
            switch(c){
                case 'D':
                    map[i].push_back(DIAMOND);
                    break;
                case 'G':
                    map[i].push_back(GOLD);
                    break;
                case 'S':
                    map[i].push_back(SHOVEL);
                    break;
                case 'V':
                    map[i].push_back(VIRUS);
                    break;
                case 'C':
                    map[i].push_back(COIN);
                    break;
                case 'P':   
                    map[i].push_back(PIG);
                    break;
                case 'X':
                    map[i].push_back(STRANGE);
                    break;
                default:
                    map[i].push_back(NONE);
                    break;
            }
        }
    }
}

bool Mine::DIG(int r){
    if(r<0 || r>=row) return false;
    auto it = map[r].begin();
    while(it!=map[r].end() && *it == NONE){
        it++;
    }
    if(it == map[r].end()) return false;
    ItemType item = *it;
    *it = NONE;
    switch(item){
        case DIAMOND:
            backpack.push_back(DIAMOND);
            break;
        case GOLD:
            backpack.push_back(GOLD);
            break;
        case SHOVEL:
            inventory.push_back(SHOVEL);
            break;
        case VIRUS:
            cout << "Game Over!" << endl;
            return true;
        case COIN:
            has_coin = true;
            break;
        case PIG:
            Pig();
            break;
        case STRANGE:
            inventory.push_back(STRANGE);
            break;
        default:
            break;
    }
    prev_r = r;
    return false;
}

bool Mine::USE(){
    if(inventory.empty()) return false;
    ItemType item = inventory.back();
    inventory.pop_back();
    switch(item){
        case SHOVEL:
            if(Shovel()) return true;
            return false;
        case STRANGE:
            Strange();
            return false;
        default:
            break;
    }
    return false;
}

void Mine::print() const{
    cout << "Backpack:";
    for(auto it = backpack.begin(); it != backpack.end(); it++){
        cout << " ";
        switch(*it){
            case DIAMOND:
                cout << "D";
                break;
            case GOLD:
                cout << "G";
                break;
            default:
                break;
        }
    }
    cout << endl;
    cout << "Inventory:";
    for(auto it = inventory.begin(); it != inventory.end(); it++){
        cout << " ";
        switch(*it){
            case SHOVEL:
                cout << "S";
                break;
            case STRANGE:
                cout << "X";
                break;
            default:
                break;
        }
    }
    cout << endl;
    cout << "Map:" << endl;
    for(int i = 0; i<row; i++){
        for(auto it = map[i].begin(); it != map[i].end(); it++){
            switch(*it){
                case DIAMOND:
                    cout << "D";
                    break;
                case GOLD:
                    cout << "G";
                    break;
                case SHOVEL:
                    cout << "S";
                    break;
                case VIRUS:
                    cout << "V";
                    break;
                case COIN:
                    cout << "C";
                    break;
                case PIG:
                    cout << "P";
                    break;
                case STRANGE:
                    cout << "X";
                    break;
                default:
                    cout << "_";
                    break;
            }
            cout << " ";
        }
        cout << endl;
    }
}

bool Mine::Shovel(){
    int r = prev_r;
    if(DIG(r)) return true;
    return DIG(r);
}

void Mine::Strange(){
    for(int i = 0; i<row; i++){
        if(i%2) map[i].push_back(GOLD);
        else map[i].push_back(DIAMOND);
    }
}

void Mine::Pig(){
    if(has_coin){
        cout<<"Ding!"<<endl;
        has_coin = false;
        return;
    }
    for(auto it = backpack.begin(); it != backpack.end(); it++){
        if(*it == GOLD){
            it = backpack.erase(it);
            it--;
        }
    }
}