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
    unsigned long num=0;
    int dop_i=i;
    for (i;(i<dop_i+32)&&(i<strlen(binary));i++){
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
        printf("%ld\n",p->pair.n);
        Print_Link(p->foll);
    }
}


char* Multyplication_large_numbers(char* num1, char* num2){
    char* res;
    unsigned long len1 = strlen(num1);
    unsigned long len2 =strlen(num2);
    if (len1>=len2){
        res=malloc(sizeof(char)*(len1+len2+1));
        res[len1+len2]='\0';
        for (unsigned long i=0;i<len1+len2;i++) res[i]='0';
        int k=len1+len2-1;
        for (int i=len2-1;i>=0;i--){
            for (int j=len1-1,l=k;j>=0;j--,l--){
                unsigned long num = (num1[j]-'0')*(num2[i]-'0');
                int dopper=(res[l]-'0')+num;
                res[l]=(dopper%10)+'0';
                res[l-1]=(dopper/10+res[l-1]-'0')+'0';
            }
            k--;
        }
    }
    if (len1<len2){
        res=malloc(sizeof(char)*(len1+len2+1));
        res[len1+len2]='\0';
        for (unsigned long i=0;i<len1+len2;i++) res[i]='0';
        int k=len1+len2-1;
        for (int i=len1-1;i>=0;i--){
            for (int j=len2-1,l=k;j>=0;j--,l--){
                unsigned long num = (num2[j]-'0')*(num1[i]-'0');
                int dopper=(res[l]-'0')+num;
                res[l]=(dopper%10)+'0';
                res[l-1]=(dopper/10+res[l-1]-'0')+'0';
            }
            k--;
        }    
    }
    if (res[0]=='0'){
        int len_0=0;
        for (int i=0;i<strlen(res);i++){
            if (res[i]!='0') break;
            len_0++;
        }
        char* dopper=malloc(sizeof(char)*(strlen(res)-len_0+1));
        dopper[strlen(res)-len_0]='\0';
        for (int i=len_0,j=0;i<strlen(res);i++,j++) dopper[j]=res[i];
        free(res);
        return dopper;
    }
    return res;
}


char* Add_large_numbers(char* num1, char* num2){
    unsigned long len1=strlen(num1);
    unsigned long len2=strlen(num2);
    unsigned long len=0;
    char* dopstr;
    char* new_num1;
    char* new_num2;
    if (len1<len2){
        len=len2-len1;
        dopstr=malloc(sizeof(char)*(len2+1));
        dopstr[len2]='\0';
        for (unsigned long i=0;i<len;i++){
            dopstr[i]='0';
        }
        for (unsigned long i=len,j=0;i<len+len1;i++,j++) {
            dopstr[i]=num1[j];
        }
        new_num1=malloc(sizeof(char)*(len2+1));
        new_num1[len2]='\0';
        for (unsigned i=0;i<len2;i++) new_num1[i]=dopstr[i];
        new_num2=malloc(sizeof(char)*(len2+1));
        new_num2[len2]='\0';
        for (unsigned i=0;i<len2;i++) new_num2[i]=num2[i];
        free(dopstr);
    }
    else if (len1>len2){
        len=len1-len2;
        dopstr=malloc(sizeof(char)*(len1+1));
        dopstr[len1]='\0';
        for (unsigned long i=0;i<len;i++){
            dopstr[i]='0';
        }
        for (unsigned long i=len,j=0;i<len+len2;i++,j++) {
            dopstr[i]=num2[j];
        }
        new_num2=malloc(sizeof(char)*(len1+1));
        new_num2[len1]='\0';
        for (unsigned i=0;i<len1;i++) new_num2[i]=dopstr[i];
        new_num1=malloc(sizeof(char)*(len1+1));
        new_num1[len1]='\0';
        for (unsigned i=0;i<len1;i++) new_num1[i]=num1[i];
        free(dopstr);
    }
    else{
        new_num1=malloc(sizeof(char)*(len1+1));
        new_num1[len1]='\0';
        new_num2=malloc(sizeof(char)*(len2+1));
        new_num2[len2]='\0';
        for (int i=0;i<len1;i++) new_num1[i]=num1[i];
        for (int i=0;i<len2;i++) new_num2[i]=num2[i];
    }
    len=strlen(new_num1);
    char* res=malloc(sizeof(char)*(len+2));
    res[len+1]='\0';
    for (unsigned long i=0;i<len+1;i++) res[i]='0';
    for (unsigned long i=len,j=len-1;i>0;i--,j--){
        int num=(new_num1[j]-'0')+(new_num2[j]-'0');
        int dop_res=(res[i]-'0')+num;
        res[i]=((((res[i]-'0')+num)%10)+'0');
        res[i-1]=((((res[i-1]-'0')+dop_res)/10)+'0');
    }
    if (res[0]=='0'){
        len=strlen(res);
        dopstr=malloc(sizeof(char)*(len));
        dopstr[len-1]='\0';
        for (unsigned long j=1;j<len;j++) dopstr[j-1]=res[j];
        return dopstr;
    }
    return res;
}



char* Return_in_decimal_system(LINK* p){
    char* mnog=malloc(2);
    mnog[0]='1';
    mnog[1]='\0';
    LINK* q=p;
    char* res=malloc(2);
    res[0]='0';
    res[1]='\0';
    while (q!=NULL){
        char coeff [30];
        snprintf(coeff,sizeof coeff,"%ld",q->pair.n);
        char* dopper;
        dopper=Multyplication_large_numbers(coeff,mnog);
        for (int i=0;i<strlen(dopper);i++) printf("%c",dopper[i]);
        printf("\n");
        res=Add_large_numbers(res,dopper);
        for (int i=0;i<strlen(res);i++) printf("%c",dopper[i]);
        printf("\n");
        free(dopper);
        mnog=Multyplication_large_numbers(mnog,"4294967296");
        q=q->foll;
    }
    return res;
}

void Printf_in_sixteenfour_system(LINK* p){
    if ((p!=NULL)&&(p->foll!=NULL)){
        unsigned long long senior_coeff=p->foll->pair.n;
        char Senior_coeff_str [30];
        senior_coeff=senior_coeff<<32;
        snprintf(Senior_coeff_str,sizeof Senior_coeff_str,"%lld",senior_coeff);
        char* for_printf;
        char junior_coeff[30];
        snprintf(junior_coeff,sizeof junior_coeff,"%ld",p->pair.n);
        for_printf=Add_large_numbers(Senior_coeff_str,junior_coeff);
        for (unsigned long i=0;i<strlen(for_printf);i++) printf("%c",for_printf[i]);
        printf("\n");
        Printf_in_sixteenfour_system(p->foll->foll);
    }
}


int main(){
    LINK* first_number;
    first_number=NULL;
    char* chastnoe=malloc(sizeof(char)*1);
    chastnoe[0]='\0';
    char* res;
    res=read_num_from_file_2(chastnoe);
    printf("...........Binary........\n");
    for(unsigned long long i=0;i<strlen(res);i++) printf("%c",res[i]);
    printf("\n\n........2^32...........\n");
    first_number=filling_structure(first_number,res,0,false,false);
    Print_Link(first_number);
    printf("\n........2^64...........\n");
    Printf_in_sixteenfour_system(first_number);
    res=Return_in_decimal_system(first_number);
    printf("\n........10...........\n");
    for(unsigned long long i=0;i<strlen(res);i++) printf("%c",res[i]);
    printf("\n");
    return 0;
}
