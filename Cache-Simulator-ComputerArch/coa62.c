#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define max 10000
int csize,bsize,asso;
    char a[max],b[max];
    int hit;
    int miss;
struct node{
    int itsset;
    char tag[max];
    int num;
};
char binary[max];
struct node *cache[max][max];
int inset[max];
int inf;
int y;

struct node* createnode(int i, char t[]) {
    struct node* temp = malloc(sizeof(struct node));
    if (temp != NULL) {
        temp->itsset = i;
        strcpy(temp->tag, t);
        temp->num=0;
      //  printf("returned %d",temp->itsset);
        return temp; // Return the allocated memory pointer
    } else {
        // Handle malloc failure if needed
        printf("Memory allocation failed in createnode\n");
        exit(EXIT_FAILURE);
    }
}

int mininset(int sets){
    int current=10000;
    int in;
    for(int i=0;i<sets;i++){
        if(inset[i]<current){
            current=inset[i];
            in=i;
        }
    }
    return in;
}
char sih[4];
void dth(int s){
    for(int i=4;i>=0;i--){
        sih[i]='0';
    }
    int count=4;
    int remainder;
    while(s!=0){
        remainder=s%16;
        if(remainder<10){
            sih[count]=remainder+'0';
        }
        else{
            sih[count]=remainder-10+'A';
        }
        count--;
        s=s/16;
    }

}

char tagh[8];
/*void taginh(char tag[]){
    char binaryt[max];
    //char hex[max];
    char *a;
    unsigned long dec=strtoul(binary,&a,2);
    snprintf(tagh,sizeof(tagh),"%lX",dec);
}*/

void taginh(char tag[]){
    int len = strlen(tag);
    int padding=len%4;
    int new_len=len+(padding == 0 ? 0 : (4 - padding));
    
    char* pad=(char*)malloc((new_len + 1)*sizeof(char));
    strcpy(pad,tag);
    while (strlen(pad)%4!=0){
        memmove(pad+1,pad,strlen(pad)+1);
        pad[0]='0';
    }
    unsigned int num = strtoul(pad,NULL,2);
    sprintf(tagh,"%X",num); 
    
    free(pad);
}



void insert(int sets,char buffer[]){
   // printf("%d ss",sets);
//    exit(1);
    char setno[max];
    int offbits=log2(bsize);
    for(int i=sets-1;i>=0;i--){
        setno[sets-i-1]=binary[31-offbits-i];
    }
    //setno[sets]='\0';
    char tag[max];
    if(asso!=0){
    for(int i=0;i<32-offbits-sets;i++){
        tag[i]=binary[i];
    }
    }
    else{
        for(int i=0;i<32-offbits;i++){
        tag[i]=binary[i];
    }
    }
  //  exit(1);
    //charbinaryString[] = "101010"; // Replace this string with your binary number
    int s = strtol(setno, NULL, 2);
    int po=strlen(tag);
    dth(s);
    taginh(tag);
   // printf("%s %dss",setno,po);
   // exit(1);
//exit(1);
   // printf("Binary: %s\nDecimal: %ld\n", binaryString, decimalNumber);
   //int con=0;
   if(asso!=0){
   for(int i=0;i<inset[s];i++){
    struct node* temp=cache[s][i];
    if(temp!=NULL && strcmp(temp->tag,tag)==0){
        //struct node* temp=cache[s][i];
        printf("address 0x%s,its set no:0x%s, hit, tag 0x%s\n",buffer,sih,tagh);
        if(strcmp(a,"LRU")==0){
        for(int j=i;j<inset[s]-1;j++){
            cache[s][j]=cache[s][j+1];
        }
        cache[s][inset[s]-1]=temp;
       // inset[s]=inset[s]+1;
    }
    hit++;
    return;
    }
   }
   miss++;
   printf("address 0x%s,its set no:0x%s, miss, tag 0x%s\n",buffer,sih,tagh);
   if(strcmp(b,"WT")!=0 || buffer[0]!='W'){
   if(inset[s]<asso){
        int l=inset[s];
        inset[s]=inset[s]+1;
   //     printf("%d %d aa\n",s,l);
        cache[s][l]=createnode(s,tag);
       // exit(1);
   }
   else if(strcmp(a,"LRU")==0 || strcmp(a,"FIFO")==0){
    for(int i=0;i<asso-1;i++){
        cache[s][i]=cache[s][i+1];
    }
    cache[s][asso-1]=createnode(s,tag);
   }
   /*else if(strcmp(a,"FIFO")==0){
    for(int i=0;i<bsize-1;i++){
        cache[s][i]=cache[s][i+1];
    }
    cache[s][i+]
   }*/
   else if(strcmp(a,"RANDOM")==0){
        //printf("aa\n");
        srand(time(0));
        int random=rand();
        int min=0;
        int maxi=asso-1;
        random=rand()%(maxi-min+1)+min;
        cache[s][random]=createnode(s,tag);
   }


}}
/*    else{
        int y=0;
        for(int s=0;s<sets;s++){
        for(int i=0;i<inset[s];i++){
    struct node* temp=cache[s][i];
    if(temp!=NULL && strcmp(temp->tag,tag)==0){
        //struct node* temp=cache[s][i];
      //  printf("cache hit for address %s,its set no:%d,tag %s\n",buffer,s,binary);
        if(strcmp(a,"LRU")==0){
        for(int j=i;j<inset[s]-1;j++){
            cache[s][j]=cache[s][j+1];
        }
        temp->num=y;
        y++;
        cache[s][inset[s]-1]=temp;
       // inset[s]=inset[s]+1;
    }
    hit++;
    return;
    }}
   }
   miss++;
   if(strcmp(b,"WT")!=0 || buffer[0]!='W'){
   s=mininset(sets);
   if(inset[s]<bsize){
        int l=inset[s];
        inset[s]=inset[s]+1;
   //     printf("%d %d aa\n",s,l);
        y++;
        cache[s][l]=createnode(s,tag);
        cache[s][l]->num=y;
        y++;
       // exit(1);
   }
   else if(strcmp(a,"LRU")==0 || strcmp(a,"FIFO")==0){
    int h=10000;
    for(int i=0;i<sets;i++){
        if(cache[i][0]->num<h){
            h=cache[i][0]->num;
            s=i;
        }
    }
    for(int i=0;i<bsize-1;i++){
        cache[s][i]=cache[s][i+1];
    }
    cache[s][bsize-1]=createnode(s,tag);
    cache[s][bsize-1]->num=y;
    y++;
   }
   /*else if(strcmp(a,"FIFO")==0){
    for(int i=0;i<bsize-1;i++){
        cache[s][i]=cache[s][i+1];
    }
    cache[s][i+]
   }
   else if(strcmp(a,"RANDOM")){
        printf("aa\n");
        srand(time(0));
        int random=rand();
        srand(time(0));
        int random0=rand();
        int min=0;
        int maxi=asso-1;
        random=rand()%(maxi-min+1)+min;
        random0=rand()%(maxi-min+1)+min;
        cache[random0][random]=createnode(s,tag);
        cache[random0][random]->num=y;
        y++;
   }


}
    }*/
else{
    for(int i=0;i<inf;i++){
        struct node* temp=cache[0][i];
        if(temp!=NULL && strcmp(temp->tag,tag)==0){
            if(strcmp(a,"LRU")==0){
                for(int j=i;j<inf;j++){
                    cache[0][j]=cache[0][j+1];
                }
                temp->num=y;
                y++;
                cache[0][inf-1]=temp;
            }
            printf("address 0x%s,its block no.%d, hit, tag 0x%s\n",buffer,i,tagh);
            hit++;
            return;
        }
    }
    miss++;
    printf("address 0x%s, miss, tag 0x%s\n",buffer,tagh);
    int blocks=csize/bsize;
    if(strcmp(b,"WT")!=0 || buffer[0]!='W'){
    if(inf<blocks){
        cache[0][inf]=createnode(0,tag);
        cache[0][inf]->num=y;
        y++;
        inf++;
    }
    else if(strcmp(a,"LRU")==0 || strcmp(a,"FIFO")==0){
        for(int i=0;i<inf-1;i++){
            cache[0][i]=cache[0][i+1];
         //   cache[0][inf-1]=createnode(0,tag);
            cache[0][inf-1]->num=y;
            y++;
        }
        cache[0][inf-1]=createnode(0,tag);
    }
    else if(strcmp(a,"RANDOM")==0){
       // printf("aa\n");
        srand(time(0));
        int random=rand();

        int min=0;
        int maxi=blocks-1;
        random=rand()%(maxi-min+1)+min;
        cache[0][random]=createnode(0,tag);
   }
}
}}

void address(char buffer[],int sets){
   // exit(1);
    int l=strlen(buffer);
    for(int i=0;i<32;i++){
        binary[i]='0';
    }
    int i=7;
    for(int count=l-1;count>=5;count--){
        char a=buffer[count];
        if(a=='0'){
        binary[4*i+0]='0';
        binary[4*i+1]='0';
        binary[4*i+2]='0';
        binary[4*i+3]='0';
    }
    else if(a=='1'){
        binary[4*i+0]='0';
        binary[4*i+1]='0';
        binary[4*i+2]='0';
        binary[4*i+3]='1';
    }
    else if(a=='2'){
        binary[4*i+0]='0';
        binary[4*i+1]='0';
        binary[4*i+2]='1';
        binary[4*i+3]='0';
    }
    else if(a=='3'){
        binary[4*i+0]='0';
        binary[4*i+1]='0';
        binary[4*i+2]='1';
        binary[4*i+3]='1';
    }
    else if(a=='4'){
        binary[4*i+0]='0';
        binary[4*i+1]='1';
        binary[4*i+2]='0';
        binary[4*i+3]='0';
    }
    else if(a=='5'){
        binary[4*i+0]='0';
        binary[4*i+1]='1';
        binary[4*i+2]='0';
        binary[4*i+3]='1';
    }
    else if(a=='6'){
        binary[4*i+0]='0';
        binary[4*i+1]='1';
        binary[4*i+2]='1';
        binary[4*i+3]='0';
    }
    else if(a=='7'){
        binary[4*i+0]='0';
        binary[4*i+1]='1';
        binary[4*i+2]='1';
        binary[4*i+3]='1';
    }
    else if(a=='8'){
        binary[4*i+0]='1';
        binary[4*i+1]='0';
        binary[4*i+2]='0';
        binary[4*i+3]='0';
    }
    else if(a=='9'){
        binary[4*i+0]='1';
        binary[4*i+1]='0';
        binary[4*i+2]='0';
        binary[4*i+3]='1';
    }
    else if(a=='a'){
        binary[4*i+0]='1';
        binary[4*i+1]='0';
        binary[4*i+2]='1';
        binary[4*i+3]='0';
    }
    else if(a=='b'){
        binary[4*i+0]='1';
        binary[4*i+1]='0';
        binary[4*i+2]='1';
        binary[4*i+3]='1';
    }
    else if(a=='c'){
        binary[4*i+0]='1';
        binary[4*i+1]='1';
        binary[4*i+2]='0';
        binary[4*i+3]='0';
    }
    else if(a=='d'){
        binary[4*i+0]='1';
        binary[4*i+1]='1';
        binary[4*i+2]='0';
        binary[4*i+3]='1';
    }
    else if(a=='e'){
        binary[4*i+0]='1';
        binary[4*i+1]='1';
        binary[4*i+2]='1';
        binary[4*i+3]='0';
    }
    else if(a=='f'){
        binary[4*i+0]='1';
        binary[4*i+1]='1';
        binary[4*i+2]='1';
        binary[4*i+3]='1';
    }
    i--;
    }
    //printf("%s aa",binary);
    insert(sets,buffer);
}
    
    int main(){
        hit=0;
        miss=0;
        inf=0;
        y=0;
        for(int i=0;i<max;i++){
            for(int j=0;j<max;j++){
                cache[i][j]=NULL;
            }
        }
        for(int i=0;i<max;i++){
            inset[i]=0;
        }
    printf("enter input format conditions: ");

    scanf("%d %d %d %s %s",&csize,&bsize,&asso,a,b);
    int sets;
    if(asso!=0){
    int ss=csize/(bsize*asso);
    sets=log2(ss);
    }
    else{
        int ss=csize/(bsize*bsize);
        sets=log2(ss);

    }


    FILE *file=fopen("trace(1).txt","r");
    char buffer[max];
    while(fgets(buffer,sizeof(buffer),file)!=NULL){
        int ccc=strlen(buffer);
        buffer[ccc-1]='\0';
        ccc=strlen(buffer);
     //   printf("%s %d\n",buffer,ccc);
        address(buffer,sets);
    }
    printf("TOTAL MISS=%d, TOTAL HIT=%d",miss,hit);
}