#include<iostream>
#include <fstream>
#include<stdlib.h>
#include <stdio.h>

//2^63, так как число 2^64 не помещается в стандартный тип данных языка C (даже самый большой)
//поэтому при реализации деления на 2^64 делим сначала на 2^63, а затем побитово сдвигаем на 1
//#define MODULE 0b10000000000000000000000000000000000000000000000000000000000000000
// значение юудет храниться в n 
//я искусственно увеличила тип (в рефале unsigned long, но у меня может не вместиться даже в long long )

 typedef struct link{
	char ptype; /* type of the link */
	union
		{
		struct link *b;	/* bracket: ptr to the pair */
		char *f;	/* function or compound symbol: ptr to label. */
		char c;		/* symbol: actual value. */
/*		unsigned long u; /+* unicode symbol */ 
		unsigned int u;  /* unicode symbol */ 
		unsigned long long n; /*  macro-digit   */
		unsigned short us_1, us_2;
		} pair;
	struct link *prec; /* ptr to preceding link */
	struct link *foll; /* ptr to following link */
} LINK;


unsigned long long read_num_from_file(){
    FILE* fp;
    char name[]="exampe.txt";
    unsigned long long number = 0;
    if ((fp= fopen(name,"r"))==NULL){
        printf("Error!");
        return -1;
    }
    char one_symbol;
    while ((one_symbol=fgetc(fp))!=EOF){
        if (one_symbol=='\n') break;
        number=number*10+one_symbol-'0';
    }
    return number;
}

//Проверка работоспособности производилась на переводе в двоичную систему счисления
LINK* save_in_Link(LINK* p, unsigned long long number){
    p=(LINK*)malloc (sizeof(LINK));
    p->prec=NULL;
    p->foll=NULL;
    unsigned long long one_num=number;
    number=number>>63;
    number=number>>1;
    unsigned long long dop_peremenaya_for_ostatok=number<<63;
    dop_peremenaya_for_ostatok=number<<1;
    p->pair.n=one_num-dop_peremenaya_for_ostatok;
    if (number) p->foll=save_in_Link(p->foll,number);
    return p;
}

void Print_Link(LINK* p){
    
}

int main(){
    unsigned long long number_ten_system = read_num_from_file();
    if (number_ten_system==-1) return 0;
    LINK* p=NULL;
    p=save_in_Link(p,number_ten_system);
    while (p!=NULL) {
        printf("%lld", p->pair.n);
        p=p->foll;
    }
    return 0;
}
