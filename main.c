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




char* read_num_from_file_2(char* chastnoe,FILE* fp){
    char* res=malloc(2);
    res[1]='\0';
    char one_symbol;
    int number=0;
    int ostatok=0;
    int kolvo=0;
    char prev_symbol;
    bool flag = false;
    while (true){
        one_symbol=fgetc(fp);
        if ((one_symbol=='\n')||(one_symbol==EOF)){
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
        res=Add_large_numbers(res,dopper);
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
        snprintf(Senior_coeff_str,sizeof Senior_coeff_str,"%lld",senior_coeff);
        char* res;
        res=Multyplication_large_numbers(Senior_coeff_str,"4294967296");
        char* for_printf;
        char junior_coeff[30];
        snprintf(junior_coeff,sizeof junior_coeff,"%ld",p->pair.n);
        for_printf=Add_large_numbers(res,junior_coeff);
        for (unsigned long i=0;i<strlen(for_printf);i++) printf("%c",for_printf[i]);
        printf("\n");
        Printf_in_sixteenfour_system(p->foll->foll);
    }
}


unsigned long long Size_Link(LINK* p){
    unsigned long long size=0;
    while (p!=NULL) {
        size+=1;
        p=p->foll;
    }
    return size;
}






LINK* alghorithm_multyplication(LINK* first_num,LINK* second_num){
    LINK* res=NULL;
    res=(LINK*)malloc(sizeof(LINK));
    res->foll=NULL;
    res->prec=NULL;
    res->pair.n=0;
    unsigned long long size_first_num=Size_Link(first_num);
    unsigned long long size_second_num=Size_Link(second_num);
    if ((size_first_num>size_second_num)||(size_first_num==size_second_num)){
        LINK* dop_for_res=res;
        LINK* dop_for_res2=res;
        LINK* dop_for_first_num=first_num;
        while (second_num!=NULL){
        while (dop_for_first_num!=NULL){
            unsigned long long mnog=0;
            mnog=second_num->pair.n*dop_for_first_num->pair.n;
            if (dop_for_res2==NULL){
                dop_for_res2=(LINK*)malloc(sizeof(LINK));
                dop_for_res2->foll=NULL;
                dop_for_res2->prec=NULL;
                dop_for_res2->pair.n=0;
            }
            mnog=dop_for_res2->pair.n+mnog;
            unsigned long mainpart=mnog>>32;
            unsigned long ost=mnog-(mainpart<<32);
            dop_for_res2->pair.n=ost;
            if (mainpart){
                if (dop_for_res2->foll==NULL){
                    dop_for_res2->foll=(LINK*)malloc(sizeof(LINK));
                    dop_for_res2->foll->pair.n=0;
                    dop_for_res2->foll->foll=NULL;
                    dop_for_res2->foll->prec=NULL;
                }
                dop_for_res2->foll->pair.n+=mainpart;
            }
            dop_for_res2=dop_for_res2->foll;
            dop_for_first_num=dop_for_first_num->foll;
        }
        dop_for_first_num=first_num;
        second_num=second_num->foll;
        dop_for_res=dop_for_res->foll;
        dop_for_res2=dop_for_res;
        }
    }
    else{
        LINK* dop_for_res=res;
        LINK* dop_for_res2=res;
        LINK* dop_for_second_num=second_num;
        while (first_num!=NULL){
        while (dop_for_second_num!=NULL){
            unsigned long long mnog=0;
            mnog=first_num->pair.n*dop_for_second_num->pair.n;
            if (dop_for_res2==NULL){
                dop_for_res2=(LINK*)malloc(sizeof(LINK));
                dop_for_res2->foll=NULL;
                dop_for_res2->prec=NULL;
                dop_for_res2->pair.n=0;
            }
            mnog=dop_for_res2->pair.n+mnog;
            unsigned long mainpart=mnog>>32;
            unsigned long ost=mnog-(mainpart<<32);
            dop_for_res2->pair.n=ost;
            if (mainpart){
                if (dop_for_res2->foll==NULL){
                    dop_for_res2->foll=(LINK*)malloc(sizeof(LINK));
                    dop_for_res2->foll->pair.n=0;
                    dop_for_res2->foll->foll=NULL;
                    dop_for_res2->foll->prec=NULL;
                }
                dop_for_res2->foll->pair.n+=mainpart;
            }
            dop_for_res2=dop_for_res2->foll;
            dop_for_second_num=dop_for_second_num->foll;
        }
        dop_for_second_num=second_num;
        first_num=first_num->foll;
        dop_for_res=dop_for_res->foll;
        dop_for_res2=dop_for_res;
        }    
    }
    return res;
}




int main(){
    LINK* first_number;
    first_number=NULL;
    char* chastnoe=malloc(sizeof(char)*1);
    chastnoe[0]='\0';
    char* num1;
    FILE* fp;
    char name[]="example.txt";
    if ((fp= fopen(name,"r"))==NULL){
        printf("Error!");
        return 0;
    }
    num1=read_num_from_file_2(chastnoe,fp);
    free(chastnoe);
    chastnoe=malloc(sizeof(char)*1);
    chastnoe[0]='\0';
    char* num2;
    num2=read_num_from_file_2(chastnoe,fp);
    fclose(fp);
    printf("...........Binary........\n");
    for(unsigned long long i=0;i<strlen(num1);i++) printf("%c",num1[i]);
    printf("\n");
    for(unsigned long long i=0;i<strlen(num2);i++) printf("%c",num2[i]);
    printf("\n\n........2^32...........\n");
    first_number=filling_structure(first_number,num1,0,false,false);
    Print_Link(first_number);
    printf("\n");
    LINK* second_number;
    second_number=filling_structure(second_number,num2,0,false,false);
    Print_Link(second_number);
    printf("\n");

    LINK* res;
    res=alghorithm_multyplication(first_number,second_number);
    char* check;
    check=Return_in_decimal_system(res);
    for (int i=0;i<strlen(check);i++) printf("%c",check[i]);
    //Print_Link(res);
    /*
    printf("\n........2^64...........\n");
    Printf_in_sixteenfour_system(first_number);
    printf("\n");
    Printf_in_sixteenfour_system(second_number);
    */
    return 0;
}
