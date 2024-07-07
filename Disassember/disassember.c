// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 1000
int pc;
int i;
char binary[32];
char f[3];
int array[max];
int count=0;
int conditionn=0;

void machinecode(char a){
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
    
    
}

int send(int offset){
offset=pc+(offset/2);
for(int j=0;j<count;j++){
if(offset==array[j]){
return j;
}}
array[count]=offset;
count++;
/*for(int ll=0;ll<count;ll++){
printf("%d ",array[ll]);
}*/
return (count-1);
}

int ispresent(){
for(int j=0;j<count;j++){
if(pc==array[j]){
return j;
}}
return -1;
}


void rformat(){
            char reg[5];
            reg[0]=binary[7];
            reg[1]=binary[8];
            reg[2]=binary[9];
            reg[3]=binary[10];
            reg[4]=binary[11];
            int rs2=strtol(reg,NULL,2);
            reg[0]=binary[12];
            reg[1]=binary[13];
            reg[2]=binary[14];
            reg[3]=binary[15];
            reg[4]=binary[16];
            int rs1=strtol(reg,NULL,2);
            reg[0]=binary[20];
            reg[1]=binary[21];
            reg[2]=binary[22];
            reg[3]=binary[23];
            reg[4]=binary[24];
            int rd=strtol(reg,NULL,2);
           if(conditionn==1){
    if(binary[1]=='1'){
        if(binary[17]=='0'){
            printf("sub x%d,x%d,x%d",rd,rs1,rs2);
        }
        else{
            printf("sra x%d,x%d,x%d",rd,rs1,rs2);
        }
    }
    else{
        char a=binary[17];
        char b=binary[18];
        char c=binary[19];
        int fun3;
        if(a=='0'){
            if(b=='0'){
                if(c=='0'){
                    fun3=0;
                }
                else{
                    fun3=1;
                }
            }
            else{
                if(c=='0'){
                    fun3=2;
                }
                else{
                    fun3=3;
                }
            }
        }
        else{
            if(b=='0'){
                if(c=='0'){
                    fun3=4;
                }
                else{
                    fun3=5;
                }
            }
            else{
                if(c=='0'){
                    fun3=6;
                }
                else{
                    fun3=7;
                }
            }
        }
    //    printf("%s",f);
      //  printf("%d %c %c %c",fun3,binary[17],binary[18],binary[19]);
     // printf("%d :",pc);
        if(fun3==0){
            printf("add x%d,x%d,x%d",rd,rs1,rs2);
        }
        else if(fun3==4){
            printf("xor x%d,x%d,x%d",rd,rs1,rs2);
        }
        else if(fun3==6){
            printf("or x%d,x%d,x%d",rd,rs1,rs2);
        }
        else if(fun3==7){
            printf("and x%d,x%d,x%d",rd,rs1,rs2);
        }
        else if(fun3==1){
            printf("sll x%d,x%d,x%d",rd,rs1,rs2);
        }
        else if(fun3==5){
            printf("srl x%d,x%d,x%d",rd,rs1,rs2);
        }
        else if(fun3==2){
            printf("slt x%d,x%d,x%d",rd,rs1,rs2);
        }
        else if(fun3==3){
            printf("sltu x%d,x%d,x%d",rd,rs1,rs2);
        }
    }}
}

void sformat(){
    char reg[5];
        reg[0]=binary[7];
        reg[1]=binary[8];
        reg[2]=binary[9];
        reg[3]=binary[10];
        reg[4]=binary[11];
        int rs1=strtol(reg,NULL,2);
        reg[0]=binary[12];
        reg[1]=binary[13];
        reg[2]=binary[14];
        reg[3]=binary[15];
        reg[4]=binary[16];
        int rs2=strtol(reg,NULL,2);
    char imm[13];
    for(int k=0;k<7;k++){
        imm[k]=binary[k];
    }
    imm[7]=binary[20];
    imm[8]=binary[21];
    imm[9]=binary[22];
    imm[10]=binary[23];
    imm[11]=binary[24];
    imm[12]='\n';
    int offset=strtol(imm,NULL,2);
    if(binary[0]=='1'){
    offset=offset-4096;
    }
    char fun3[3];
    fun3[0]=binary[17];
    fun3[1]=binary[18];
    fun3[2]=binary[19];
    if(conditionn==1){
    if(fun3[0]=='0'){
        if(fun3[1]=='0'){
            if(fun3[2]=='0'){
                printf("sb x%d,%d(x%d)",rs1,offset,rs2);
            }
            else{
                printf("sh x%d,%d(x%d)",rs1,offset,rs2);
            }
        }
        else{
            if(fun3[2]=='0'){
                printf("sw x%d,%d(x%d)",rs1,offset,rs2);
            }
            else{
                printf("sd x%d,%d(x%d)",rs1,offset,rs2);
            }
        }
    }
}}

void iformat(){
    char imm[13];
    for(int k=0;k<12;k++){
        imm[k]=binary[k];
    }
    imm[12]='\n';
    int offset=strtol(imm,NULL,2);
    if(imm[0]=='1'){
    offset=-4096+offset;
    }
    char reg1[6];
    reg1[0]=binary[12];
    reg1[1]=binary[13];
    reg1[2]=binary[14];
    reg1[3]=binary[15];
    reg1[4]=binary[16];
    reg1[5]='\n';
    int rs1=strtol(reg1,NULL,2);
    char reg2[6];
    reg2[0]=binary[20];
    reg2[1]=binary[21];
    reg2[2]=binary[22];
    reg2[3]=binary[23];
    reg2[4]=binary[24];
    reg2[5]='\n';
    char f3[4];
    f3[0]=binary[17];
    f3[1]=binary[18];
    f3[2]=binary[19];
    f3[3]='\n';
    int fun3=strtol(f3,NULL,2);
    int rs2=strtol(reg2,NULL,2);
    if(conditionn==1){
    if(binary[25]=='0'){
    if(binary[27]=='1'){
        if(fun3==0){
            printf("addi x%d,x%d,%d",rs2,rs1,offset);
        }
        else if(fun3==1){
            printf("slli x%d,x%d,%d",rs2,rs1,offset);
        }
        else if(fun3==2){
            printf("slti x%d,x%d,%d",rs2,rs1,offset);
        }
        else if(fun3==3){
            printf("sltiu x%d,x%d,%d",rs2,rs1,offset);
        }
        else if(fun3==4){
            printf("xori x%d,x%d,%d",rs2,rs1,offset);
        }
        else if(fun3==5){
            if(binary[1]=='1'){
            printf("srai x%d,x%d,%d",rs2,rs1,offset-1024);
            }
            else{
            printf("srli x%d,x%d,%d",rs2,rs1,offset);
            }
        }
        else if(fun3==6){
            printf("ori x%d,x%d,%d",rs2,rs1,offset);
        }
        else if(fun3==7){
            printf("andi x%d,x%d,%d",rs2,rs1,offset);
        }
    }
    else{
        if(fun3==0){
            printf("lb x%d,%d(x%d)",rs2,offset,rs1);
        }
        else if(fun3==1){
            printf("lh x%d,%d(x%d)",rs2,offset,rs1);
        }
        else if(fun3==2){
            printf("lw x%d,%d(x%d)",rs2,offset,rs1);
        }
        else if(fun3==3){
            printf("ld x%d,%d(x%d)",rs2,offset,rs1);
        }
        else if(fun3==4){
            printf("lbu x%d,%d(x%d)",rs2,offset,rs1);
        }
        else if(fun3==5){
            printf("lhu x%d,%d(x%d)",rs2,offset,rs1);
        }
        else if(fun3==6){
            printf("lwu x%d,%d(x%d)",rs2,offset,rs1);
        }
    }}
    else{
        if(binary[27]=='0'){
            printf("jalr x%d,%d(x%d)",rs2,offset,rs1);
        }
    }
}
}
void bformat(){
    char imm[13];
    imm[0]=binary[0];
    for(int j=1;j<7;j++){
        imm[j+1]=binary[j];
    }
    imm[8]=binary[20];
    imm[9]=binary[21];
    imm[10]=binary[22];
    imm[11]=binary[23];
    imm[1]=binary[24];
    imm[12]='\n';
    char reg1[6];
    reg1[0]=binary[12];
    reg1[1]=binary[13];
    reg1[2]=binary[14];
    reg1[3]=binary[15];
    reg1[4]=binary[16];
    reg1[5]='\n';
    int rs1=strtol(reg1,NULL,2);
    char reg2[6];
    reg2[0]=binary[7];
    reg2[1]=binary[8];
    reg2[2]=binary[9];
    reg2[3]=binary[10];
    reg2[4]=binary[11];
    reg2[5]='\n';
    char f3[4];
    f3[0]=binary[17];
    f3[1]=binary[18];
    f3[2]=binary[19];
    f3[3]='\n';
    int fun3=strtol(f3,NULL,2);
    int rs2=strtol(reg2,NULL,2);
    int offset=strtol(imm,NULL,2);
    if(binary[0]=='1'){
    offset=offset-4096;
    }
    int condition=send(offset);
    if(conditionn==1){
    if(fun3==0){
        printf("beq x%d,x%d,L%d",rs1,rs2,condition);
    }
    else if(fun3==1){
        printf("bne x%d,x%d,L%d",rs1,rs2,condition);
    }
    else if(fun3==4){
        printf("blt x%d,x%d,L%d",rs1,rs2,condition);
    }
    else if(fun3==5){
        printf("bge x%d,x%d,L%d",rs1,rs2,condition);
    }
    else if(fun3==6){
        printf("bltu x%d,x%d,L%d",rs1,rs2,condition);
    }
    else if(fun3==7){
        printf("bgeu x%d,x%d,L%d",rs1,rs2,condition);
    }
}}

void uformat(){
    char imm[21];
    for(int j=0;j<20;j++){
        imm[j]=binary[j];
    }
    imm[20]='\n';
    int offset=strtol(imm,NULL,2);
    char reg[6];
    reg[0]=binary[20];
    reg[1]=binary[21];
    reg[2]=binary[22];
    reg[3]=binary[23];
    reg[4]=binary[24];
    reg[5]='\n';
    int rd=strtol(reg,NULL,2);
    if(conditionn==1){
    if(binary[26]=='0'){
        printf("auipc x%d,%d",rd,offset);
    }
    else{
        printf("lui x%d,%d",rd,offset);
    }
}}

void jformat(){
    char imm[21];
    imm[0]=binary[0];
    for(int j=1;j<=10;j++){
        imm[j+9]=binary[j];
    }
    imm[9]=binary[11];
    for(int k=1;k<9;k++){
        imm[k]=binary[11+k];
    }
    imm[20]='\n';
    int offset=strtol(imm,NULL,2);
    if(binary[0]=='1'){
    offset=offset-1048576;
    }
    int condition=send(offset);
    char reg[6];
    reg[0]=binary[20];
    reg[1]=binary[21];
    reg[2]=binary[22];
    reg[3]=binary[23];
    reg[4]=binary[24];
    reg[5]='\n';
    int rd=strtol(reg,NULL,2);
    if(conditionn==1){
    printf("jal x%d,L%d",rd,condition);
}}

int main() {
  //  int array[8]={0xf,0xe,9,5,0,0xf,0xe,3};
    pc=0;
    FILE *file;
    char line[10];
   /* if (file == NULL) {
        perror("Error opening file");
        return -1;
    }*/
    for(conditionn=0;conditionn<2;conditionn++){
    file = fopen("test3.txt", "r");
    pc=0;
    while (fgets(line, sizeof(line), file) != NULL) {
    if(conditionn==1){
        printf("\n");
        }
    	pc++;
   int lcc=ispresent();
   if(conditionn==1){
if(lcc!=-1){
   printf("L%d: ",lcc);
   }  }
   // printf("%s\n",line);
        int digitCount = 0;
        for (i = 0;i<8; i++) {
            machinecode(line[i]);
           // printf("%c\n",line[i]);
        }

  //  fclose(file); 
  //  printf("%s \n",binary);
   /* for(int j=0;j<32;j++){
        printf("%c",binary[j]);
    }*/
    /*if(binary[25]=='0' && binary[26]=='1' && binary[27]=='1' && binary[28]=='0' && binary[29]=='0' && binary[30]=='1' && binary[31]=='1'){
        iformat();
    }*/
    if(binary[25]=='0' && binary[26]=='1' && binary[27]=='1' && binary[28]=='0' && binary[29]=='0' && binary[30]=='1' && binary[31]=='1'){
        rformat();
    }
    else if(binary[25]=='0' && binary[26]=='1' && binary[27]=='0' && binary[28]=='0' && binary[29]=='0' && binary[30]=='1' && binary[31]=='1'){
        sformat();
    }
    else if(binary[25]=='0' && binary[26]=='0' && binary[28]=='0' && binary[29]=='0' && binary[30]=='1' && binary[31]=='1'){
        iformat();
    }
    else if(binary[25]=='1' && binary[26]=='1' && binary[27]=='0' && binary[28]=='0' && binary[29]=='0' && binary[30]=='1' && binary[31]=='1'){
        bformat();
    }
    else if(binary[25]=='0' && binary[27]=='1' && binary[28]=='0' && binary[29]=='1' && binary[30]=='1' && binary[31]=='1'){
        uformat();
    }
    else if(binary[25]=='1' && binary[26]=='1' && binary[27]=='0' && binary[28]=='1' && binary[29]=='1' && binary[30]=='1' && binary[31]=='1'){
        jformat();
    }
    if(binary[25]=='1' && binary[26]=='1' && binary[27]=='0' && binary[28]=='0' && binary[29]=='1' && binary[30]=='1' && binary[31]=='1'){
        iformat();
}  }
fclose(file);
}
/*for(int kk=0;kk<count;kk++){
printf("%d ",array[kk]);
}*/
    return 0;
}
