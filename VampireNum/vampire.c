#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define max 1000000
#define maxu 1000
int countvamp;
struct node
{
    int data[max];
    int total;
};

struct node *createnode()
{
    struct node *temp = malloc(sizeof(struct node));
    return temp;
}

int checkvamp(int num)
{
    int numd = 0;
    int s = num;
    int array[max];
    int iarray = 0;
    while (num != 0)
    {
        int a = num % 10;
        array[iarray] = a;
        iarray++;
        num = num / 10;
    }
    num = s;
    if (iarray % 2 != 0)
    {
        return 0;
    }
    if (iarray == 2)
    {
        for (int i = 0; i < iarray; i++)
        {
            for (int j = 0; j < iarray; j++)
            {
                if (i != j)
                {
                    if (array[i] * array[j] == num)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else if (iarray == 4)
    {
        int num1 = 0;
        int num2 = 0;
        for (int i = 0; i < iarray; i++)
        {
            for (int j = 0; j < iarray; j++)
            {
                if (i != j)
                {
                    num1 = array[i] * 10 + array[j];

                    for (int k = 0; k < iarray; k++)
                    {
                        if (k != i && k != j)
                        {
                            for (int l = 0; l < iarray; l++)
                            {
                                if (l != i && l != j && l != k)
                                {
                                    num2 = array[k] * 10 + array[l];
                                    if (num1 * num2 == num && (num1%10!=0 || num2%10!=0))
                                    {
                                        //   printf("%d %d-num %d aa\n ",num1,num2,num);
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (iarray == 6)
    {
        int num1 = 0;
        int num2 = 0;
        for (int i = 0; i < iarray; i++)
        {
            for (int j = 0; j < iarray; j++)
            {
                for (int k = 0; k < iarray; k++)
                {
                    if (i != j && i != k && j != k)
                    {
                        num1 = array[i] * 100 + array[j] * 10 + array[k];
                        int not [3];
                        not [0] = i;
                        not [1] = j;
                        not [2] = k;
                        for (int l = 0; l < iarray; l++)
                        {
                            if (l != not [0] && l != not [1] && l != not [2])
                            {
                                for (int m = 0; m < iarray; m++)
                                {
                                    if (m != not [0] && m != not [1] && m != not [2] && m != l)
                                    {
                                        for (int n = 0; n < iarray; n++)
                                        {
                                            if (n != not [0] && n != not [1] && n != not [2] && n != l && n != m && l != m)
                                            {
                                                num2 = array[l] * 100 + array[m] * 10 + array[n];
                                                if (num1 * num2 == num && (num1%10!=0 || num2%10!=0))
                                                {
                                                    return 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (iarray == 8)
    {
        for (int i = 0; i < iarray; i++)
        {
            for (int j = 0; j < iarray; j++)
            {
                for (int k = 0; k < iarray; k++)
                {
                    for (int l = 0; l < iarray; l++)
                    {
                        if (i != j && i != k && i != l && j != k && j != l && k != l)
                        {
                            int num1 = array[i] * 1000 + array[j] * 100 + array[k] * 10 + array[l];
                            int array[4];
                            array[0] = i;
                            array[1] = j;
                            array[2] = k;
                            array[3] = l;
                            int anum2[4];
                            int innum2 = 0;
                            for (int m = 0; m < iarray; m++)
                            {
                                if (array[m] != i && array[m] != j && array[m] != k && array[m] != l)
                                {
                                    anum2[innum2] = m;
                                    innum2++;
                                }
                            }
                            for (int m = 0; m < 4; m++)
                            {
                                for (int n = 0; n < 4; n++)
                                {
                                    for (int o = 0; o < 4; o++)
                                    {
                                        for (int p = 0; p < 4; p++)
                                        {
                                            if (m != n && m != o && m != p && n != o && n != p && o != p)
                                            {
                                                int num2 = anum2[m] * 1000 + anum2[n] * 100 + anum2[o] * 10 + anum2[p];
                                                if (num1 * num2 == num && (num1%10!=0 || num2%10!=0))
                                                {
                                                    return 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (iarray == 10)
    {
        int num1;
        int num2;
        for (int i = 0; i < iarray; i++)
        {
            for (int j = 0; j < iarray; j++)
            {
                for (int k = 0; k < iarray; k++)
                {
                    for (int l = 0; l < iarray; l++)
                    {
                        for (int m = 0; m < iarray; m++)
                        {
                            if (i != j && i != k && i != l && i != m && j != k && j != l && j != m && k != l && k != m && l != m)
                            {
                                num1 = array[i] * 10000 + array[j] * 1000 + array[k] * 100 + array[l] * 10 + array[m];
                                int innum2[5];
                                int innum = 0;
                                for (int n = 0; n < iarray; n++)
                                {
                                    if (array[n] != i && array[n] != j && array[n] != k && array[n] != l && array[n] != m )
                                    {
                                        innum2[innum] = array[n];
                                        innum++;
                                    }
                                }
                                for (int n = 0; n < 5; n++)
                                {
                                    for (int o = 0; o < 5; o++)
                                    {
                                        for (int p = 0; p < 5; p++)
                                        {
                                            for (int q = 0; q < 5; q++)
                                            {
                                                for (int r = 0; r < 5; r++)
                                                {
                                                    if (n != o && n != p && n != q && n != r && o != p && o != q && o != r && p != q && p != r && q != r)
                                                    {
                                                        num2 = innum2[n] * 10000 + innum2[o] * 1000 + innum2[p] * 100 + innum2[q] * 10 + innum2[r];
                                                        if (num1 * num2 == num &&(num1%10!=0 || num2%10!=0))
                                                        {
                                                            return 1;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void *check(void *arg)
{
    struct node *temp = (struct node *)(arg);
    int t = temp->total;
    int inv = 0;
    int vamp[maxu];
    for (int i = 0; i < t; i++)
    {
        int r = checkvamp(temp->data[i]);
        if (r == 1)
        {
            vamp[inv] = temp->data[i];
            countvamp++;
            inv++;
        }
    }
    temp->total = inv;
    pthread_exit(vamp);
}
int main()
{
    countvamp=0;
    FILE *filein;
    char buffer[maxu];
    filein = fopen("input1", "r");
    if (filein == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    int n, m;
    if (fscanf(filein, "%d %d", &n, &m) != 2)
    {
        printf("Unable to read two integers from the file.\n");
        fclose(filein);
        return 1;
    }
    fgets(buffer, sizeof(buffer), filein);
    int ll = strlen(buffer);
    buffer[ll - 1] = '\0';
    ll = strlen(buffer);
   // char *a1 = strtok(buffer, " ");
    //char *a2 = strtok(NULL, " ");
    // printf("%s %s",a1,a2);
    //  int n=atoi(a1);
    //  int m=atoi(a2);

    fclose(filein);

   // printf("%d %d ss", m, n);
    //  exit(0);
    pthread_t threads[maxu];
    struct node *d[max];
    int result[max];
    //   clock_t s_time,e_time;
    double cpu_time_used;
    struct timespec s_time, e_time;
    //  struct node result[max];
    // int m=10;
    // int n=1000000;
    int count = 0;
    int give = n;
    for (int i = 0; i < m; i++)
    {
        d[i] = createnode();
        //     result[i]=*createnode();
    }
    clock_gettime(CLOCK_MONOTONIC, &s_time);

    while (n != -1)
    {
        for (int i = 0; i < m; i++)
        {
            d[i]->data[count] = n;
            n--;
            d[i]->total = count + 1;
            if (n == -1)
            {
                break;
            }
        }
        count++;
    }

    for (int i = 0; i < m; i++)
    {
        pthread_create(&threads[i], NULL, check, d[i]);
    }

    FILE *file;
    file = fopen("OutMain.txt", "w");
    for (int i = 0; i < m; i++)
    {
        int *result;
        pthread_join(threads[i], (void **)&result);
        for (int j = 0; j < d[i]->total; j++)
        {
            fprintf(file, "in thread %d-number %d is vampire number\n", i, result[j]);
        }
    }
    fprintf(file,"total number of vampire numbers is %d",countvamp);
    clock_gettime(CLOCK_MONOTONIC, &e_time);
    double elapsed_time = (e_time.tv_sec - s_time.tv_sec) +
                          (e_time.tv_nsec - s_time.tv_nsec) / 1e9;
    fclose(file);
    printf("total time:%fs", elapsed_time);
}
