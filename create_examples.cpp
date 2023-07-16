#include<iostream>
#include <fstream>
#include<stdlib.h>
#define DEGREE 3
#define SIZE_OF_EXAMPLE 4

using namespace std;

unsigned long long find_quantity_of_num_for_generation(){
    unsigned long long quanity_numbers=1;
    for (int i=1;i<=DEGREE;i++) quanity_numbers*=2;
    return quanity_numbers;
}

int generation_number(){
    unsigned long long quantity_numbers=find_quantity_of_num_for_generation();
    FILE* fp;
    char name[]="exampe.txt";
    if ((fp= fopen(name,"w"))==NULL){
        printf("Error!");
        return -1;
    }
    for (int j=0;j<SIZE_OF_EXAMPLE;j++){
        unsigned long long num=0;
        for (unsigned long long i=0;i<quantity_numbers;i++){
            num=num*10+rand()%10;
        }
        fprintf(fp,"%lld\n",num);
    }
    
    return 0;
}



int main(){
    if (generation_number()==-1) return 0;
    //printf("%lld",quanity_numbers);
    return 0;
}
