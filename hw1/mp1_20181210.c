
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **readMatrix(FILE *fp, int **matrix);
void printMatrix(int **matrix);
int bruteForce(int** matrix);  //n^6
int quartic(int** matrix); //n^4
int cubic(int** matrix);   //n^3
int* kadane(int *arr, int* max);

int r, c, largestE;
clock_t start, end;
double operation;

int main(int argc, char* argv[])
{
    FILE *f_in = NULL;
    f_in = fopen(argv[1], "r");
    if (f_in == NULL)
    {
        printf("Input file doesn't exist\n");
        return -1;
    }

    if(argc != 3){
        printf("invalid number of arguments\n");
        return -1;
    }

    int **matrix = NULL;
    matrix = readMatrix(f_in, matrix);
    fclose(f_in);
    int maxSum;
    if(*argv[2] == '1'){
        start = clock();
        maxSum = bruteForce(matrix);
        end = clock();
        operation = (double)(end - start) ;
    }

    else if(*argv[2] == '2'){
        start = clock();
        maxSum = quartic(matrix);
        end = clock();
        operation = (double)(end - start);
    }

    else if(*argv[2] == '3'){
        start = clock();
        maxSum = cubic(matrix);
        end = clock();
        operation = (double)(end - start) ;
    }

    else{
        printf("invalid algorithm index %s\n", argv[2]);
        return -1;
    }
    
    char filename[20];
    sprintf(filename, "result_%s", argv[1]);
    FILE *f_out = fopen(filename, "w");
    fprintf(f_out,"%s\n", argv[1]);
    fprintf(f_out,"%s\n", argv[2]);
    fprintf(f_out,"%d\n%d\n", r,c);
    fprintf(f_out,"%d\n", maxSum);
    fprintf(f_out,"%.3lf\n", operation);

    return 0;

}



int **readMatrix(FILE *fp, int **matrix)
{
    fscanf(fp, "%d %d", &r, &c);
    matrix = (int **)malloc(sizeof(int *) * r);
    for (int i = 0; i < r; i++)
    {
        matrix[i] = (int *)malloc(sizeof(int) * c);
    }
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            fscanf(fp, "%d", &matrix[i][j]);
        }
    }

    return matrix;
}

void printMatrix(int **matrix)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int bruteForce(int** matrix)
{


    int sum;
    int maxSum = matrix[0][0];
    largestE = matrix[0][0];
    for (int fi = 0; fi < r; fi++)
    {
        for (int fj = 0; fj < c; fj++)//choose point 1
        {
            for(int si = 0; si < r; si++){
                for(int sj = 0; sj < c; sj++){//choose point 2
                    sum = 0;
                    for(int i = fi; i <= si ; i++){
                        for(int j = fj; j <= sj; j++){//calculate the sum 
                            if(largestE < matrix[i][j])
                                largestE = matrix[i][j];
                            sum += matrix[i][j];
                        }
                    }
                    if(sum > maxSum) maxSum = sum;
                }
            }
        }
    }

    if(largestE < 0){
        return largestE;
    }
    else{
        return maxSum;
    }
}

int quartic(int** matrix){
    int maxSum, currMax;
    int* buffer = (int*)malloc(sizeof(int)*c);
    largestE = matrix[0][0];
    for(int row = 0; row < r; row++){
        for(int i = 0; i < c; i++){//copy matrix[row] array to buffer
            buffer[i] = matrix[row][i];
            if(largestE < buffer[i]) largestE = buffer[i];
        }

        currMax = buffer[0];
        int tempMax;
        for(int x = 0; x < c; x++){//calculate max of buffer
            tempMax = buffer[x];
            for(int y = x+1; y < c; y++){
                tempMax+=buffer[y];
                if(tempMax > currMax)   currMax = tempMax;
            }
        }
        if(row == 0){
            maxSum = currMax;
        }
        for(int i = row+1 ; i < r; i++){//iterate from the following row
            for(int col = 0; col < c; col++){
                buffer[col] += matrix[i][col];
            }
            currMax = buffer[0];
            for(int x = 0; x < c; x++){//calculate max of buffer
                tempMax = buffer[x];
                for(int y = x+1; y < c; y++){
                    tempMax+=buffer[y];
                    if(tempMax > currMax)   currMax = tempMax;
                }
            }

            if(currMax > maxSum){
                maxSum = currMax;
            }
        }
    }
    
    if(largestE < 0){
        return largestE;
    }
    else
        return maxSum;
}

int cubic(int** matrix){
    int maxSum, currMax;
    int* buffer = (int*)malloc(sizeof(int)*c);
    largestE = matrix[0][0];
    for(int row = 0; row < r; row++){
        for(int i = 0; i < c; i++){//copy first row to buffer
            buffer[i] = matrix[row][i];
        }
        kadane(matrix[row], &currMax);
        if(row == 0){
            maxSum = currMax;
        }
        for(int i = row+1 ; i < r; i++){//iterate from following row
            for(int col = 0; col < c; col++){
                buffer[col] += matrix[i][col];
            }
            int kadaneMax;
            kadane(buffer, &kadaneMax);
            if(kadaneMax > maxSum){
                maxSum = kadaneMax;
            }
        }
    }
    
    if(largestE < 0){
        return largestE;
    }
    else
        return maxSum;
}

int* kadane(int *arr, int* max){
    int* result = (int*)malloc(sizeof(int)*c);
    for(int i = 0; i < c; i++){
        result[i] = arr[i];
        if(largestE < arr[i]) largestE = arr[i];
    }
    *max = arr[0];
    for(int i = 1; i < r; i++){
        if(result[i] <= result[i] + result[i-1]){
            result[i] = result[i] + result[i-1];
        }
        if(*max < result[i]){
            *max = result[i];
        }
    }
    return result;
}
