#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#define DEGREE 5
#define SIZE_OF_EXAMPLE 4


unsigned long long find_quantity_of_num_for_generation(){
    unsigned long long quanity_numbers=1;
    for (int i=1;i<=DEGREE;i++) quanity_numbers*=2;
    return quanity_numbers;
}

int generation_number(){
    unsigned long long quantity_numbers=find_quantity_of_num_for_generation();
    FILE* fp;
    char name[]="example.txt";
    if ((fp= fopen(name,"w"))==NULL){
        printf("Error!");
        return -1;
    }
    for (int j=0;j<SIZE_OF_EXAMPLE;j++){
        for (unsigned long long i=0;i<quantity_numbers;i++) fprintf(fp,"%d",rand()%10);
        fprintf(fp,"\n");
    }
    
    return 0;
}



int main(){
    if (generation_number()==-1) return 0;
    //printf("%lld",quanity_numbers);
    return 0;
}
