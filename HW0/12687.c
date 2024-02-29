#include<stdio.h>
#include<stdlib.h>

typedef struct obj{
    int v;
    int w;
}OBJ;
OBJ *items;
int n, k;
int recursion(int idx, int w_left, int v){
    if(idx==n) return v;
    int a = recursion(idx+1, w_left, v);
    if(items[idx].w<=w_left){
        int b = recursion(idx+1, w_left-items[idx].w, v+items[idx].v);
        if(a>b) return a;
        else return b;
    }
    return a;
}

int main(){
    scanf("%d %d", &n, &k);
    items = (OBJ*)malloc(sizeof(OBJ)*n);
    for(int i=0; i<n; i++){
        scanf("%d %d", &items[i].v, &items[i].w);
    }
    printf("%d\n", recursion(0, k, 0));
    free(items);
    return 0;
}