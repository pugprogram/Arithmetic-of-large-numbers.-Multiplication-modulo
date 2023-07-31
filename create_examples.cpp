#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


typedef struct link{
	char ptype; /* type of the link */
	union
		{
		struct link *b;	/* bracket: ptr to the pair */
		char *f;	/* function or compound symbol: ptr to label. */
		char c;		/* symbol: actual value. */
/*		unsigned long u; /+* unicode symbol */ 
		unsigned int u;  /* unicode symbol */ 
		unsigned long n; /*  macro-digit   */
		unsigned short us_1, us_2;
		} pair;
	struct link *prec; /* ptr to preceding link */
	struct link *foll; /* ptr to following link */
}LINK;

#define NEXT(q)    ((q)->foll)
#define PREV(q)    ((q)->prec)
#define TYPE(q)    ((q)->ptype)




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
    bool flag = false;
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
        flag=true;
        number=number - (dopper<<1);
    }
    if (!flag) return res;
    while (true){
        number=0;
        char* dopchastnoe;
        dopchastnoe=malloc(1);
        dopchastnoe[0]='\0';
        if (strlen(chastnoe)==1){
                int num=chastnoe[0]-'0';
                if (num<2){
                    res=Copy_and_add(res,(num+'0'));
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

LINK* filling_structure(LINK* p,char* binary,int i,bool flag_chetn,bool flag_enter_0){
    unsigned long long multiplier=1;
    p=(LINK*)malloc(sizeof(LINK));
    p->foll=NULL;
    p->prec=NULL;
    p->ptype='d';
    if (flag_enter_0){
        p->pair.n=0;
        return p;
    }
    unsigned long long num=0;
    unsigned long long dop_i=i;
    for (i;(i<dop_i+31)&&(i<strlen(binary));i++){
        num+=(binary[i]-'0')*multiplier;
        multiplier=multiplier<<1;
    }
    p->pair.n=num;
    if (i<(strlen(binary)-1)) p->foll=filling_structure(p->foll,binary,i,!flag_chetn,false);
    else if (!flag_chetn) p->foll=filling_structure(p->foll,binary,i,!flag_chetn,true);
    return p;
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
    printf("\n");
    first_number=filling_structure(first_number,res,0,false,false);
    Print_Link(first_number);
    return 0;
}
