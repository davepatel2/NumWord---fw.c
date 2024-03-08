#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define STARTMEM 200
char *getword(FILE *stream);
int getindex(char *wordadd);
int strcmp(const char* str1, const char* str2);
void print_usage_and_exit();
void handle_argument(long *topamount, int *indexofarg , int argc, char *argv[]);
typedef struct nodes{
 long numof;
 char* word;
 struct nodes* nextnode;
} Node;
char *getword(FILE *stream){
    int startmem= 20;
    char *current;
    int index = 0;
    int letter;
    
    current = (char *)malloc(sizeof(char) * startmem);
    letter = fgetc(stream);
    if(isalpha(letter)!= 0){
        while(isalpha(letter)!= 0){
            

            letter = tolower(letter);
            *(current + index) = letter; 
            index++;
            

            if(index == startmem){
                startmem = (startmem *2) + 3;
                current = (char *)realloc(current, sizeof(char)* startmem);
    
            }
        letter = fgetc(stream);  

        }
    }else{
        if(letter == EOF && strlen(current)== 0){
            *(current) = EOF;
            index++;
            *(current + index)= '\0';
            return current;  
    }
    }

    *(current + index)= '\0';
    return current; 
    
    
}

int getindex(char *wordadd){
    int ind = 0; 
     
    int ascii = 0;
    char *currentchar = wordadd;
    while((*(currentchar + ind)) != '\0'){
        ascii += (int)(*(currentchar + ind));
        ind++;

    }
    ascii = ascii % STARTMEM;

    /* check if space is availbale or quadratic re-hash */
    
    
    return ascii; 

}
void print_usage_and_exit(){
    fprintf(stderr, "usage: fw [-n num] [file1 [file2 ...]\n");
    exit(-1);
}

void handle_argument(long *topamount,int *indexofarg , int argc, char *argv[]){
    *indexofarg = 0;
    if(argc == 1){
        *topamount = 10; 
        *indexofarg = 1;
        return;
    }
    if(strcmp(argv[1], "-n") == 0){
        if(argc > 2){
            char *ptr = NULL;
            *topamount = strtol(argv[2],&ptr, 0);
            if (*ptr == '\0'){
                *indexofarg = 3;
            }else{
                print_usage_and_exit();
            }

        }else{
        print_usage_and_exit();
    }  
    }else{
        *indexofarg = 1;
        *topamount = 10;
        return;
    }
}   

int main(int argc, char *argv[]){
    
    Node *currentnode;
    Node *beforenode;
    int initalizeindex = 0;
    int nindex =0; 
    int totalwords = 0;
    long topamount = 0;
    int indexofarg=0;
    FILE *file;
    
    Node hash[STARTMEM];
    while(initalizeindex != STARTMEM){
        (hash + initalizeindex) -> numof = 0;
        (hash + initalizeindex) -> word = NULL;
        (hash + initalizeindex) -> nextnode = NULL;
        initalizeindex++;
    }
    
    handle_argument(&topamount, &indexofarg, argc, argv);
    
    if(argv[indexofarg] == NULL){
        
        file = stdin;
    }else{
        file = fopen(argv[indexofarg], "r");
    }

    
    while(argv[indexofarg] != NULL || file == stdin){

        
        if(file ==stdin){
            
            ;
        }else{
            
            file = fopen(argv[indexofarg], "r");
            if(file == NULL){
                
                perror(argv[indexofarg]);
                indexofarg++;

                if(argv[indexofarg] == NULL || file ==stdin){ 
                    break;
                }else{
                    continue;
                }
            }
        }
        char *newword= getword(file);
        

    
        if(*newword == EOF){
            
            indexofarg++;

            if(argv[indexofarg] == NULL){ 
                break;
            }else{
                continue;
            }
            
        }


        while(*newword != EOF){
            
            
            
                if (newword[0]== '\0'){
                    
                    newword = getword(file);
                    continue;
                }
                nindex = getindex(newword);
                
                if(hash[nindex].word == NULL){
                    totalwords++;
                    hash[nindex].word = newword; 
                    hash[nindex].numof = 1;
                    hash[nindex].nextnode = NULL;
                    
                    
                    newword = getword(file);
                    
                    
                    continue;
                }
                if(strcmp(hash[nindex].word, newword) == 0){
                    
                    hash[nindex].numof++;
                    
                    newword = getword(file);
                    continue;
                } 
                currentnode = &hash[nindex];
                beforenode = &hash[nindex];
                
                        
                while(currentnode != NULL){
                    if(strcmp(currentnode -> word, newword) == 0){
                        
                        
                        currentnode -> numof++;
                        
                        break;
                    }
                    beforenode = currentnode; 
                    currentnode = currentnode -> nextnode;

                }
                if(currentnode == NULL){
                    Node *newnode =  (Node *)malloc(sizeof(Node));
                    newnode -> numof = 1;
                    newnode -> word = newword;
                    newnode -> nextnode = NULL;
                    beforenode -> nextnode = newnode;
                    
                    totalwords++;
                    newword = getword(file);
                    continue; 
                    
                    
                }
                if(strcmp(currentnode -> word, newword) == 0){
                    free(newword);
                    newword = getword(file);
                    continue;
                }
                newword = getword(file);

        }
        indexofarg++;

        if(argv[indexofarg] == NULL || file ==stdin){ 
            break;
        }
        
    } 
    printf("The top %ld words (out of %d) are:\n", topamount, totalwords);
    Node *curnode = &hash[0];
    Node *maxnode = curnode;
     
    int numofhash = 0; 
    while(topamount != 0){
        curnode = &hash[0];
        maxnode = curnode;
        
        numofhash = 0; 
        while(numofhash != 200){
            while (curnode != NULL){
            
            
            if (curnode -> numof > maxnode -> numof){
                maxnode = curnode;
            }

            if (curnode -> numof == maxnode -> numof){
                if(maxnode -> word != NULL){
                    if(strcmp(curnode-> word, maxnode -> word) > 0 ){  
                        maxnode = curnode;
                    }
                }   
            }


            if(curnode -> nextnode == NULL){
                break;
            }
            curnode = curnode -> nextnode;
        }
        
        numofhash++;
        if(numofhash > 200){
            break;
        }
        curnode = &hash[numofhash];
        
    }
    int count10=0;
    int multof10 = maxnode -> numof;
    while(multof10 != 0){
        multof10 /= 10;
        ++count10;
    }
    if(maxnode-> word != NULL){
        int realcount10 = 9 - count10;
        while (realcount10 !=0){
            printf(" ");
            realcount10--;
        }
        printf("%ld %s\n", maxnode -> numof, maxnode -> word);
    }
    
    maxnode -> numof =0; 
    (topamount)--;
}  
    return 0;
}

