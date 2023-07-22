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


unsigned long long* read_num_from_file(unsigned long long* array){
    array[0]=0;
    array[1]=0;
    FILE* fp;
    char name[]="example.txt";
    unsigned long long number = 0;
    if ((fp= fopen(name,"r"))==NULL){
        printf("Error!");
        array[0]=-1;
        array[1]=-1;
        return array;
    }
    char one_symbol;
    bool flag=false;
    while ((one_symbol=fgetc(fp))!=EOF){
        if ((one_symbol=='\n') and (!flag)){
            flag=true;
            continue;
        }
        if ((one_symbol=='\n') and (flag)) break;
        if (!flag) array[0]=array[0]*10+one_symbol-'0';
        if (flag) array[1]=array[1]*10+one_symbol-'0';
    }
    return array;
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

unsigned long long Print_Link_decimal_system(LINK *p, int num,unsigned long long answer){
    if (p!=NULL){
        unsigned long long dopper = p->pair.n;
        int num2=num;
        while (num2){
            dopper=dopper<<63;
            dopper=dopper<<1;
            num2--;
        }
        answer=answer+dopper;
        answer=Print_Link_decimal_system(p->foll,num+1,answer);
    }
    return answer;
}



void Print_Link(LINK* p){
    if (p!=NULL){
        printf("%lld",p->pair.n);
        Print_Link(p->foll);
    }
}

int main(){
    unsigned long long* array_for_num=NULL;
    array_for_num=(unsigned long long*)malloc(2*sizeof(unsigned long long));
    array_for_num = read_num_from_file(array_for_num);
    if (array_for_num[0]==-1) return 0;
    LINK* first_num=NULL;
    first_num=save_in_Link(first_num,array_for_num[0]);
    LINK* second_num=NULL;
    second_num=save_in_Link(second_num,array_for_num[1]);
    //Print_Link(p);
    unsigned long long answer = 0;
    answer = Print_Link_decimal_system(first_num,0,answer);
    printf("%lld\n",answer);
    answer=0;
    answer=Print_Link_decimal_system(second_num,0,answer);
    printf("%lld",answer);
    return 0;
}
