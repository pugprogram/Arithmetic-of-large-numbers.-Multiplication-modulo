#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

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
}LINK;

#define NEXT(q)    ((q)->foll)
#define PREV(q)    ((q)->prec)
#define TYPE(q)    ((q)->ptype)




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





char* Copy_and_add(char* chastnoe,char dopper){
    char* dop_str=malloc(sizeof(char)*(strlen(chastnoe)+2));
    for (unsigned long long i=0;i<strlen(chastnoe);i++){
        dop_str[i]=chastnoe[i];
    }
    dop_str[strlen(chastnoe)]=dopper;
    dop_str[strlen(chastnoe)+1]='\0';
    free(chastnoe);
    chastnoe=malloc(sizeof(char)*(strlen(dop_str)+1));
    for (unsigned long long i=0;i<strlen(dop_str);i++){
        chastnoe[i]=dop_str[i];
    }
    chastnoe[strlen(dop_str)]='\0';
    free(dop_str);

    return chastnoe;

}



char* read_num_from_file_2(char* chastnoe){
    char* res=malloc(2);
    res[1]='\0';
    FILE* fp;
    char name[]="example.txt";
    if ((fp= fopen(name,"r"))==NULL){
        printf("Error!");
        return NULL;
    }
    char one_symbol;
    int number=0;
    int ostatok=0;
    int kolvo=0;
    char prev_symbol;
    while ((one_symbol=fgetc(fp))!=EOF){
        if (one_symbol=='\n'){
            res[0]=(number+'0');
            break; 
        }
        number=(number*10)+one_symbol-'0';
        if (number<2){
           if (strlen(chastnoe)){
                chastnoe=Copy_and_add(chastnoe,'0');
           }
            continue;
        }
        int dopper=number>>1;
        chastnoe=Copy_and_add(chastnoe,(dopper+'0'));
        number=number - (dopper<<1);
    }
    while (true){
        number=0;
        char* dopchastnoe;
        dopchastnoe=malloc(1);
        dopchastnoe[0]='\0';
        if (strlen(chastnoe)==1){
                int num=chastnoe[0]-'0';
                if (num<2){
                     res=Copy_and_add(res,chastnoe[0]);
                     break;
                }      
        }
        for (unsigned long long i=0;i<strlen(chastnoe);i++){
            number=(number*10)+(chastnoe[i]-'0');
            if (number<2){
                if (strlen(dopchastnoe)){
                    dopchastnoe=Copy_and_add(dopchastnoe,'0');
                }
            continue;
            }
            int dopper=number>>1;
            dopchastnoe=Copy_and_add(dopchastnoe,(dopper+'0'));
            number=number - (dopper<<1);
        }
        res=Copy_and_add(res,(number+'0'));
        free(chastnoe);
        chastnoe=malloc(sizeof(char)*(strlen(dopchastnoe)+1));
        for (unsigned long long i=0;i<strlen(dopchastnoe);i++) chastnoe[i]=dopchastnoe[i];
        chastnoe[strlen(dopchastnoe)]='\0';
        free(dopchastnoe);
    }
    return res;
}

LINK* filling_structure(LINK* p){

}


void Print_Link(LINK* p){
    if (p!=NULL){
        printf("%lld\n",p->pair.n);
        Print_Link(p->foll);
    }
}



int main(){
    LINK* first_number;
    first_number=NULL;
    char* chastnoe=malloc(sizeof(char)*1);
    chastnoe[0]='\0';
    char* res;
    res=read_num_from_file_2(chastnoe);
    for(unsigned long long i=0;i<strlen(res);i++) printf("%c",res[i]);
    return 0;
}
