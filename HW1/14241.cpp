#include <cstdlib>
#include <cstdio>
#include <cstring>
enum ItemType{
    NONE = 0,
    GOLD,
    DIAMOND,
    BOMB,
    FLASHLIGHT,
    MAGNET,
    LUCKY_CLOVER,
    PIG
};

typedef struct item{
    ItemType type;
    struct item *next;
}Item;

class My_queue{
    public:
    My_queue(){
        head = NULL;
        tail = NULL;
    }
    ~My_queue(){
        while(head != NULL){
            Item *temp = head;
            head = head->next;
            delete temp;
        }
    }
    void push(ItemType type){
        Item *new_item = new Item;
        new_item->type = type;
        new_item->next = NULL;
        if(head == NULL){
            head = new_item;
            tail = new_item;
        }
        else{
            tail->next = new_item;
            tail = new_item;
        }
    }
    ItemType pop(){
        if(head == NULL){
            return NONE;
        }
        Item *temp = head;
        ItemType type = head->type;
        head = head->next;
        delete temp;
        return type;
    }
    private:
    Item *head;
    Item *tail;
};

class My_stack{
    public:
    My_stack(){
        head = NULL;
        size = 0;
    }
    ~My_stack(){
        while(head != NULL){
            Item *temp = head;
            head = head->next;
            delete temp;
        }
    }
    void push(ItemType type){
        Item *new_item = new Item;
        new_item->type = type;
        new_item->next = head;
        head = new_item;
        size++;
    }
    ItemType top(){
        if(head == NULL){
            return NONE;
        }
        return head->type;
    }
    ItemType pop(){
        if(head == NULL){
            return NONE;
        }
        Item *temp = head;
        ItemType type = head->type;
        head = head->next;
        delete temp;
        size--;
        return type;
    }
    void print(){
        ItemType *array = new ItemType[size];
        Item* temp = head;
        for(int i = size-1; i >= 0; i--){
            array[i] = temp->type;
            temp = temp->next;
        }
        for(int i = 0; i < size; i++){
            ItemType type = array[i];
            switch (type){
            case GOLD:
                printf("G ");
                break;
            case DIAMOND:
                printf("D ");
                break;
            default:
                break;
            }
        }
        printf("\n");
        delete[] array;
    }
    private:
    Item *head;
    int size;
};

class My_array{
    public:
    My_array(){
        size = 0;
        array = NULL;
    }
    ~My_array(){
        if(array!=NULL) delete[] array;
    }
    void resize(int new_size){
        if(new_size < 0 || new_size == size){
            return;
        }
        ItemType *new_array = new ItemType[new_size];
        if(array != NULL){
            if(new_size < size){
                for(int i = 0; i < new_size; i++){
                    new_array[i] = array[i];
                }
            }
            else {
                int i = 1;
                for(; i < size; i++){
                    new_array[i] = array[i];
                }
                for(; i < new_size; i++){
                    new_array[i] = NONE;
                }
            }
            delete[] array;
        }
        array = new_array;
        size = new_size;
    }
    ItemType operator[](int index){
        if(index < 0 || index >= size){
            return NONE;
        }
        return array[index];
    }
    void edit(int index, ItemType type){
        if(index < 0 || index >= size){
            return;
        }
        array[index] = type;
    }
    private:
    ItemType *array;
    int size;
};

class Mine{
    public:
        Mine(int col, int level){
            total_col = col;
            total_level = level;
            map = new My_array[col];
            for(int i = 0; i < col; i++){
                map[i].resize(level+1);
            }
            for(int i=level;i>0;i--){
                for(int j=0;j<col;j++){
                    char temp;
                    scanf(" %c", &temp);
                    ItemType type = NONE;
                    switch (temp) {
                    case 'G': type = GOLD; break;
                    case 'D': type = DIAMOND; break;
                    case 'B': type = BOMB; break;
                    case 'F': type = FLASHLIGHT; break;
                    case 'M': type = MAGNET; break;
                    case 'C': type = LUCKY_CLOVER; break;
                    case 'P': type = PIG; break;
                    default: type = NONE; break;
                    }
                    map[j].edit(i, type);
                }
            }
            Level_resize();
        }
        ~Mine(){
            if(map) delete[] map;
        }
        void DIG(int col){
            ItemType type;
            int l;
            for(l = total_level; l > 0; l--){
                type = map[col][l];
                if(type != NONE){
                    break;
                }
            }
            switch (type){
            case GOLD:
                bag.push(GOLD);
                break;
            case DIAMOND:
                bag.push(DIAMOND);
                break;
            case BOMB:
                Bomb(col, l);
                break;
            case FLASHLIGHT:
                inventory.push(FLASHLIGHT);
                break;
            case MAGNET:
                inventory.push(MAGNET);
                break;
            case LUCKY_CLOVER:
                Lucky_clover(col);
                break;
            case PIG:
                Pig();
                break;
            default:
                break;
            }
            map[col].edit(l, NONE);
            Level_resize();
        }
        void USE(){
            ItemType type = inventory.pop();
            switch (type){
            case FLASHLIGHT:
                Flashlight();
                break;
            case MAGNET:
                Magnet();
                break;
            default:
                break;
            }
        }
        void Print(){
            printf("FINAL BAG:\n");
            bag.print();
            printf("FINAL MAP:\n");
            //printf("MINE LEVEL:%d\n", total_level);
            for(int i=total_level;i>0;i--){
                for(int j=0; j<total_col; j++){
                    ItemType type = map[j][i];
                    switch (type){
                    case GOLD:
                        printf("G ");
                        break;
                    case DIAMOND:
                        printf("D ");
                        break;
                    case BOMB:
                        printf("B ");
                        break;
                    case FLASHLIGHT:
                        printf("F ");
                        break;
                    case MAGNET:
                        printf("M ");
                        break;
                    case LUCKY_CLOVER:
                        printf("C ");
                        break;
                    case PIG:
                        printf("P ");
                        break;
                    default:
                        printf("_ ");
                        break;
                    }
                }
                printf("\n");
            }
        }
    private:
    int total_col;
    int total_level;
    My_array *map;
    My_queue inventory;
    My_stack bag;
    void Bomb(int col, int level){
        for(int i=col-1;i<=col+1;i++){
            for(int j=level-1;j<=level+1;j++){
                if(i>=0 && i<total_col && j>=0 && j<=total_level){
                    map[i].edit(j, NONE);
                }
            }
        }
    }
    void Flashlight(){
        if(total_level){
            printf("MINE LEVEL:%d\n", total_level);
            for(int k = 0; k<total_col; k++){
                ItemType type = map[k][total_level];
                switch (type){
                case GOLD:
                    printf("G ");
                    break;
                case DIAMOND:
                    printf("D ");
                    break;
                case BOMB:
                    printf("B ");
                    break;
                case FLASHLIGHT:
                    printf("F ");
                    break;
                case MAGNET:
                    printf("M ");
                    break;
                case LUCKY_CLOVER:
                    printf("C ");
                    break;
                case PIG:
                    printf("P ");
                    break;
                default:
                    printf("_ ");
                    break;
                }
            }
        }
        else{
            printf("MINE LEVEL:1\n");
            for(int i=0;i<total_col;i++){
                printf("_ ");
            }
        }
        printf("\n");
    }
    void Magnet(){
        for(int i = 0; i < total_col; i++){
            DIG(i);
        }
    }
    void Lucky_clover(int col){
        ItemType top = bag.top();
        if(top==NONE) return;
        int max = 0;
        for(int i = col - 2; i <= col + 2; i++){
            if(i<0 || i>=total_col) continue;
            for(int l = total_level; l > 0; l--){
                if(map[i][l] != NONE){
                    max = max<l ? l : max;
                    break;
                }
            }
        }
        if(max+3 > total_level){
            for(int i = 0; i<total_col;i++){
                map[i].resize(max+4);
            }
            total_level = max+3;
        }
        for(int i = col - 2; i <= col + 2; i++){
            if(i<0 || i>=total_col) continue;
            for(int l = max+1; l <= max+3; l++){
                map[i].edit(l, top);
            }
        }
    }
    void Pig(){
        ItemType type;
        do{
            type = bag.pop();
            if(type == DIAMOND){
                bag.push(type);
                break; 
            }
        }while(bag.top() != NONE);
    }
    void Level_resize(){
        int m;
        for(m = total_level; m > 0; m--){
            int flag = 0;
            for(int i = 0; i < total_col; i++){
                if(map[i][m] != NONE){
                    flag = 1;
                    break;
                }
            }
            if(flag) break;
        }
        for(int i = 0; i < total_col; i++){
            map[i].resize(m+1);
            
        }
        total_level = m;
    }
};

int main(){
    int r, l, n;
    scanf("%d%d%d", &r, &l, &n);
    Mine mine(r,l);
    for(int i=1;i<=n;i++){
        char action[4];
        scanf(" %s", action);
        if(!strcmp(action, "DIG")){
            int col;
            scanf("%d", &col);
            mine.DIG(col);
        }
        else mine.USE();
    }
    mine.Print();
}