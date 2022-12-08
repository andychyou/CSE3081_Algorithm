#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv){
    int len = 1000;
    char filename[30];
    
    sprintf(filename, "test_%d", len);
    FILE *fp = fopen(filename, "w");
    srand(time(NULL));
    fprintf(fp, "%d ", len);
    for(int i = 0 ; i < len; i++)
        fprintf(fp, "%d ", rand());
    fclose(fp);
    printf("create end\n");
    return 0;
}