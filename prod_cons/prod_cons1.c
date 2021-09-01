#include <stdio.h>
#include <stdlib.h>  
#include <pthread.h> 
#include <unistd.h> 
#include <string.h>

#define BUFFFSIZE 255
static void* producer(void* );
static void* consumer(void* );
int allnumber;                      // 總共有幾題
int isfound=1 ;                     // 還沒找到答案
char QA_buff[BUFFFSIZE] = {'\0'};   // producer 和 consumer 都要從這裡拿資料

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct QA{
    char *Q;
    char *A;
    int number;
}QA;


int main(){
    srand(time(NULL));
    QA test[]={ {.Q = "James", .A = "Harden",   .number = 0},
                {.Q = "Kevin" , .A = "Love",    .number = 1},
                {.Q = "Ray"  , .A = "Allen",    .number = 2},
                {.Q = "Devin", .A = "book",     .number = 3}};
    allnumber = sizeof(test)/sizeof(test[0]);
    printf("allnumber:%d\n",allnumber);
   
    pthread_t prod,cons;
    if((pthread_create(&prod,NULL,consumer,&test)) != 0){
        perror("producer create:");
    }
     if((pthread_create(&cons,NULL,producer,&test)) != 0){
        perror("consumer create:");
    }

    pthread_join(prod,NULL);
    pthread_join(cons,NULL);
   
}

static void* producer(void *p){
    QA *qa_prod = (QA*) p;
    while(1){
        pthread_mutex_lock(&mutex);
        int i =rand() %allnumber;
        snprintf(QA_buff,BUFFFSIZE,"%s",qa_prod[i].Q);
        printf("Q: %s\n",QA_buff);
        while(isfound > 0){
            //printf("%s\n","wait");
            sleep(1);
            pthread_cond_wait(&cond,&mutex);
        }
        printf("A: %s\n",QA_buff);
        isfound=1;
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

static void* consumer(void *p){
    QA *qa_cons = (QA*) p;
    while(1){
        pthread_mutex_lock(&mutex);
        for(int k=0; k< allnumber ;k++)
        {
            if( (strcmp(QA_buff,qa_cons[k].Q)) == 0){
                snprintf(QA_buff,BUFFFSIZE,"%s",qa_cons[k].A);
                isfound = -1;
                pthread_cond_signal (&cond);
                //printf("%s\n","signal");
                break;
            }
        }
        //printf("%s\n",QA_buff);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}