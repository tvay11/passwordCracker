#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "crypt.h"


char alphabet[27]="abcdefghijklmnopqrstuvwxyza";
//the last a is for breaking the code when it's finished
// ONLY WORK IF ITS NOT A TO Z  shouldnt matter

struct crack{
    int start;
    int end;
    int keySize;
    char* target;
    char* salt;
    int threadid;
}*args;
void* iterate3( int keySize,const char* target,char* salt) {
    int place = keySize - 1;
    char* candidate;
    candidate[0]='a';
    candidate[1]='\n';

    while (1) {
        char *result = crypt(candidate, salt);
        if (strcmp(target, result) == 0) {
            printf("%s\n", candidate);
            exit(1);
        }
        if (place == 0 && candidate[0] == 'z') {
            candidate[place] = 'a';
            break;
        }
        if (candidate[place] == 'z') {
            candidate[place] = 'a';
            place--;
        }
        if (candidate[place] != 'z') {
            candidate[place]++;
            if (place < keySize - 1) {
                place++;
            }
        }

    }
}

//pthread_mutex_t t;
void* iterate2( void *arguments) {
    //pthread_mutex_lock(&t);
    struct crack *args = arguments;
    int place = args->keySize - 1;
    int start = args->start;
    int keySize = args->keySize;
    int end = args->end;
    char *salt = args->salt;
    char *target = args->target;
    char *candidate = (char *) malloc(1 * sizeof(char));
    //printf("%c------%c-------%d\n",alphabet[start],alphabet[end],args->threadid);
    candidate[0] = alphabet[start];
    candidate[1]='\n';
    struct crypt_data data;
    data.initialized = 0;
    while (candidate[0]!=alphabet[end+1]) {
        char *result = crypt_r(candidate, salt,&data);
        //printf("%s      %s       %d\n", candidate, result,args->threadid);
        if (strcmp(target, result) == 0) {;
            printf("%s\n", candidate);
            exit(1);
        }
        if (place == 0 && candidate[0] == alphabet[end+1]) {
            //don't know if needed - use for testing
            candidate[place] = 'a';
            printf("break");
            break;
        }
        if (candidate[place] == 'z') {
            candidate[place] = 'a';
            place--;
        }
        if (candidate[place] != 'z') {
            candidate[place]++;
            if (place < keySize - 1) {
                place++;
            }
        }
    }
    //printf("thread finished %d\n",args->threadid);
    //pthread_mutex_unlock(&t);
}
void *ph(int thread, int keysize, char *target, char *salt) {
    int curen = 26 / thread;
    int divide=curen-1;
    pthread_t th[thread-1];
    int firstN = 0;
    int lastN = firstN+divide;
    struct crack ce[thread];
    for (int i = 0; i < thread ; i++) {
        if(i==thread-1){
            lastN=25;
        }
        if (lastN > 25) { // dont know if needed
            lastN = 25;
        }
        ce[i].start = firstN;
        ce[i].end = lastN;
        ce[i].keySize = keysize;
        ce[i].target = target;
        ce[i].salt = salt;
        ce[i].threadid = i;
        if(pthread_create(&th[i], NULL, &iterate2, &ce[i])!=0){
            perror("error creating");
            exit(1);
        }
        firstN = lastN+1;
        lastN = firstN + divide;
    }

    for (int i = 0; i < thread ; i++) {
        if(pthread_join(th[i], NULL)!=0){
            perror("error joining %d\n");
        }
    }

}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("CAN ONLY ENTER 4 ARGS");
        exit(1);
    }
    char *l = argv[1];
    int threads = strtol(l, NULL, 10);
    char *k = argv[2];
    int keysize = strtol(k, NULL, 10);
    if (keysize > 8) {
        perror("keysize need to be less than 8");
        exit(1);
    }
    char *string3;
    string3 = argv[3];
    char salt[2];
    salt[0] = string3[0];
    salt[1] = string3[1];
    if(threads==1){
        iterate3(keysize,argv[3],salt);
        exit(1);
    }
    ph(threads, keysize, argv[3], salt);

    return 0;

}
