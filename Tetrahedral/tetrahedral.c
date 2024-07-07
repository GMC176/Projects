#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <math.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#define max 2000000
#define maxu 100
struct node
{
    int data[max];
    int tet;
};

struct node *createnode()
{
    struct node *temp = malloc(sizeof(struct node));
    return temp;
}

void process(struct node *in, int nn)
{
    //  printf("aaaaaaaaaaaaa\n");
    /*int intetra=0;
    int nintetra=0;
    int tetra[max];
    int ntetra[max];*/
    int *ptr;
    int SIZE = 100000;
    int list[max];
    int inlist = 0;
    char name[20];
    char filename[20];
    sprintf(filename, "OutFile%d", nn);
    FILE *file = fopen(filename, "w");
    sprintf(name, "shareno%d", nn);
    int fd;
    int f = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(f, SIZE);
    ptr = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    int t = in->tet;
    int con = 0;
    // printf("%d ss",t);
    for (int i = 0; i < t; i++)
    {
        if (nn == 0)
        {
            // printf("%d ss\n",in->data[i]);
        }
        con = 0;
        int num = in->data[i];
        num = num * 6;
        //  printf("%d gg",num);
        int uplimit = cbrt(num);
        for (int j = 0; j < uplimit + 4; j++)
        {
            if (j * (j + 1) * (j + 2) == num)
            {
                list[inlist] = num / 6;
                //             printf("%d aaaaaaaaaaaaaahshdah",num/6);
                inlist++;
                fprintf(file, "%d: is a tetrahedral number\n", num / 6);
                con = 1;
                break;
            }
        }
        if (con == 0)
        {
            fprintf(file, "%d: not a tetrahedral number\n", num / 6);
        }
    }
    int dist = 0;
    //   printf("%d ss\n",inlist);
    for (int i = 0; i < inlist; i++)
    {
        dist = dist + sprintf((char *)ptr + dist, "%d ", list[i]);
    }
    fclose(file);
}

int main()
{
    // int m=10;
    // int n=970;
    FILE *filein;
    // char buffer[maxu];
    filein = fopen("input1", "r");
    // fgets(buffer,sizeof(buffer),filein);
    // int ll=strlen(buffer);
    // buffer[ll-1]='\0';
    // ll=strlen(buffer);
    int n, m;
    // if (fscanf(filein, "%d %d", &n, &m) != 2)
    // {
    //     printf("Unable to read two integers from the file.\n");
    //     fclose(filein);
    //     return 1;
    // }
    // char *a1=strtok(buffer," ");
    // char *a2=strtok(NULL," ");
    // //printf("%s %s",a1,a2);
    // int n=atoi(a1);
    // int m=atoi(a2);
    fscanf(filein, "%d %d", &n, &m);
    // printf("%d %d \n",n,m);
    // int n=100000000;
    // int m=50;
    int size = 1000000;
    //  printf("%d",m);
    // char *name="share";
    // int fd=shm_open(name,O_RDWR,0666);
    // char *ptr=(char *)mmap(0,size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    int count = 0;
    fclose(filein);
    struct node *allnode[maxu];
    double cpu_time_used;
    struct timespec s_time, e_time;
    clock_gettime(CLOCK_MONOTONIC, &s_time);
    for (int i = 0; i < m; i++)
    {

        allnode[i] = createnode();
        allnode[i]->tet = 0;
    }
    while (n != -1)
    {
        for (int i = 0; i < m; i++)
        {
            allnode[i]->data[count] = n;
            n--;
            allnode[i]->tet = allnode[i]->tet + 1;
            if (n == -1)
            {
                break;
            }
        }
        if (n == -1)
        {
            break;
        }
        count++;
    }

    int cfork = 0;
    char share[maxu];
    for (int i = 0; i < m; i++)
    {
        // printf("ss");
        pid_t pid = fork();
        sprintf(share, "shareno%d", i);
        int fd = shm_open(share, O_RDWR, 0666);
        ftruncate(fd, sizeof(max));
        if (pid == 0)
        {
            process(allnode[i], i);
            exit(0);
        }
        else if (pid < 0)
        {
            printf("error in fork");
        }
    }
    int SIZE = 1000000;
    char mainfile[20];
    for (int i = 0; i < m; i++)
    {
        wait(NULL);
    }
    FILE *file = fopen("OutMain", "w");
    for (int i = 0; i < m; i++)
    {
        sprintf(share, "shareno%d", i);
        int fd = shm_open(share, O_RDWR, 0666);
        int *ptr = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        fprintf(file, "P%d:", i);
        if (ptr != MAP_FAILED)
        {
            fprintf(file, "%s\n", (char *)ptr);
        }
        shm_unlink(share);
    }

    clock_gettime(CLOCK_MONOTONIC, &e_time);
    double elapsed_time = (e_time.tv_sec - s_time.tv_sec) +
                          (e_time.tv_nsec - s_time.tv_nsec) / 1e9;

    printf("total time:%fs", elapsed_time);
    fprintf(file, "total time:%fs", elapsed_time);
    fclose(file);

    //   printf("%s",ptr);
}
