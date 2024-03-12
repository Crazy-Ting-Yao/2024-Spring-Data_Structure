#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
typedef struct{
    int v;
    int w;
}Item;
Item *items;
int min(int a, int b){
    return a<b?a:b;
}
int N, k;
int minw = 1e9;
void recursion(int idx, int total_w, int total_v){
    if(idx==N) return;
    if(total_v + items[idx].v >= k){
        minw = min(total_w+items[idx].w, minw);
        recursion(idx+1, total_w, total_v);
        return;
    }
    recursion(idx+1, total_w, total_v);
    recursion(idx+1, total_w+items[idx].w, total_v+items[idx].v);
}

int main(){
    scanf("%d %d",&N,&k);
    items = (Item *)malloc(sizeof(Item)*N);
    for(int i=0;i<N;i++){
        scanf("%d %d",&items[i].v,&items[i].w);
    }
    recursion(0,0,0);
    printf("%d\n",minw);
    free(items);
    return 0;
}