#include <stdio.h>
#include <stdlib.h>

typedef struct Heap {
    int *array;
    int capacity;
    int size;
}Heap;



Heap* createHeap(int capacity){
    Heap* h = (Heap*)malloc(sizeof(Heap));
    // if == NULL;

    h->capacity = capacity;
    h->size = 0;
    h->array = (int*)malloc(capacity * sizeof(int));
    //if h-array == NULL

    return h;
}

void push(Heap* h, int value)
{
    if ( h->size >= h->capacity){
        printf("Heap is full!\n");
        return;
    } 

    h->array[h->size] =value;
    int i = h->size;
    h->size++;

    while (i != 0 && h->array[(i-1)/2] > h->array[i])
    {
        int temp = h->array[(i-1)/2];
        h->array[(i-1)/2] = h->array[i];
        h->array[i] = temp;
        i = ((i-1)/2);
    }
    return;
}

int pop(Heap* h){
    //if size <= 0

    int root = h->array[0];
    h->array[0] = h->array[--h->size];

    int i = 0;
    while (2*i +1 < h->size)
    {
        int left = 2* i + 1;
        int right = 2* i + 2;
        int smallest = i;
        if(left < h->size &&  h->array[left] < h->array[i]) smallest = left;
        if( right < h->size && h->array[right] < h->array[left]) smallest = right;
        if (smallest != i){
            int temp = h->array[i];
            h->array[i] = h->array[smallest];
            h->array[smallest] = temp;
        }
        else break;
    }
    
    return root;

}



int main(int argc, char const *argv[])
{

    return 0;
}
