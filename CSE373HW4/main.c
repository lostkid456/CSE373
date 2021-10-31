#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min=__INT_MAX__;
int *least=&min;

int number_found=0;
int *number_found_ptr=&number_found;

int set_len=0;

struct lst* poss_sol;

struct lst* sol;


struct lst{
    int val;
    struct lst *next;
};

void add_to_lst(struct lst** lst,int val){
    struct lst* new_node=malloc(sizeof(struct lst*));
    new_node->val=val;
    new_node->next=NULL;
    if(*lst==NULL){
        *lst=new_node;
        return;
    }
    struct lst* temp=*lst;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next=new_node;
}

void remove_last(struct lst** lst){
    if(*lst==NULL){
        return;
    }
    struct lst* temp=*lst;
    if(temp->next==NULL){
        *lst=NULL;
        return;
    }
    struct lst* prev;
    while(temp->next!=NULL){
        prev=temp;
        temp=temp->next;
    }
    free(prev->next);
    prev->next=NULL;
}

void print_lst(struct lst** lst){
    struct lst* temp=*lst;
    while(temp!=NULL){
        printf("%d ",temp->val);
        temp=temp->next;
    }
}

void find_set(struct lst* all_sets[],int index,int range,int sets,int length_of_sets[],int bool[]){
    if(isSolution(*number_found_ptr,range)){
        processSolution();
        return;
    }
    if(index==sets){
        return;
    }
    if(set_len>*least){
        return;
    }
    index++;
    int cand[sets];
    int nc=0;
    nc=construct_candidates(poss_sol,index,sets,cand,nc);
    for(int i=0;i<nc;i++){
        add_to_lst(&poss_sol,cand[i]);
        set_len++;
        int len=length_of_sets[cand[i]];
        struct lst* previous=malloc(sizeof(struct lst*));
        previous=NULL;
        struct lst* temp=all_sets[cand[i]];
        for(int j=0;j<len;j++){
            if(bool[temp->val]==-1){
                add_to_lst(&previous,temp->val);
                *number_found_ptr+=1;
                bool[temp->val]=1;
            }
            temp=temp->next;
        }
        find_set(all_sets,index,range,sets,length_of_sets,bool);
        remove_last(&poss_sol);
        set_len--;
        while(previous!=NULL){
            bool[previous->val]=-1;
            *number_found_ptr-=1;
            previous=previous->next;
        }
        free(previous);
    }
    if(isSolution(*number_found_ptr,range)){
        processSolution();
        return;
    }
}

int construct_candidates(struct lst* poss_sol,int index,int n,int c[],int nc){
    int in_combo[n];
    for(int i=0;i<n;i++){
        in_combo[i]=0;
    }
    for(int i=index;i<n;i++){
        if(in_combo[i]==0){
            c[nc]=i;
            nc++;
        }
    }
    return nc;
}

int isSolution(int number_found,int range){
    return number_found==range;
}

void processSolution(){
    if(set_len<*least){
        *least=set_len;
        while(sol!=NULL){
            remove_last(&sol);
        }
        struct lst* temp=poss_sol;
        for(int i=0;i<set_len;i++){
            add_to_lst(&sol,temp->val);
            temp=temp->next;
        }
    }
}



int main(){
    // range-> the universe: 5 would mean {1,...,5}
    // sets-> The number of sets in the input file 
    int range,sets;
    FILE *fp=fopen("/home/jz/CSE373HW4/TestCases/s-X-12-6","r");
    if(fp==NULL){
        return EXIT_FAILURE;
    }
    int bufferLength = 255;
    char buffer[bufferLength];

    range=atoi(fgets(buffer,bufferLength,fp));
    sets=atoi(fgets(buffer,bufferLength,fp)); 
    
    // The list of list holding all the sets 
    struct lst* all_sets[sets];

    // Holds the length of each set at their respective index 
    int length_of_sets[sets];
     
    int i=0;
    char* c; 
    // Goes through each line of the file to get the list of list 
    while((c=fgets(buffer,bufferLength,fp))!=NULL){
        int cnt=0;
        all_sets[i]=malloc(sizeof(struct lst*));
        all_sets[i]=NULL;
        char* line=strtok(c," ");
        while(line!=NULL){
            if(atoi(line)==0){
            }else{
                add_to_lst(&all_sets[i],atoi(line));
                cnt+=1;
            }
            line=strtok(NULL," ");
        }
        length_of_sets[i]=cnt;
        i++;
    }

    // Array for determining of a certain number in the universe is already found 
    // If set_bool_arr[i]!=-1, then it represents the index of the set it was found in
    // If set_bool_arr[i]=-1, then it means the number i is not yet found  
    // Ex: set_bool_arr[1] -> For S={1,...,N}, if the number 1 was found or not 
    int set_bool_arr[range+1];
    for(int i=1;i<range+1;i++){
        set_bool_arr[i]=-1;
    }

    poss_sol=malloc(sizeof(struct lst*));
    poss_sol=NULL;

    find_set(all_sets,-1,range,sets,length_of_sets,set_bool_arr);

    for(int i=0;i<*least;i++){
        print_lst(&all_sets[sol->val]);
        printf("\n");
        sol=sol->next;
    }

    printf("\nMin Set Cover Length: %d",*least);

    fclose(fp);

    return EXIT_SUCCESS;
}