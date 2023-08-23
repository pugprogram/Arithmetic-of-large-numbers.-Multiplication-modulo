#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
//#define INT_MAX 18446744073709551615

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

LINK* SUB_LINK(LINK*p, LINK*q);

//Вспомогательная функция
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

typedef struct num_for_file{
    char* num;
    bool flag;
}NUM_FILE;



//Функция чтения из файла
//Для запуска необходимо создать char* chastnoe, а также выделить ему память
//также должен быть запущен файл, из которого берем тестовые данные 
NUM_FILE* read_num_from_file_2(char* chastnoe,FILE* fp){
    NUM_FILE* num_res=(NUM_FILE*)malloc(sizeof(NUM_FILE));
    num_res->flag=false;
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
        if (one_symbol==EOF) num_res->flag=true;
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
    if (!flag) {
        num_res->num=res;
        return num_res;
    }
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
    num_res->num=res;
    return num_res;
}

//Функция сопоставления строки, полученной в предыдущей функции, и стуктуры LINK
//Параметры - p=NULL, char* binary - строка, полученная в предыдущей функции
//i=0,flag_chetn=flag_enter_0=0
LINK* filling_structure(LINK* p,LINK* prev,char* binary,int i,bool flag_chetn,bool flag_enter_0){
    unsigned long long multiplier=1;
    p=(LINK*)malloc(sizeof(LINK));
    p->foll=NULL;
    p->prec=prev;
    p->ptype='d';
    if (flag_enter_0){
        p->pair.n=0;
        return p;
    }
    unsigned long long num=0;
    int dop_i=i;
    for (i;(i<dop_i+32)&&(i<strlen(binary));i++){
        num+=(binary[i]-'0')*multiplier;
        multiplier=multiplier<<1;
    }
    p->pair.n=num;
    if (i<(strlen(binary))) p->foll=filling_structure(p->foll,p,binary,i,!flag_chetn,false);
    //else if (!flag_chetn) p->foll=filling_structure(p->foll,p,binary,i,!flag_chetn,true);
    return p;
}

//Функция простой печати LINK(печает в системе счисления 2^32)
void Print_Link(LINK* p){
    if (p!=NULL){
        printf("%ld\n",p->pair.n);
        Print_Link(p->foll);
    }
}

//Функция перемножения больших чисел, представленных в виде строк (вспомогательная функция)
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

//Функция сложения больших чисел, представленных в виде строк (вспомогательная функция)
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


//Функция, которая возвращает строку - число, которое хранится в нашей структуре в десятичной системе
//счисления
char* Return_in_decimal_system(LINK* p){
    char* mnog=malloc(2);
    mnog[0]='1';
    mnog[1]='\0';
    LINK* q=p;
    char* res=malloc(2);
    res[0]='0';
    res[1]='\0';
    if (q!=NULL){
        if (q->ptype=='n') printf("-");
    }
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

//Функция печати числа в системе счисления 2^64
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

//Вспомогательная функция
unsigned long long Size_Link(LINK* p){
    unsigned long long size=0;
    while (p!=NULL) {
        size+=1;
        p=p->foll;
    }
    return size;
}

//1 str1>=str2
//else 0
int Compare_str(char* str1, char* str2){
    if (strlen(str1)>strlen(str2)) return 1;
    if (strlen(str1)<strlen(str2)) return 0;
    else{
        for (unsigned long i=0;i<strlen(str1);i++){
            if (str1[i]>str2[i]) return 1;
            if (str1[i]<str2[i]) return 0;
        }   
    }
    return 1;
}

typedef struct Ost_And_Res{
    unsigned long res;
    unsigned long ost;
}RES;

RES* division_in_ten_system(char* mnog){
    RES* p =(RES*) malloc(sizeof(RES));
    char* res=malloc(sizeof(char));
    res[0]='\0';
    char* chastnoe=malloc(sizeof(char));
    chastnoe[0]='\0';
    int i=0;
    for (i=0;i<strlen(mnog);i++){
        chastnoe=Copy_and_add(chastnoe,mnog[i]);
        if (Compare_str(chastnoe,"4294967296")==1){
            unsigned long long mnog_num=strtoull(chastnoe,NULL,10);
            mnog_num=mnog_num>>32;
            res=Copy_and_add(res,mnog_num+'0');
            unsigned long long chastnoe_num=strtoull(chastnoe,NULL,10);
            chastnoe_num-=mnog_num<<32;
            char dop_chastnoe[40];
            snprintf(dop_chastnoe,sizeof dop_chastnoe, "%lld",chastnoe_num);
            free(chastnoe);
            chastnoe=malloc(sizeof(char)*(strlen(dop_chastnoe)+1));
            chastnoe[strlen(dop_chastnoe)]='\0';
            for(int j=0;j<strlen(dop_chastnoe);j++){
                chastnoe[j]=dop_chastnoe[j];
            }
            break;
        }
    }
    
    if ((i+1)==strlen(mnog)) {
        p->ost=strtoul(chastnoe,NULL,10);
        p->res=strtoul(res,NULL,10);
        return p;
    }
    for (i=i+1;i<strlen(mnog);i++){
        chastnoe=Copy_and_add(chastnoe,mnog[i]);
        if (!Compare_str(chastnoe,"4294967296")){
            res=Copy_and_add(res,'0');
            continue;
        }
        unsigned long long mnog_num=strtoull(chastnoe,NULL,10);
        mnog_num=mnog_num>>32;
        res=Copy_and_add(res,mnog_num+'0');
        unsigned long long chastnoe_num=strtoull(chastnoe,NULL,10);
        chastnoe_num-=mnog_num<<32;
        char dop_chastnoe[40];
        snprintf(dop_chastnoe,sizeof dop_chastnoe, "%lld",chastnoe_num);
        free(chastnoe);
        chastnoe=malloc(sizeof(char)*(strlen(dop_chastnoe)+1));
        chastnoe[strlen(dop_chastnoe)]='\0';
        for(int j=0;j<strlen(dop_chastnoe);j++){
            chastnoe[j]=dop_chastnoe[j];
        }
    }
    p->ost=strtoul(chastnoe,NULL,10);
    p->res=strtoul(res,NULL,10);
    return p;
}

LINK* reverse(LINK* p){
    if (p->foll==NULL) return p;
    while (p->foll->foll!=NULL) p=p->foll;
    return p->foll;
}

LINK* DELETE_ZERROW(LINK*p){
    LINK* p_r=reverse(p);
    while (true){
        if (p_r->pair.n==0) {
            LINK* pred=p_r->prec;
            p_r->prec->foll=NULL;
            free(p_r);
            p_r=pred;
        }
        else break;
    }
    return p;
}

//Реализация алгоритма умножения в системе счисления 2^32
LINK* alghorithm_multyplication(LINK* first_num,LINK* second_num){
    LINK* res=NULL;
    res=(LINK*)malloc(sizeof(LINK));
    res->foll=NULL;
    res->prec=NULL;
    if (((first_num->ptype=='d')&&(second_num->ptype=='n'))||((first_num->ptype=='n')&&(second_num->ptype=='d'))) res->ptype='n';
    else res->ptype='d';
    res->pair.n=0;
    unsigned long long size_first_num=Size_Link(first_num);
    unsigned long long size_second_num=Size_Link(second_num);
    if (((size_first_num==1)&&(first_num->pair.n==0))||((size_second_num==1)&&(second_num->pair.n==0))) return res;
    if ((size_first_num>size_second_num)||(size_first_num==size_second_num)){
        LINK* dop_for_dop_res2=NULL;
        LINK* dop_for_res=res;
        LINK* dop_for_res2=res;
        LINK* dop_for_first_num=first_num;
        while (second_num!=NULL){
        while (dop_for_first_num!=NULL){
            unsigned long long mnog=0;
            unsigned long mainpart=0;
            unsigned long ost=0;
            //Проверка на переполнение
            if (dop_for_res2==NULL){
                dop_for_res2=(LINK*)malloc(sizeof(LINK));
                dop_for_res2->ptype='d';
                dop_for_res2->foll=NULL;
                dop_for_res2->prec=dop_for_dop_res2;
                dop_for_res2->pair.n=0;
            }
            //unsigned long long per=184467440737095599;
            /*if ((second_num->pair.n*dop_for_first_num->pair.n)>per){
                char* res;
                char str1[30];
                snprintf(str1,sizeof str1,"%ld",second_num->pair.n);
                char str2[30];
                snprintf(str2,sizeof str2,"%ld",dop_for_first_num->pair.n);
                res=Multyplication_large_numbers(str1,str2);
                char str3[30];
                snprintf(str3,sizeof str3,"%ld",dop_for_res2->pair.n);
                res=Add_large_numbers(res,str3);
                RES* p;
                p=division_in_ten_system(res);
                mainpart=p->res;
                printf("mainpart = %ld\n",mainpart);
                ost=p->ost;
            }*/
            //else {
                mnog=second_num->pair.n*dop_for_first_num->pair.n;
                mnog=dop_for_res2->pair.n+mnog;
                if (mnog){
                    mainpart=mnog>>32;
                    ost=mnog-(mainpart<<32);
                }
                else{
                    mainpart=0;
                    ost=0;
                }
            //}
            dop_for_res2->pair.n=ost;
            //if (mainpart){
                if (dop_for_res2->foll==NULL){
                    dop_for_res2->foll=(LINK*)malloc(sizeof(LINK));
                    dop_for_res2->foll->ptype='d';
                    dop_for_res2->foll->pair.n=0;
                    dop_for_res2->foll->foll=NULL;
                    dop_for_res2->foll->prec=dop_for_res2;
                }
                dop_for_res2->foll->pair.n+=mainpart;
            //}
            dop_for_dop_res2=dop_for_res2;
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
        LINK* dop_for_dop_res2=NULL;
        LINK* dop_for_res=res;
        LINK* dop_for_res2=res;
        LINK* dop_for_second_num=second_num;
        while (first_num!=NULL){
        while (dop_for_second_num!=NULL){
            unsigned long long mnog=0;
            mnog=first_num->pair.n*dop_for_second_num->pair.n;
            if (dop_for_res2==NULL){
                dop_for_res2=(LINK*)malloc(sizeof(LINK));
                dop_for_res2->ptype='d';
                dop_for_res2->foll=NULL;
                dop_for_res2->prec=dop_for_dop_res2;
                dop_for_res2->pair.n=0;
            }
            mnog=dop_for_res2->pair.n+mnog;
            unsigned long mainpart=mnog>>32;
            unsigned long ost=mnog-(mainpart<<32);
            dop_for_res2->pair.n=ost;
            //if (mainpart){
                if (dop_for_res2->foll==NULL){
                    dop_for_res2->foll=(LINK*)malloc(sizeof(LINK));
                    dop_for_res2->foll->ptype='d';
                    dop_for_res2->foll->pair.n=0;
                    dop_for_res2->foll->foll=NULL;
                    dop_for_res2->foll->prec=dop_for_res2;
                }
                dop_for_res2->foll->pair.n+=mainpart;
            //}
            dop_for_dop_res2=dop_for_res2;
            dop_for_res2=dop_for_res2->foll;
            dop_for_second_num=dop_for_second_num->foll;
        }
        dop_for_second_num=second_num;
        first_num=first_num->foll;
        dop_for_res=dop_for_res->foll;
        dop_for_res2=dop_for_res;
        }    
    }
    if (res!=NULL) res=DELETE_ZERROW(res);
    return res;
}






//return 1 if p>q
//return 0 if p=q
//return -1 p<q
int Compare(LINK* p, LINK* q){
    int size_p=Size_Link(p);
    int size_q=Size_Link(q);
    LINK* reverse_p=reverse(p);
    LINK* reverse_q=reverse(q);
    if (size_p>size_q){
      return 1;  
    } 
    if (size_q>size_p) return -1;
    while (reverse_p!=NULL){
        if (reverse_p->pair.n>reverse_q->pair.n) return 1;
        if (reverse_p->pair.n<reverse_q->pair.n) return -1;
        reverse_p=reverse_p->prec;
        reverse_q=reverse_q->prec;
    }
    return 0;
}

LINK* COPY_LINK(LINK*p){
    if (p==NULL) return NULL;
    LINK* res;
    res=(LINK*)malloc(sizeof(LINK));
    res->foll=NULL;
    res->prec=NULL;
    res->ptype='d';
    res->pair.n=p->pair.n;
    LINK* dop_res=res;
    p=p->foll;
    while (p!=NULL){
        dop_res->foll=(LINK*)malloc(sizeof(LINK));
        dop_res->foll->foll=NULL;
        dop_res->foll->pair.n=p->pair.n;
        dop_res->foll->prec=dop_res;
        dop_res->foll->ptype='d';
        dop_res=dop_res->foll;
        p=p->foll;
    }
    return res;
}

//Функция сложения в системе счисления 2^32
LINK* ADD_LINK(LINK*p, LINK*q){
    if ((Size_Link(p)==1)&&(p->pair.n==0)) return q;
    if ((Size_Link(q)==1)&&(q->pair.n==0)) return p;
    if ((p->ptype=='n')&&(q->ptype=='d')){
        LINK* p_new;
        p_new=COPY_LINK(p);
        p_new=SUB_LINK(q,p_new);
        return p_new;
    }
    else if ((p->ptype=='d')&&(q->ptype=='n')){
        LINK* q_new;
        q_new=COPY_LINK(q);
        q_new=SUB_LINK(p,q_new);
        return q_new;
    }
    LINK* res=(LINK*)malloc(sizeof(LINK));
    res->foll=NULL;
    res->prec=NULL;
    if ((p->ptype=='n')&&(q->ptype=='n')) res->ptype='n';
    else res->ptype='d';
    res->pair.n=0;
    LINK* dop_res=res;
    if (Size_Link(p)<Size_Link(q)){
        LINK* dop=p;
        p=q;
        q=dop;
    }
    while (q!=NULL){
        unsigned long long num=p->pair.n+q->pair.n+dop_res->pair.n;
        unsigned long long mainpart=num>>32;
        unsigned long long ost=num-(mainpart<<32);
        dop_res->pair.n=ost;
        dop_res->foll=(LINK*)malloc(sizeof(LINK));
        dop_res->foll->prec=dop_res;
        dop_res->foll->ptype='d';
        dop_res->foll->foll=NULL;
        dop_res->foll->pair.n=mainpart;
        q=q->foll;
        p=p->foll;
        dop_res=dop_res->foll;
    }
    while (p!=NULL){
        unsigned long long num=p->pair.n+dop_res->pair.n;
        unsigned long long mainpart=num>>32;
        unsigned long long ost=num-(mainpart<<32);
        dop_res->pair.n=ost;
        dop_res->foll=(LINK*)malloc(sizeof(LINK));
        dop_res->foll->prec=dop_res;
        dop_res->foll->ptype='d';
        dop_res->foll->foll=NULL;
        dop_res->foll->pair.n=mainpart;
        p=p->foll;
        dop_res=dop_res->foll;
    }
    res=DELETE_ZERROW(res);
    return res;
}

//Вспомогательная! нужна для алгоритма деления в 2^32 системе счисления
//Функция обрабатывает все случаи.........
LINK* SUB_LINK(LINK*p, LINK*q){
    bool flag1=false;
    if (Compare(p,q)==1){
        if ((p->ptype=='n')&&(q->ptype=='n')){
            flag1=true;
        }
        else if ((p->ptype=='n')&&(q->ptype=='d')){
            LINK* res;
            res=ADD_LINK(p,q);
            res->ptype='n';
            return res;
        }
        else if ((p->ptype=='d')&&(q->ptype=='n')){
            LINK* res;
            res=ADD_LINK(p,q);
            return res;
        }
    }
    else if (Compare(p,q)==-1){
        if ((p->ptype=='d')&&(q->ptype=='d')){
            LINK* dop=p;
            p=q;
            q=dop;
            flag1=true;
        }
        else if ((p->ptype=='n')&&(q->ptype=='n')){
            LINK* dop=p;
            p=q;
            q=dop;
        }
        else if ((p->ptype=='d')&&(q->ptype=='n')){
            LINK* res;
            LINK* new_q=COPY_LINK(q);
            res=ADD_LINK(p,new_q);
            return res;
        }
        else if ((p->ptype=='n')&&(q->ptype=='d')){
            LINK* res;
            LINK* new_p=COPY_LINK(p);
            res=ADD_LINK(new_p,q);
            res->ptype='n';
            return res;
        }
    } 
    else if (Compare(p,q)==0){
        if (((p->ptype=='d')&&(q->ptype=='d'))||((p->ptype=='n')&&(q->ptype=='n'))){
            LINK* res=(LINK*)malloc(sizeof(LINK));
            res->pair.n=0;
            res->ptype='d';
            res->foll=NULL;
            res->prec=NULL;
            return res;
        }
        else if ((p->ptype=='d')&&(q->ptype=='n')){
            LINK* res;
            LINK* mnog;
            mnog=(LINK*)malloc(sizeof(LINK));
            mnog->foll=NULL;
            mnog->prec=NULL;
            mnog->ptype='d';
            mnog->pair.n=2;
            res=alghorithm_multyplication(p,mnog);
            return res;
        }
        else if ((p->ptype=='n')&&(q->ptype=='d')){
            LINK* res;
            LINK* mnog;
            mnog=(LINK*)malloc(sizeof(LINK));
            mnog->foll=NULL;
            mnog->prec=NULL;
            mnog->ptype='n';
            mnog->pair.n=2;
            res=alghorithm_multyplication(p,mnog);
            return res;
        }
    }
    bool flag =false;
    LINK* res=(LINK*)malloc(sizeof(LINK));
    res->pair.n=0;
    if (flag1) res->ptype='n';
    else res->ptype='d';
    res->foll=NULL;
    res->prec=NULL;
    LINK* dop_for_res=res;
    while (q!=NULL){
        if (p->pair.n>=q->pair.n) {
            dop_for_res->pair.n=p->pair.n-q->pair.n;
            if (flag){
                dop_for_res->pair.n-=1;
                flag=false;
            }
        }
        else{
            dop_for_res->pair.n=4294967296+p->pair.n-q->pair.n;
            if (flag) {
                dop_for_res->pair.n-=1;
            }
            flag=true;
        }
        p=p->foll;
        dop_for_res->foll=(LINK*)malloc(sizeof(LINK));
        dop_for_res->foll->ptype='d';
        dop_for_res->foll->pair.n=0;
        dop_for_res->foll->foll=NULL;
        dop_for_res->foll->prec=dop_for_res;
        q=q->foll;
        dop_for_res=dop_for_res->foll;
    }
    while (p!=NULL){
        unsigned long long num=p->pair.n+dop_for_res->pair.n;
        if (flag){
            num-=1;
            flag=false;
        }
        unsigned long long mainpart=num>>32;
        unsigned long long ost=num-(mainpart<<32);
        dop_for_res->pair.n=ost;
        dop_for_res->foll=(LINK*)malloc(sizeof(LINK));
        dop_for_res->foll->prec=dop_for_res;
        dop_for_res->foll->ptype='d';
        dop_for_res->foll->foll=NULL;
        dop_for_res->foll->pair.n=mainpart;
        p=p->foll;
        dop_for_res=dop_for_res->foll;
    }
    if (res!=NULL) res=DELETE_ZERROW(res);
    return res;
}

unsigned long long Size_Link_For_Not_reverse(LINK* p){
    unsigned long long size=0;
    while (p!=NULL) {
        size+=1;
        p=p->prec;
    }
    return size;
}

//return 1 if p>q
//return 0 if p=q
//return -1 p<q
int Compare_For_Not_Reverse(LINK* p, LINK* q){
    int size_p=Size_Link_For_Not_reverse(p);
    int size_q=Size_Link_For_Not_reverse(q);
    if ((size_p>size_q)&&(size_p-size_q!=1)&&(p->pair.n!=0)) return 1;
    if ((size_q>size_p)&&(size_q-size_p!=1)&&(q->pair.n!=0)) return -1;
    if (size_p>size_q) p=p->prec;
    else if (size_q>size_p) q=q->prec;
    while (p!=NULL){
        if (p->pair.n>q->pair.n) return 1;
        if (p->pair.n<q->pair.n) return -1;
        p=p->prec;
        q=q->prec;
    }
    return 0;
}



void Print_Link_reverse(LINK* p){
    if (p!=NULL){
        printf("%ld\n",p->pair.n);
        Print_Link_reverse(p->prec);
    }
}



LINK* Div_in_32_system(LINK*a, LINK* b){
    if ((a==NULL)||(b==NULL)) return NULL;
    if (Compare(a,b)==-1){
        LINK* res;
        res=(LINK*)malloc(sizeof(LINK));
        res->pair.n=0;
        res->foll=NULL;
        res->prec=NULL;
        res->ptype='d';
        return res;
    }
    if (((Size_Link(a)==Size_Link(b))&&(Size_Link(a)==1))){
        if (!b->pair.n){
            printf("Деление на 0 запрещено\n");
            return NULL;
        }
        LINK* res;
        res=(LINK*)malloc(sizeof(LINK));
        unsigned long long res1=a->pair.n/b->pair.n;
        if ((res1)&&(((a->ptype=='d')&&(b->ptype=='n'))||((a->ptype=='n')&&(b->ptype=='d')))) res->ptype='n';
        else res->ptype='d';
        res->prec=NULL;
        res->foll=NULL;
        res->pair.n=res1;
        return res;
    }
    if ((Size_Link(a)==1)&&(a->pair.n==0)){
        LINK* res;
        res=(LINK*)malloc(sizeof(LINK));
        res->foll=NULL;
        res->pair.n=0;
        res->prec=NULL;
        res->ptype='d';
        return res;
    }
    LINK* bn=reverse(b);
    LINK* normal_a;
    LINK* normal_b;
    LINK* vector_for_normalization=NULL;
    vector_for_normalization=(LINK*)malloc(sizeof(LINK));
    vector_for_normalization->ptype='d';
    vector_for_normalization->prec=NULL;
    unsigned long long base=4294967296;
    vector_for_normalization->pair.n=(unsigned long) (base/(bn->pair.n+1));
    vector_for_normalization->foll=NULL;
    unsigned long long base_without_1=4294967296-1;
    normal_a=alghorithm_multyplication(a,vector_for_normalization);
    normal_b=alghorithm_multyplication(b,vector_for_normalization);
    //Результат
    LINK* res;
    res=(LINK*)malloc(sizeof(LINK));
    res->foll=NULL;
    res->prec=NULL;
    res->pair.n=0;
    res->ptype='d';
    LINK* normal_a_reverse;
    LINK* normal_b_reverse;
    normal_a_reverse=reverse(normal_a);
    normal_b_reverse=reverse(normal_b);
    //Остаток
    LINK* q;
    q=(LINK*)malloc(sizeof(LINK));        
    q->foll=NULL;
    q->pair.n=normal_a_reverse->pair.n;
    q->prec=NULL;
    q->ptype='d';
    /*q->foll=(LINK*)malloc(sizeof(LINK));        
    q->foll->foll=NULL;
    q->foll->pair.n=0;
    q->foll->prec=q;
    */
    unsigned long long first_num=normal_a_reverse->pair.n;
    unsigned long long second_num=normal_a_reverse->prec->pair.n;
    while (Compare(q,normal_b)==-1){
        if (normal_a_reverse->prec==NULL){
            LINK* ost;
            ost=(LINK*)malloc(sizeof(LINK));
            ost->foll=NULL;
            ost->ptype='d';
            ost->prec=NULL;
            ost->pair.n=0;
            return ost;
        }
        normal_a_reverse=normal_a_reverse->prec;
        q->prec=malloc(sizeof(LINK));
        q->prec->ptype='d';
        q->prec->foll=q;
        q->prec->pair.n=normal_a_reverse->pair.n;
        q->prec->prec=NULL;
        q=q->prec;
    }
    if (first_num>=normal_b_reverse->pair.n){
        res->pair.n=1;
        /*while (true){
            res->pair.n+=1;
            printf("res=%ld\n",res->pair.n);
            LINK* mnog;
            mnog=alghorithm_multyplication(normal_b,res);
            printf("MNOG\n");
            char* check=Return_in_decimal_system(mnog);
            for(int i=0;i<strlen(check);i++) printf("%c",check[i]);
            printf("\n");
            if (Compare(q,mnog)==-1){
                res->pair.n-=1;
                break;
            }
            else if (Compare(q,mnog)==0) break;
        }*/
        LINK* mnog;
        mnog=alghorithm_multyplication(normal_b,res);
        q=SUB_LINK(q,mnog);
    }
    else{
        LINK* delta;
        delta=(LINK*)malloc(sizeof(LINK));
        delta->ptype='d';
        delta->foll=NULL;
        delta->prec=NULL;
        unsigned long long num=(first_num<<32)/normal_b_reverse->pair.n;
        unsigned long long num2=second_num/normal_b_reverse->pair.n;
        num=num+num2+5;
        if (num<=base_without_1) {
            delta->pair.n=num;
        }
        else delta->pair.n=base_without_1;
        while (true){
            LINK* mnog;
            mnog=alghorithm_multyplication(delta,normal_b);
            if (Compare(mnog,q)==1) delta->pair.n-=1;
            else break;
        }
        res->pair.n=delta->pair.n;
        LINK* mnog;
        mnog=alghorithm_multyplication(res,normal_b);
        q=SUB_LINK(q,mnog);
        free(delta);
    }
    if (normal_a_reverse->prec==NULL) {
        if (((a->ptype=='d')&&(b->ptype=='n'))||((a->ptype=='n')&&(b->ptype=='d'))) res->ptype='n';
        return res;
    }
    while (normal_a_reverse!=NULL){
        normal_a_reverse=normal_a_reverse->prec;
        if (normal_a_reverse==NULL) break;
        q->prec=(LINK*)malloc(sizeof(LINK));
        q->prec->ptype='d';
        q->prec->foll=q;
        q->prec->pair.n=normal_a_reverse->pair.n;
        q->prec->prec=NULL;
        q=q->prec;
        LINK* reverse_q=reverse(q);
        first_num=reverse_q->pair.n;
        if (reverse_q->prec==NULL) second_num=0;
        else second_num=reverse_q->prec->pair.n;
        if (Compare(q,normal_b)==-1){
            res->prec=(LINK*)malloc(sizeof(LINK));
            res->prec->ptype='d';
            res->prec->foll=res;
            res->prec->prec=NULL;
            res->prec->pair.n=0;
            res=res->prec;
            continue;
        }
        if (first_num>=normal_b_reverse->pair.n){
            res->prec=(LINK*)malloc(sizeof(LINK));
            res->prec->ptype='d';
            res->prec->foll=res;
            res->prec->prec=NULL;
            res->prec->pair.n=1;
            res=res->prec;
            LINK* for_one_num;
            for_one_num=(LINK*)malloc(sizeof(LINK));
            for_one_num->ptype='d';
            for_one_num->prec=NULL;
            for_one_num->foll=NULL;
            for_one_num->pair.n=1;
            LINK* mnog;
            mnog=alghorithm_multyplication(normal_b,for_one_num);
            free(for_one_num);
            q=SUB_LINK(q,mnog);
            continue;
        }
        else{
            LINK* delta;
            delta=(LINK*)malloc(sizeof(LINK));
            delta->foll=NULL;
            delta->ptype='d';
            delta->prec=NULL;
            delta->pair.n=0;
            unsigned long long num=(first_num<<32)/normal_b_reverse->pair.n;
            unsigned long long num2=second_num/normal_b_reverse->pair.n;
            num=num+num2+5;
            if (num<=base_without_1) {
                delta->pair.n=num;
            }
            else delta->pair.n=base_without_1;
            while (true){
                LINK* mnog;
                mnog=alghorithm_multyplication(delta,normal_b);
                if (Compare(mnog,q)==1){
                    delta->pair.n-=1;
                } 
                else break;
            }
            res->prec=(LINK*)malloc(sizeof(LINK));
            res->prec->ptype='d';
            res->prec->foll=res;
            res->prec->prec=NULL;
            res->prec->pair.n=delta->pair.n;
            res=res->prec;
            LINK* mnog;
            mnog=alghorithm_multyplication(delta,normal_b);
            q=SUB_LINK(q,mnog);
        }
    }
    if (((a->ptype=='d')&&(b->ptype=='n'))||((a->ptype=='n')&&(b->ptype=='d'))) res->ptype='n';
    return res;
    
}

//Я БОЛЬШЕ НЕ МОГУ(((((((((((((
LINK* FIND_MOD(LINK*num,LINK* mod){
    LINK*q;
    char* check;

    q=Div_in_32_system(num,mod);

    q=alghorithm_multyplication(q,mod);
    q=SUB_LINK(num,q);
    return q;
}

LINK* FREE_MEMBER(LINK*p){
    while (p->foll!=NULL){
        LINK* q=p->foll;
        free(p);
        p=q;
    }
    free(p);
    return NULL;
}

LINK* COPY_LINK_1(LINK*p){
    if (p==NULL) return NULL;
    LINK* res;
    res=(LINK*)malloc(sizeof(LINK));
    res->foll=NULL;
    res->prec=NULL;
    res->ptype=p->ptype;
    res->pair.n=p->pair.n;
    LINK* dop_res=res;
    p=p->foll;
    while (p!=NULL){
        dop_res->foll=(LINK*)malloc(sizeof(LINK));
        dop_res->foll->foll=NULL;
        dop_res->foll->pair.n=p->pair.n;
        dop_res->foll->prec=dop_res;
        dop_res->foll->ptype=p->ptype;
        dop_res=dop_res->foll;
        p=p->foll;
    }
    return res;
}



//а-число b-модуль

LINK* extended_euclid(LINK*a, LINK*b){
    if ((a==NULL)||(b==NULL)) return NULL;
    LINK* x=NULL;
    LINK* d=NULL;
    LINK* Zerrow=NULL;
    Zerrow=(LINK*)malloc(sizeof(LINK));
    Zerrow->ptype='d';
    Zerrow->foll=NULL;
    Zerrow->prec=NULL;
    Zerrow->pair.n=0;
    if ((Size_Link(b)==1)&&(b->pair.n)==0){
        if ((Size_Link(a)==1)&&(a->pair.n==1)){
            LINK* res=(LINK*)malloc(sizeof(LINK));
            res->foll=NULL;
            res->prec=NULL;
            res->ptype='d';
            res->pair.n=1;
            return res;
        }
        else{
            printf("ERROR!\n");
            LINK* res=(LINK*)malloc(sizeof(LINK));
            res->foll=NULL;
            res->prec=NULL;
            res->ptype='d';
            res->pair.n=0;
            return res;
        }
    }
    LINK* x2=(LINK*)malloc(sizeof(LINK));
    x2->foll=NULL;
    x2->pair.n=1;
    x2->prec=NULL;
    x2->ptype='d';

    LINK* x1=(LINK*)malloc(sizeof(LINK));
    x1->foll=NULL;
    x1->pair.n=0;
    x1->prec=NULL;
    x1->ptype='d';

    LINK* q=NULL;
    LINK* r=NULL;

    LINK* first_num=COPY_LINK_1(a);
    LINK* second_num=COPY_LINK_1(b);

    while (true){
        if ((second_num!=NULL)&&(second_num->ptype=='n')) break;
        if ((second_num!=NULL)&&(Size_Link(second_num)==1)&&(second_num->pair.n==0)) break;
        //if (q!=NULL) q=FREE_MEMBER(q);
        q=Div_in_32_system(first_num,second_num);
        //if (r!=NULL) r=FREE_MEMBER(r);
        r=alghorithm_multyplication(q,second_num);
        r=SUB_LINK(first_num,r);
        //if (x!=NULL) x=FREE_MEMBER(x);
        x=alghorithm_multyplication(q,x1);
        x=SUB_LINK(x2,x); 
        first_num=second_num;
        //first_num=FREE_MEMBER(first_num);
        //first_num=COPY_LINK_1(second_num);
        //second_num=FREE_MEMBER(second_num);
        //second_num=COPY_LINK_1(r);
        second_num=r;
        //x2=FREE_MEMBER(x2);
        //x2=COPY_LINK_1(x1);
        x2=x1;
        x1=x;
        //x1=FREE_MEMBER(x1);
        //x1=COPY_LINK_1(x);     
    }
    while (x2->ptype=='n'){
       x2=ADD_LINK(x2,b);
    }
    if ((Size_Link(first_num)==1)&&(first_num->pair.n==1)) return x2;
    printf("ERRRRRRRRRROR!!!!\n");
    LINK* res=(LINK*)malloc(sizeof(LINK));
    res->foll=NULL;
    res->prec=NULL;
    res->ptype='d';
    res->pair.n=0;
    return res;
}




typedef struct M_i{
	LINK* num;
    struct M_i* next;
}M_I;

LINK* FIND_M(M_I* m){
    LINK* res=m->num;
    m=m->next;
    while (m!=NULL){
        res=alghorithm_multyplication(res,m->num);
        m=m->next;
    }
    return res;
}


M_I* FIND_ALL_R_I(LINK* multy, M_I* m_i){
    if ((multy==NULL)||(m_i==NULL)) return NULL;
    M_I* res=(M_I*)malloc(sizeof(M_I));
    res->num=FIND_MOD(multy,m_i->num);
    res->next=NULL;
    M_I* dop_res=res;
    m_i=m_i->next;
    while (m_i!=NULL){
        dop_res->next=(M_I*)malloc(sizeof(M_I));
        //dop_res->next->num=m_i->num;
        dop_res->next->num=FIND_MOD(multy,m_i->num);
        dop_res->next->next=NULL;
        m_i=m_i->next;
        dop_res=dop_res->next;
    }
    return res;
} 

LINK* chinese_theorema(M_I* m_i,M_I* r_i,LINK* M){
    LINK* x=(LINK*)malloc(sizeof(LINK));
    x->foll=NULL;
    x->pair.n=0;
    x->prec=NULL;
    x->ptype='d';
    while (m_i!=NULL){
        char* check;
        LINK* y_i=NULL;
        y_i=Div_in_32_system(M,m_i->num);
        LINK* s_i=extended_euclid(y_i,m_i->num);
        LINK* c_i=alghorithm_multyplication(r_i->num,s_i);
        c_i=FIND_MOD(c_i,m_i->num);
        LINK* x_dop=NULL;
        x_dop=alghorithm_multyplication(c_i,y_i);
        x_dop=FIND_MOD(x_dop,M);
        x=ADD_LINK(x,x_dop);
        m_i=m_i->next;
        r_i=r_i->next;
    }
    x=FIND_MOD(x,M);
    if ((Size_Link(x)!=1)&&(x->pair.n!=0)) x=DELETE_ZERROW(x);
    return x;

}

int generation_number(long degree){
    //unsigned long long quantity_numbers=find_quantity_of_num_for_generation();
    FILE* fp;
    char name[]="example.txt";
    if ((fp= fopen(name,"w"))==NULL){
        printf("Error!");
        return -1;
    }
    for (int j=0;j<2;j++){
        for (long i=0;i<degree;i++) fprintf(fp,"%d",rand()%10);
        fprintf(fp,"\n");
    }
    //Указывается вручную разложение модуля M
    fprintf(fp,"3\n7\n31\n127\n8191\n131071\n524287\n2147483647\n2305843009213693951");
    fclose(fp);
    return 0;
}

int Create_Output(long len,double time1, double time2,int num_of_iterations){
    FILE* fp;
    char name[]="output.txt";
    if ((fp= fopen(name,"w"))==NULL){
        printf("Error!");
        return -1;
    }
    double time=time1/num_of_iterations;
    fprintf(fp,"Len = %ld\n",len);
    fprintf(fp,"time standart alghorithm = %f\ntime Chinese alghotythm = %f",time,time2/num_of_iterations);
    fclose(fp);
    return 0;

}

int Create_Error_output(int num_of_iterations,LINK* first_number,LINK* second_number,LINK* res1,LINK* res2){
    FILE* fp;
    char name[]="output.txt";
    if ((fp= fopen(name,"w"))==NULL){
        printf("Error!");
        return -1;
    }

    fprintf(fp,"Error answer! Num of test = %d\n",num_of_iterations);
    fprintf(fp,"Num: ");
    char* check;
    check=Return_in_decimal_system(first_number);
    for (int i=0;i<strlen(check);i++)fprintf(fp,"%c",check[i]);
    fprintf(fp," ");
    check=Return_in_decimal_system(second_number);
    for (int i=0;i<strlen(check);i++)fprintf(fp,"%c",check[i]);
    fprintf(fp,"\nThe result of the standart alghorithm = ");
    check=Return_in_decimal_system(res1);
    for (int i=0;i<strlen(check);i++)fprintf(fp,"%c",check[i]);
    fprintf(fp,"\nThe result of the chinese alghorithm = ");
    check=Return_in_decimal_system(res2);
    for (int i=0;i<strlen(check);i++)fprintf(fp,"%c",check[i]);
    fclose(fp);
    return 0;    
}


int main(int argc,char** argv){
    if (argc<=1){
        printf("Error");
        return 0;
    }
    long a;
    a=strtol(argv[1],NULL,10);
    long num_of_repeat_program=strtol(argv[2],NULL,10);
    double time_spent_standart_alghorythm=0.0;
    double time_spent_chinese_alghorythm=0.0;
    long num_of_iterations=num_of_repeat_program;
    while (num_of_repeat_program){
        if (generation_number(a)==-1) return 0;
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
        num1=read_num_from_file_2(chastnoe,fp)->num;
        free(chastnoe);
        chastnoe=malloc(sizeof(char)*1);
        chastnoe[0]='\0';
        char* num2;
        num2=read_num_from_file_2(chastnoe,fp)->num;
        free(chastnoe);
        chastnoe=malloc(sizeof(char)*1);
        chastnoe[0]='\0';
        NUM_FILE* m_i=NULL;
        M_I* many_mod=NULL;
        m_i=read_num_from_file_2(chastnoe,fp);
        char* mod=m_i->num;
        many_mod=(M_I*)malloc(sizeof(M_I));
        many_mod->num=filling_structure(many_mod->num,NULL,mod,0,false,false);
        many_mod->next=NULL;
        M_I* dop_many_mod=many_mod;
        while (!m_i->flag){
            free(chastnoe);
            chastnoe=malloc(sizeof(char)*1);
            chastnoe[0]='\0';
            free(m_i);
            m_i=read_num_from_file_2(chastnoe,fp);
            free(mod);
            mod=m_i->num;
            dop_many_mod->next=(M_I*)malloc(sizeof(M_I));
            dop_many_mod->next->num=filling_structure(dop_many_mod->next->num,NULL,mod,0,false,false);
            dop_many_mod->next->next=NULL;
            dop_many_mod=dop_many_mod->next;
        }
        fclose(fp);
        char* check;
        LINK* M=FIND_M(many_mod);
        first_number=filling_structure(first_number,NULL,num1,0,false,false);
        LINK* second_number;
        second_number=filling_structure(second_number,NULL,num2,0,false,false);
        LINK* res;
        clock_t begin = clock ();
        res=alghorithm_multyplication(first_number,second_number);
        clock_t end = clock();
        time_spent_standart_alghorythm += (double)(end-begin)/CLOCKS_PER_SEC;
        check=Return_in_decimal_system(res);
        printf("Multyplication result = ");
        for (int i=0;i<strlen(check);i++) printf("%c",check[i]);
        printf("\n");
        //printf("\ntime = %f\n",time_spent);
        M_I* r_i=FIND_ALL_R_I(res,many_mod);
        
        LINK* res2;
        begin = clock ();
        res2=chinese_theorema(many_mod,r_i,M);
        end = clock();
        check=Return_in_decimal_system(res2);
        printf("CHINESE result = ");
        for (int i=0;i<strlen(check);i++) printf("%c",check[i]);
        time_spent_chinese_alghorythm += (double)(end-begin)/CLOCKS_PER_SEC;
        //printf("\ntime = %f\n\n",time_spent);
        printf("\n\n");
        if (Compare(res,res2)!=0){
            Create_Error_output(num_of_iterations-num_of_repeat_program,first_number,second_number,res,res2);
            return 0;
        }
        
        num_of_repeat_program-=1;
    }
    int output=Create_Output(a,time_spent_standart_alghorythm,time_spent_chinese_alghorythm,num_of_iterations);
    return 0;
}
