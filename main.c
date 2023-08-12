#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#define INT_MAX 18446744073709551615

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



//Функция чтения из файла
//Для запуска необходимо создать char* chastnoe, а также выделить ему память
//также должен быть запущен файл, из которого берем тестовые данные 
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
    unsigned long num=0;
    int dop_i=i;
    for (i;(i<dop_i+32)&&(i<strlen(binary));i++){
        num+=(binary[i]-'0')*multiplier;
        multiplier=multiplier<<1;
    }
    p->pair.n=num;
    if (i<(strlen(binary)-1)) p->foll=filling_structure(p->foll,p,binary,i,!flag_chetn,false);
    else if (!flag_chetn) p->foll=filling_structure(p->foll,p,binary,i,!flag_chetn,true);
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
            mnog_num=mnog_num/4294967296;
            res=Copy_and_add(res,mnog_num+'0');
            unsigned long long chastnoe_num=strtoull(chastnoe,NULL,10);
            chastnoe_num-=mnog_num*4294967296;
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
        mnog_num=mnog_num/4294967296;
        res=Copy_and_add(res,mnog_num+'0');
        unsigned long long chastnoe_num=strtoull(chastnoe,NULL,10);
        chastnoe_num-=mnog_num*4294967296;
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


//Реализация алгоритма умножения в системе счисления 2^32
LINK* alghorithm_multyplication(LINK* first_num,LINK* second_num){
    LINK* res=NULL;
    res=(LINK*)malloc(sizeof(LINK));
    res->foll=NULL;
    res->prec=NULL;
    res->pair.n=0;
    unsigned long long size_first_num=Size_Link(first_num);
    unsigned long long size_second_num=Size_Link(second_num);
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
                printf("mnog=%lld\n",mnog);
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
    return res;
}

LINK* reverse(LINK* p){
    while (p->foll->foll!=NULL) p=p->foll;
    return p->foll;
}


//return 1 if p>q
//return 0 if p=q
//return -1 p<q
int Compare(LINK* p, LINK* q){
    int size_p=Size_Link(p);
    int size_q=Size_Link(q);
    if (size_p>size_q) return 1;
    if (size_p<size_q) return -1;
    LINK* reverse_p=reverse(p);
    LINK* reverse_q=reverse(q);
    while (reverse_p!=NULL){
        if (reverse_p->pair.n>reverse_q->pair.n) return 1;
        if (reverse_p->pair.n<reverse_q->pair.n) return -1;
        reverse_p=reverse_p->prec;
        reverse_q=reverse_q->prec;
    }
    return 0;
}


//Вспомогательная! нужна для алгоритма деления в 2^32 системе счисления
//Функция обрабатывает только случаи, когда p>q 
LINK* SUB_LINK(LINK*p, LINK*q){
    if (Compare(p,q)==-1) return NULL;
    bool flag =false;
    LINK* res=(LINK*)malloc(sizeof(LINK));
    res->pair.n=0;
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
            if (flag) dop_for_res->pair.n-=1;
        }
        p=p->foll;
        if (q->foll!=NULL){
            dop_for_res->foll=(LINK*)malloc(sizeof(LINK));
            dop_for_res->foll->pair.n=0;
            dop_for_res->foll->foll=NULL;
            dop_for_res->foll->prec=dop_for_res;
        }
        q=q->foll;
        dop_for_res=dop_for_res->foll;
    }
    return res;
}

LINK* Div_in_32_system(LINK*a, LINK* b){
    if ((a==NULL)||(b==NULL)) return NULL;
    LINK* an=reverse(a);
    LINK* bn=reverse(b);
    LINK* normal_a;
    LINK* normal_b;
    LINK* vector_for_normalization=NULL;
    vector_for_normalization=(LINK*)malloc(sizeof(LINK));
    vector_for_normalization->prec=NULL;
    unsigned long long base=4294967296;
    vector_for_normalization->pair.n=(unsigned long) (base/(bn->pair.n+1));
    vector_for_normalization->foll=(LINK*)malloc(sizeof(LINK));
    vector_for_normalization->foll->foll=NULL;
    vector_for_normalization->foll->prec=vector_for_normalization;
    vector_for_normalization->foll->pair.n=0;
    normal_a = alghorithm_multyplication(a,vector_for_normalization);
    normal_b=alghorithm_multyplication(b,vector_for_normalization);
    printf("\n");
    Print_Link(normal_b);
    printf("\n");
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
    first_number=filling_structure(first_number,NULL,num1,0,false,false);
    Print_Link(first_number);
    printf("\n");
    LINK* second_number;
    second_number=filling_structure(second_number,NULL,num2,0,false,false);
    Print_Link(second_number);
    printf("\n");
    LINK* res;
    double time_spent=0.0;
    clock_t begin = clock ();
    res=alghorithm_multyplication(first_number,second_number);
    clock_t end = clock();
    time_spent += (double)(end-begin)/CLOCKS_PER_SEC;
    //Printf_in_sixteenfour_system(res);
    //printf("\n");
    char* check;
    check=Return_in_decimal_system(res);
    for (int i=0;i<strlen(check);i++) printf("%c",check[i]);
    printf("\n");
    printf("\ntime = %f\n",time_spent);
    //reverse(res);
    //printf("\n");
    //Print_Link(res);
    //LINK* check_div;
    //check_div=Div_in_32_system(first_number,second_number);
    //printf("%d",Compare(first_number,second_number));
    /*
    printf("\n........2^64...........\n");
    Printf_in_sixteenfour_system(first_number);
    printf("\n");
    Printf_in_sixteenfour_system(second_number);
    */
    //reverse(res);
    return 0;
}
