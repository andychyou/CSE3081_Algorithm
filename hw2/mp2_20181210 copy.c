#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printArr(int arr[], int len);
void insertionSort(int arr[], int len);
void quickSort(int arr[],  int p, int r);
int partition(int arr[], int p, int r);
void countSort(int arr[], int len);
void getMinMax(int arr[], int len, int* min, int* max);

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Please enter file name and index number\n");
        return -1;
    }

    FILE *fp_in = fopen(argv[1],"r");

    int len;
    fscanf(fp_in, "%d", &len);

    int* arr = (int*)malloc(sizeof(int)*len);
    for(int i = 0 ; i < len ; i++){
        fscanf(fp_in, "%d", &arr[i]);
    }
    
    countSort(arr, len);
    printArr(arr, len);
    return 0;
}

void printArr(int arr[], int len){
    for(int i = 0; i < len ;i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void insertionSort(int arr[], int len){
    for(int i = 1 ; i < len ; i++){
        int key = arr[i];
        for(int j = i - 1; j >= 0; j--){
            if(key < arr[j]){
                arr[j+1] = arr[j];
            }
            else{
                arr[j+1] = key;
                break;
            }
        }
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

void countSort(int arr[], int len){
    int min,max;
    getMinMax(arr, len, &min, &max);
    max = max - min;
    int* buckets = (int*)malloc(sizeof(int)*(max+1));
    for(int i = 0; i < max+1; i++){
        buckets[i] = 0;
    }

    for(int i = 0; i < len; i++){
        buckets[arr[i] - min]++;
    }


    int idx = 0;
    for(int i = 0; i < max+1; i++){
        while(buckets[i] > 0){
            arr[idx] = i + min;
            idx++;
            buckets[i]--;
        }
    }
}

void getMinMax(int arr[], int len, int* min, int* max){
    *min = *max = arr[0];
    for(int i = 1; i < len; i+=2){

        if(i == len - 1){
            if(arr[i] > *max){
                *max = arr[i];
            }
            if(arr[i] < *min){
                *min = arr[i];
            }
            break;
        }

        if(arr[i] < arr[i+1]){
            if(arr[i] < *min){
                *min = arr[i];
            }
            if(arr[i+1] > *max){
                *max = arr[i+1];
            }
        }
        else{
            if(arr[i] > *max){
                *max = arr[i];
            }
            if(arr[i+1] < *min){
                *min = arr[i+1];
            }
        }
    }
}