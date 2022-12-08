#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void printArr(int arr[], int len, FILE* fp_out);
void insertionSort(int arr[], int p, int r);
void quickSort(int arr[],  int p, int r);
int partition(int arr[], int p, int r);
void createHeap(int heap[], int cnt);
void heapSort(int heap[], int cnt);
void heapify(int heap[], int last, int here);

void quickSort2(int arr[],  int p, int r);
int partition2(int arr[], int p, int r);


int main(int argc, char* argv[]){
    clock_t begin = clock();
    if(argc != 3){
        fprintf(stdout, "Please enter file name and index number\n");
        return -1;
    }

    FILE *fp_in = fopen(argv[1],"r");
    int len;
    fscanf(fp_in, "%d", &len);

    int* arr = (int*)malloc(sizeof(int)*len);
    for(int i = 0 ; i < len ; i++){
        fscanf(fp_in, "%d", &arr[i]);
    }
    int* arr_temp;

    if(strcmp(argv[2], "1") == 0){
        insertionSort(arr, 0, len-1);
    }
    else if(strcmp(argv[2], "2") == 0){
        quickSort(arr, 0, len-1);
    }
    else if(strcmp(argv[2], "3") == 0){

        heapSort(arr, len);

    }
    else if(strcmp(argv[2], "4") == 0){
        quickSort2(arr, 0, len-1);
    }
    else{
        fprintf(stdout, "wrong index\n");
        return -1;
    }
    clock_t end = clock();
    double time = (double)(end-begin) / CLOCKS_PER_SEC;

    char outfile_name[30];
    sprintf(outfile_name, "result_algorithm_%s", argv[1]);
    FILE *fp_out = fopen( outfile_name, "w");
    fprintf(fp_out, "%s\n", argv[1]);
    fprintf(fp_out, "%s\n", argv[2]);
    fprintf(fp_out, "%d\n", len);
    fprintf(fp_out, "%lf\n", time);
    printArr(arr, len, fp_out);

    free(arr);
    fclose(fp_in);
    fclose(fp_out);
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
            if(key < arr[j]){
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
        if(arr[r] > arr[j]){
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


void createHeap(int heap[], int cnt){ 
    for(int check = cnt / 2 - 1; check >=0 ; check--){
        heapify(heap, cnt, check);
    }
}
void heapSort(int arr[], int cnt)
{
    createHeap(arr, cnt);
    for (int i = cnt - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}
void heapify(int arr[], int cnt, int here)
{
    int largest = here;
    int l = 2 * here + 1;
    int r = 2 * here + 2;
    if (l < N && arr[l] > arr[largest])
        largest = l;
    if (r < N && arr[r] > arr[largest])
        largest = r;
    if (largest != here) {
        int temp = arr[here];
        arr[here] = arr[largest];
        arr[largest] = temp;
        heapify(arr, N, largest);
    }

}
void quickSort2(int arr[],  int p, int r){

    if(r-p <= 16){
        insertionSort(arr,p,r);
        return;
    }

    while(p < r){
        int q = partition2(arr, p, r);
        if(q - p < r - q){
            quickSort2(arr, p, q-1);
            p = q + 1;
        }
        else{
            quickSort2(arr, q+1, r);
            r = q - 1;
        }
    }
    
}

int partition2(int arr[], int p, int r){
    int m = (p + r ) / 2;
    int min, median;
    if(arr[p] <= arr[m] && arr[m] <= arr[r]){
        median = m;
    }
    else if(arr[p] <= arr[r] && arr[r] <= arr[m]){
        median = r;
    }
    else if(arr[r] <= arr[p] && arr[p] <= arr[m]){
        median = p;
    }
    else if(arr[r] <= arr[m] && arr[m] <= arr[p]){
        median = m;
    }
    else if(arr[m] <= arr[r] && arr[r] <= arr[p]){
        median = r;
    }
    else{
        median = p;
    }

    int pivot = median;
    
    int temp = arr[r];
    arr[r] = arr[pivot];
    arr[pivot] = temp;

    int i = p - 1;
    for(int j = p; j <= r-1; j++){
        if(arr[r] > arr[j]){
            i++;
            int temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }
    i++;
    temp = arr[r];
    arr[r] = arr[i];
    arr[i] = temp;
    return i;
}

