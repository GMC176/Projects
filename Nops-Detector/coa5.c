#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 500
int r1;
int r;
char* get_reg(char var[]) {

        int k = 0, i = 0;
        char* reg = (char*)malloc(sizeof(char) * 5);
        while (var[k] != '(') k++;
        k++;
        while (var[k] != ')')       
            reg[i++] = var[k++];

        return reg;
}

void change(char a[]){
    if(strcmp(a,"zero")==0){
        strcpy(a,"x0");
    }
    else if(strcmp(a,"ra")==0){
        strcpy(a,"x1");
    }
    else if(strcmp(a,"sp")==0){
        strcpy(a,"x2");
    }
    else if(strcmp(a,"gp")==0){
        strcpy(a,"x3");
    }
    else if(strcmp(a,"tp")==0){
        strcpy(a,"x4");
    }
    else if(strcmp(a,"t0")==0){
        strcpy(a,"x5");
    }else if(strcmp(a,"t1")==0){
        strcpy(a,"x6");
    }else if(strcmp(a,"t2")==0){
        strcpy(a,"x7");
    }
    else if(strcmp(a,"s0")==0){
        strcpy(a,"x8");
    }else if(strcmp(a,"s1")==0){
        strcpy(a,"x9");
    }else if(strcmp(a,"a0")==0){
        strcpy(a,"x10");
    }else if(strcmp(a,"a1")==0){
        strcpy(a,"x11");
    }else if(strcmp(a,"a2")==0){
        strcpy(a,"x12");
    }else if(strcmp(a,"a3")==0){
        strcpy(a,"x13");
    }else if(strcmp(a,"a4")==0){
        strcpy(a,"x14");
    }else if(strcmp(a,"a5")==0){
        strcpy(a,"x15");
    }else if(strcmp(a,"a6")==0){
        strcpy(a,"x16");
    }else if(strcmp(a,"a7")==0){
        strcpy(a,"x17");
    }else if(strcmp(a,"s2")==0){
        strcpy(a,"x18");
    }else if(strcmp(a,"s3")==0){
        strcpy(a,"x19");
    }else if(strcmp(a,"s4")==0){
        strcpy(a,"x20");
    }else if(strcmp(a,"s5")==0){
        strcpy(a,"x21");
    }else if(strcmp(a,"s6")==0){
        strcpy(a,"x22");
    }else if(strcmp(a,"s7")==0){
        strcpy(a,"x23");
    }else if(strcmp(a,"s8")==0){
        strcpy(a,"x24");
    }else if(strcmp(a,"s9")==0){
        strcpy(a,"x25");
    }else if(strcmp(a,"s10")==0){
        strcpy(a,"x26");
    }else if(strcmp(a,"s11")==0){
        strcpy(a,"x27");
    }else if(strcmp(a,"t3")==0){
        strcpy(a,"x28");
    }else if(strcmp(a,"t4")==0){
        strcpy(a,"x29");
    }else if(strcmp(a,"t5")==0){
        strcpy(a,"x30");
    }else if(strcmp(a,"t6")==0){
        strcpy(a,"x31");
    }
    
}

int processf(char a[],char b[]){
    char *a1=strtok(a," ");
    char *a2=strtok(NULL," ");
    char *a3=strtok(NULL," ");
    char *a4=strtok(NULL," ");
    char *b1=strtok(b," ");
    char *b2=strtok(NULL," ");
    char *b3=strtok(NULL," ");
    char *b4=strtok(NULL," ");
    if(strcmp(a1,"ld") == 0 || strcmp(a1,"lb") == 0 || strcmp(a1,"lh") == 0 || strcmp(a1,"lw") == 0 || strcmp(a1,"lbu") == 0 ||strcmp(a1,"lhu") == 0 || strcmp(a1,"lwu") == 0){
      int l=strlen(a2);
        a2[l-1]='\0';
      //  a2=get_reg(a2);
        change(a2);
        if(strcmp(a2,"x0")==0){
            return 0;
        }
        if(strcmp(b1,"ld") == 0 || strcmp(b1,"lb") == 0 || strcmp(b1,"lh") == 0 || strcmp(b1,"lw") == 0 || strcmp(b1,"lbu") == 0 ||strcmp(b1,"lhu") == 0 || strcmp(b1,"lwu") == 0){
        b3 = get_reg(b3);
        change(b3);
        if(strcmp(b3,a2)==0){
            return 1;
        }}
        else if(strcmp(b1,"sd") == 0 || strcmp(b1,"sw") == 0 || strcmp(b1,"sb") == 0 || strcmp(b1,"sh") == 0 ){
        int l=strlen(b2);
        b2[l-1]='\0';
        change(b2);
        b3=get_reg(b3);
        change(b3);
        if(strcmp(a2,b2)==0 || strcmp(a2,b3)==0){
            return 1;
        }
    }
        else{
            int l=strlen(b3);
            b3[l-1]='\0';
            if(strcmp(b3,a2)==0){
                return 1;
            }
            else if(strcmp(b4,a2)==0){
                return 1;
            }

        }
    

        
    }        
    return 0;
}

int process(char a[],char b[]){
   /* printf("%s %s ddd abba",a,b);
    int al=strlen(a);
    a[al-1]='\0';
    int bl=strlen(b);
    b[bl-1]='\0';
    printf("%s %s ddd abba",a,b);*/
  //  printf("%s %s ddd abba",a,b);
    char *a1=strtok(a," ");
    char *a2=strtok(NULL," ");
    char *a3=strtok(NULL," ");
    char *a4=strtok(NULL," ");
    char *b1=strtok(b," ");
    char *b2=strtok(NULL," ");
    char *b3=strtok(NULL," ");
    char *b4=strtok(NULL," ");
    /*if (b4 != NULL) {b4[strlen(b4) - 1] = '\0';}
    else {b3[strlen(b3) - 1] ='\0';}
    if (a4 != NULL) {a4[strlen(a4) - 1] = '\0';}
    else {a3[strlen(a3) - 1] ='\0';}*/
    
    char source1[max]="a",dest[max],source2[max]="b";
    if(strcmp(a1,"ld") == 0 || strcmp(a1,"lb") == 0 || strcmp(a1,"lh") == 0 || strcmp(a1,"lw") == 0 || strcmp(a1,"lbu") == 0 ||strcmp(a1,"lhu") == 0 || strcmp(a1,"lwu") == 0){
        int l=strlen(a2);
        a2[l-1]='\0';
      //  a2=get_reg(a2);
        change(a2);
        strcpy(dest,a2);
    }
    else if(strcmp(a1,"sd") == 0 || strcmp(a1,"sw") == 0 || strcmp(a1,"sb") == 0 || strcmp(a1,"sh") == 0 ){
        strcpy(dest,"ssss");
    }
    else{
        int l=strlen(a2);
        a2[l-1]='\0';
        change(a2);
    
        if(strcmp(a2,"x0")==0){
            return 0;
        }
        // l=strlen(a3);
        // a3[l-1]='\0';
        // change(a3);
        strcpy(dest,a2);
    }

    if (strcmp(a2, "zero")==0){return 0;}

    if(strcmp(b1,"ld") == 0 || strcmp(b1,"lb") == 0 || strcmp(b1,"lh") == 0 || strcmp(b1,"lw") == 0 || strcmp(b1,"lbu") == 0 ||strcmp(b1,"lhu") == 0 || strcmp(b1,"lwu") == 0){
        b3 = get_reg(b3);
        change(b3);
        strcpy(source1,b3);
    }
    else if(strcmp(b1,"sd") == 0 || strcmp(b1,"sw") == 0 || strcmp(b1,"sb") == 0 || strcmp(b1,"sh") == 0 ){
        int l=strlen(b2);
        b2[l-1]='\0';
        change(b2);
        b3=get_reg(b3);
        change(b3);
        strcpy(source2,b3);
        strcpy(source1,b2);
    }
    else{
        int l=strlen(b3);
        b3[l-1]='\0';
        change(b3);
        strcpy(source1,b3);
//        if(b3[0]!='x'){
        change(b4);

        //strcat(source," ");
        strcpy(source2,b4);
    }
   // printf("assa %s %s %s assa\n",dest,source1,source2);
    if((strcmp(dest,source1)==0 || strcmp(dest,source2)==0)){
        if(strcmp(a1,"sd")!=0){
            return 2;
        }
        else{
            return 1;
        }
    }
    else{
        return 0;
    }
}

int main() {
    //printf("enter number of instructions:");
    //int n;
    r1=2;
    FILE *file=fopen("test1(1).txt","r");
    char buffer[max];
    printf("Without forwarding\n");
    //getchar();
    int al,bl;
    fgets(buffer,sizeof(buffer),file);
   // scanf("%d",&n);
    char a[max],b[max],c[max];
    al=strlen(buffer);
    buffer[al-1]='\0';
    strcpy(a,buffer);
    //getchar();
    fgets(buffer,sizeof(buffer),file);
    al=strlen(buffer);
    buffer[al-1]='\0';
  //  getchar();
//    fgets(b,sizeof(b),stdin);
    strcpy(b,buffer);
    char adup[max],bdup[max],cdup[max];
    strcpy(adup,a);
    strcpy(bdup,b);
    r=process(a,b);
    printf("%s\n",adup);
    if(r==2){
        printf("NOP\n");
        printf("NOP\n");
        r1+=2;
    }
    else if(r==1){
        printf("NOP\n");
        r1+=1;
    }
    printf("%s\n",bdup);
    while(fgets(buffer,sizeof(buffer),file)!=NULL){
        al=strlen(buffer);
    buffer[al-1]='\0';
        r1+=1;
        if(strlen(buffer)==0){
            exit(0);
        }
        //fgets(buffer,sizeof(buffer),file);
        strcpy(c,buffer);
        strcpy(cdup,c);
        strcpy(a,adup);
        strcpy(b,bdup);
        if(r!=0){
            r=process(b,c);
            //printf("%s\n",cdup);
            if(r==2){
                printf("NOP\n");
                printf("NOP\n");
                r1+=2;
            }
            else if(r==1){
                printf("NOP\n");
                r1+=1;
            }
            printf("%s\n",cdup);
        }
        else{
            r=process(b,c);
            //printf("%d ",r);
            if(r!=0){
               // printf("%s\n",cdup);
                if(r==2){
                printf("NOP\n");
                printf("NOP\n");
                r1+=2;
            }
            else if(r==1){
                printf("NOP\n");
                r1+=1;
            }
            printf("%s\n",cdup);
            }
            else{
               // printf("%s",cdup);
                strcpy(c,cdup);
                strcpy(a,adup);
                strcpy(b,bdup);
                r=process(a,c);
                if(r==2){
                    printf("NOP\n");
                    r1+=1;
                    r=1;
                }
                else{
                    r=0;
                }
                printf("%s\n",cdup);
            }
        }
        strcpy(adup,bdup);
        strcpy(bdup,cdup);
    }
    fclose(file);
    printf("total cycles: %d",r1+4);
    printf("\n\nWith forwarding\n\n");
    FILE *file2=fopen("input(1).txt","r");
    if(file2==NULL){
        printf("ok");
        exit(1);
    }
    fgets(buffer,sizeof(buffer),file2);
    strcpy(a,buffer);
    strcpy(adup,buffer);
    fgets(buffer,sizeof(buffer),file2);
    strcpy(b,buffer);
    strcpy(bdup,buffer);
    al=strlen(a);
    a[al-1]='\0';
    bl=strlen(b);
    b[bl-1]='\0';
    r1=2;
    int r=processf(a,b);
    printf("%s",adup);
    if(r==1){
        printf("NOP");
        r1+=1;
    }
    strcpy(a,bdup);
    printf("%s",bdup);
    while(fgets(buffer,sizeof(buffer),file2)!=NULL){
        strcpy(b,buffer);
        bl=strlen(b);
        b[bl-1]='\0';
        r1+=1;
        strcpy(bdup,b);
        r=processf(a,b);
        if(r==1){
            printf("NOP\n");
            r1+=1;
        }
        printf("%s\n",bdup);
        strcpy(a,bdup);
    }
    printf("Total cycples= %d",r1+4);

    return 0;
}

