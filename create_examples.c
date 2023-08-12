#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#define DEGREE 10
#define SIZE_OF_EXAMPLE 2
//DEGREE - степень, отвечающая за размер генерируемых примеров (размер = 2^DEGREE)
//SIZE_OF_EXAMPLE-количество генерируемых примеров

/*
unsigned long long find_quantity_of_num_for_generation(){
    unsigned long long quanity_numbers=1;
    for (int i=1;i<=DEGREE;i++) quanity_numbers*=2;
    return quanity_numbers;
}
*/

int generation_number(long degree){
    //unsigned long long quantity_numbers=find_quantity_of_num_for_generation();
    FILE* fp;
    char name[]="example.txt";
    if ((fp= fopen(name,"w"))==NULL){
        printf("Error!");
        return -1;
    }
    for (int j=0;j<SIZE_OF_EXAMPLE;j++){
        for (long i=0;i<degree;i++) fprintf(fp,"%d",rand()%10);
        fprintf(fp,"\n");
    }
    fclose(fp);
    return 0;
}



int main(int argc, char** argv){
    if (argc<=1){
        printf("Error");
        return 0;
    }
    long a;
    a=strtol(argv[1],NULL,10);
    if (generation_number(a)==-1) return 0;
    //printf("%lld",quanity_numbers);
    return 0;
}
