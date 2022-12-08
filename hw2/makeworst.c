#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void printArr(int arr[], int len, FILE* fp_out);
void insertionSort(int arr[], int p, int r);
void quickSort(int arr[],  int p, int r);
int partition(int arr[], int p, int r);
void createMinHeap(int minheap[], int cnt);
int* minHeapSort(int minheap[], int cnt);
void minHeapify(int minheap[], int last, int here);

void quickSort2(int arr[],  int p, int r);
int partition2(int arr[], int p, int r);


int main(int argc, char* argv[]){
    FILE* fp_in = fopen("test_100000", "r");
    int len;
    fscanf(fp_in, "%d", &len);
    char filename[30];
    sprintf(filename, "worst_%d", len);
    FILE* fp_out = fopen(filename, "w");
    int* arr = (int*)malloc(sizeof(int)*len);
    for(int i = 0 ; i < len ; i++){
        fscanf(fp_in, "%d", &arr[i]);
    }
    insertionSort(arr,0,len-1);
    fprintf(fp_out, "%d ", len);
    printArr(arr, len, fp_out);

    printf("make worst end\n");
    return 0;
}

void printArr(int arr[], int len, FILE *fp_out){
    for(int i = 0; i < len ;i++){
        fprintf(fp_out, "%d ", arr[i]);
    }
    fprintf(fp_out, "\n");
}

void insertionSort(int arr[], int p, int r){
    for(int i = p+1 ; i <= r ; i++){
        int key = arr[i];
        int j = i - 1;
        for(; j >= p; j--){
            if(key > arr[j]){
                arr[j+1] = arr[j];
            }
            else break;
        }
        arr[j+1] = key;
    }
}

void quickSort(int arr[],  int p, int r){
    if(p < r){
        int q = partition(arr, p, r);
        quickSort(arr, p, q-1);
        quickSort(arr, q+1, r);
    }
}

int partition(int arr[], int p, int r){
    int i = p - 1;
    for(int j = p; j <= r-1; j++){
        if(arr[r] < arr[j]){
            i++;
            int temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }
    i++;
    int temp = arr[r];
    arr[r] = arr[i];
    arr[i] = temp;
    return i;
}
void createMinHeap(int minheap[], int cnt){ 
    for(int check = cnt / 2; check >=0 ; check--){
        minHeapify(minheap, cnt, check);
    }
}
int* minHeapSort(int minheap[], int cnt){
    int last = cnt;
    createMinHeap(minheap, cnt);
    int* result_arr = (int*)malloc(sizeof(int)*cnt);
    int i = 0;
    while(last > 0){
        int pop_root = minheap[1];
        result_arr[i++] = pop_root;
        minheap[1] = minheap[last];
        minheap[last--] = -1;
        minHeapify(minheap, last, 1);
    }
    return result_arr;
}
void minHeapify(int minheap[], int last, int here){    //check heap and if it's invalid, correct it
    if(here > last / 2) return;
    int lc = here * 2;
    int rc = here * 2 + 1;
    int smaller_child;
    if(minheap[rc] == -1)  smaller_child = lc;
    else{
        if(minheap[lc] < minheap[rc])   smaller_child = lc;
        else smaller_child = rc;
    } 

    if(minheap[here] > minheap[smaller_child]){
        int temp = minheap[here];
        minheap[here] = minheap[smaller_child];
        minheap[smaller_child] = temp;
        here = smaller_child;
        minHeapify(minheap, last, 1);
    }
    else return;   //if here node is bigger than its children, no longer need to check the validation of heap. at this point it's all valid
}
void quickSort2(int arr[],  int p, int r){
    while(p < r && r - p > 16){
        int q = partition2(arr, p, r);
        quickSort2(arr, p, q);
        p = q + 1;
    }
    insertionSort(arr, p, r);
}

int partition2(int arr[], int p, int r){
    int m = (p + r ) / 2;
    int min = arr[p];
    int pivot = p;
    if(min > arr[m]) {
        min = arr[m];
        pivot = m;
    }
    if(min > arr[r]){
        min = arr[r];
        pivot = r;
    }
    int temp = arr[r];
    arr[r] = arr[pivot];
    arr[pivot] = temp;
    pivot = p - 1;
    for(int j = p; j <= r-1; j++){
        if(arr[r] < arr[j]){
            pivot++;
            int temp = arr[j];
            arr[j] = arr[pivot];
            arr[pivot] = temp;
        }
    }
    pivot++;
    arr[r] = arr[pivot];
    arr[pivot] = temp;
    return pivot;
}

