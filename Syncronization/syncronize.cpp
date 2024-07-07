#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <chrono>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <atomic>
using namespace std;
int n;
int k, p, num;
int result[10000][10000];
int resultm[10000][10000];
int resultbcas[10000][10000], resultatom[10000][10000];
int given[10000][10000];

int step;
int counter, key;
bool waiting[1000] = {false};
atomic<int> lockcas(0);
atomic<int> lockbcas(0);
atomic_flag test = ATOMIC_FLAG_INIT;
atomic_int counteratom;
struct node
{
    int start;
    int numrows;
    int k;
    int inrow;
};

struct node *createnode()
{
    struct node *temp = new struct node;
    return temp;
}

void *multiplychunktas(void *args)
{
    struct node *temp = (struct node *)(args);
    int itsnum = temp->k;
    int itss;
    int itssend;
    /* int numrows=temp->numrows;
     int num=temp->inrow;
     int s=temp->start;
     int e=numrows+s-1;
     int k=temp->k;*/
    int sum, c, start, con = 0;
    while (counter < num)
    {
        while (test.test_and_set())
        {
            1;
        }
        if (counter >= num)
        {
            break;
        }

        /* do nothing */
        /* critical section */
        // cout << k ;
        start = counter;
        counter = counter + step;
        itssend = counter;
        test.clear();
      //  cout << "a";
        for (int i = start; i < itssend; i++)
        {
            for (int j = 0; j < num; j++)
            {
                c = 0;
                sum = 0;
                while (c != num)
                {
                    sum = sum + given[i][c] * given[c][j];
                    c++;
                }
                result[i][j] = sum;
            }
        }
    }

    return nullptr;
}

void *multiplychunkatomic(void *args)
{
    struct node *temp = (struct node *)(args);
    int itsnum = temp->k;
    int itss;
    int itssend;
    /* int numrows=temp->numrows;
     int num=temp->inrow;
     int s=temp->start;
     int e=numrows+s-1;
     int k=temp->k;*/
    int sum, c, start, con = 0;
    while (counteratom < num)
    {
        /*while (test.test_and_set()){
         1;
        }
        if(counter>=num){
         break;
        }*/

        /* do nothing */
        /* critical section */
        // cout << k ;
        start = counteratom.fetch_add(step);
        itssend = start + step;
        // cout << itsnum << " " << "a " << start<<endl;

        for (int i = start; i < itssend; i++)
        {
            for (int j = 0; j < num; j++)
            {
                c = 0;
                sum = 0;
                while (c != num)
                {
                    sum = sum + given[i][c] * given[c][j];
                    c++;
                }
                resultatom[i][j] = sum;
            }
        }
    }

    return nullptr;
}

void *multiplychunkcas(void *args)
{
    // cout << "ss" << endl;
    struct node *temp = (struct node *)(args);
    int itsnum = temp->k;
    int itss;
    int itssend;
    /* int numrows=temp->numrows;
     int num=temp->inrow;
     int s=temp->start;
     int e=numrows+s-1;
     int k=temp->k;*/
    int sum, c, start, con = 0;
    bool cas;

    while (counter < num)
    {
        int ppp = 0;
        while (!lockcas.compare_exchange_weak(ppp, 1))
        {
            1;
        }
        if (counter >= num)
        {
            break;
        }

        /* do nothing */
        /* critical section */
        // cout << k ;
        start = counter;
        counter = counter + step;
        itssend = counter;
        lockcas = 0;

        // cout << start << " a" << itsnum <<"a" <<endl;

        for (int i = start; i < itssend; i++)
        {
            for (int j = 0; j < num; j++)
            {
                c = 0;
                sum = 0;
                while (c != num)
                {
                    sum = sum + given[i][c] * given[c][j];
                    c++;
                }
                resultm[i][j] = sum;
            }
        }
    }

    return nullptr;
}

void *multiplychunkbcas(void *args)
{
    struct node *temp = (struct node *)(args);
    int itsnum = temp->k;
    int sum, got = 0, next, c, start;

    while (counter < num)
    {
        if (got == 0)
        {
            waiting[itsnum] = true;
        } 
        int expected = 0;
        key = 0;

        while (waiting[itsnum] && key == 0)
        {
            key = lockbcas.compare_exchange_weak(expected, 1);
        }
        got = 0;
        waiting[itsnum] = false;

        if (counter >= num)
        {
            break;
        }

        start = counter;
        counter += step;
        int itssend = counter;
        next = -1;
        for (int i = itsnum + 1; i < k; i++)
        {
            if (waiting[i])
            {
                next = i;
                break;
            }
        }

        if (next == -1)
        {
            for (int i = 0; i < itsnum; i++)
            {
                if (waiting[i])
                {
                    next = i;
                    break;
                }
            }
        }

        for (int i = start; i < itssend; i++)
        {
            for (int j = 0; j < num; j++)
            {
                sum = 0;
                for (c = 0; c < num; c++)
                {
                    sum += given[i][c] * given[c][j];
                }
                resultbcas[i][j] = sum;
            }
        }
        // cout << start << " a" << itsnum <<"a" <<endl;
        if (next != -1)
        {
            lockbcas = 0;
            waiting[next] = false;
        }
        else
        {
            got = 1;
        }
    }

    return nullptr;
}

void *chunktas(void *args)
{
    pthread_t threads[n];
    struct node *temp[k];
    // int rem=n%k;
    int t = n / k;
    /// 111
    /// 222
    // int tt=rem*(n/k+1)-rem*(n/k);
    auto start_time = chrono::high_resolution_clock::now();
    cout << "TAS:\n";
    /* for(int i=0;i<k;i++){
     temp[i]=createnode();
     temp[i]->start=i*(t);
     temp[i]->numrows=n/k;
     temp[i]->k=k;
     temp[i]->inrow=p/n;
     pthread_create(&threads[i],NULL,multiplychunk,temp[i]);

     }*/
    int rem = n % k;
    for (int i = 0; i < k; i++)
    {
        /*  if(i<rem){
          temp[i]=createnode();
          temp[i]->start=i*((n/k)+1);
          temp[i]->numrows=n/k+1;
          temp[i]->k=k;
          temp[i]->inrow=p/n;
          pthread_create(&threads[i],NULL,multiplychunk,temp[i]);
          }
          else{
              temp[i]=createnode();
          temp[i]->start=rem*(n/k+1)+(i-rem)*(n/k);
          temp[i]->numrows=n/k;
          temp[i]->k=k;
          temp[i]->inrow=p/n;*/
        temp[i] = createnode();
        temp[i]->k = i;
        pthread_create(&threads[i], NULL, multiplychunktas, temp[i]);
    }
    // auto start_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        pthread_join(threads[i], NULL);
    }
    auto end_time = chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationc = end_time - start_time;
    ofstream output("chunka3tas.output");
    // auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    output << "Time elapsed for tas section: " << durationc.count() << " seconds" << endl;
    cout << "Time elapsed for tas section: " << durationc.count() << " seconds" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p / n; j++)
        {
            output << result[i][j] << " ";
        }
        output << "\n";
    }
    output.close();
    return nullptr;
}

void *chunkcas(void *args)
{
    pthread_t threadscas[n];
    struct node *temp[k];
    // int rem=n%k;
    int t = n / k;
    /// 111
    /// 222
    // int tt=rem*(n/k+1)-rem*(n/k);
    auto start_time = chrono::high_resolution_clock::now();
    cout << "CAS:\n";
    /* for(int i=0;i<k;i++){
     temp[i]=createnode();
     temp[i]->start=i*(t);
     temp[i]->numrows=n/k;
     temp[i]->k=k;
     temp[i]->inrow=p/n;
     pthread_create(&threads[i],NULL,multiplychunk,temp[i]);

     }*/
    int rem = n % k;
    for (int i = 0; i < k; i++)
    {
        /*  if(i<rem){
          temp[i]=createnode();
          temp[i]->start=i*((n/k)+1);
          temp[i]->numrows=n/k+1;
          temp[i]->k=k;
          temp[i]->inrow=p/n;
          pthread_create(&threads[i],NULL,multiplychunk,temp[i]);
          }
          else{
              temp[i]=createnode();
          temp[i]->start=rem*(n/k+1)+(i-rem)*(n/k);
          temp[i]->numrows=n/k;
          temp[i]->k=k;
          temp[i]->inrow=p/n;*/
        temp[i] = createnode();
        temp[i]->k = i;
        pthread_create(&threadscas[i], NULL, multiplychunkcas, temp[i]);
    }
    // auto start_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        pthread_join(threadscas[i], NULL);
    }
    auto end_time = chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationc = end_time - start_time;
    ofstream output("chunka3cas.output");
    // auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    output << "Time elapsed for cas section: " << durationc.count() << " seconds" << endl;
    cout << "Time elapsed for cas section: " << durationc.count() << " seconds" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p / n; j++)
        {
            output << resultm[i][j] << " ";
        }
        output << "\n";
    }
    output.close();
    return nullptr;
}

void *chunkbcas(void *args)
{
    pthread_t threadsbcas[n];
    struct node *tempbcas[k];
    // int rem=n%k;
    int t = n / k;
    /// 111
    /// 222
    // int tt=rem*(n/k+1)-rem*(n/k);
    auto start_time = chrono::high_resolution_clock::now();
    cout << "BCAS:\n";
    /* for(int i=0;i<k;i++){
     temp[i]=createnode();
     temp[i]->start=i*(t);
     temp[i]->numrows=n/k;
     temp[i]->k=k;
     temp[i]->inrow=p/n;
     pthread_create(&threads[i],NULL,multiplychunk,temp[i]);

     }*/
    int rem = n % k;
    for (int i = 0; i < k; i++)
    {
        /*  if(i<rem){
          temp[i]=createnode();
          temp[i]->start=i*((n/k)+1);
          temp[i]->numrows=n/k+1;
          temp[i]->k=k;
          temp[i]->inrow=p/n;
          pthread_create(&threads[i],NULL,multiplychunk,temp[i]);
          }
          else{
              temp[i]=createnode();
          temp[i]->start=rem*(n/k+1)+(i-rem)*(n/k);
          temp[i]->numrows=n/k;
          temp[i]->k=k;
          temp[i]->inrow=p/n;*/
        tempbcas[i] = createnode();
        tempbcas[i]->k = i;
        pthread_create(&threadsbcas[i], NULL, multiplychunkbcas, tempbcas[i]);
    }
    // auto start_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        pthread_join(threadsbcas[i], NULL);
    }
    auto end_time = chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationc = end_time - start_time;
    ofstream output("chunka3bcas.output");
    // auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    output << "Time elapsed for bcas section: " << durationc.count() << " seconds" << endl;
    cout << "Time elapsed for bcas section: " << durationc.count() << " seconds" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p / n; j++)
        {
            output << resultbcas[i][j] << " ";
        }
        output << "\n";
    }
    output.close();
    return nullptr;
}

void *chunkatomic(void *args)
{
    pthread_t threadsatomic[n];
    struct node *tempatomic[k];
    // int rem=n%k;
    int t = n / k;
    /// 111
    /// 222
    // int tt=rem*(n/k+1)-rem*(n/k);
    auto start_time = chrono::high_resolution_clock::now();
    cout << "ATOMIC:\n";
    /* for(int i=0;i<k;i++){
     temp[i]=createnode();
     temp[i]->start=i*(t);
     temp[i]->numrows=n/k;
     temp[i]->k=k;
     temp[i]->inrow=p/n;
     pthread_create(&threads[i],NULL,multiplychunk,temp[i]);

     }*/
    int rem = n % k;
    for (int i = 0; i < k; i++)
    {
        /*  if(i<rem){
          temp[i]=createnode();
          temp[i]->start=i*((n/k)+1);
          temp[i]->numrows=n/k+1;
          temp[i]->k=k;
          temp[i]->inrow=p/n;
          pthread_create(&threads[i],NULL,multiplychunk,temp[i]);
          }
          else{
              temp[i]=createnode();
          temp[i]->start=rem*(n/k+1)+(i-rem)*(n/k);
          temp[i]->numrows=n/k;
          temp[i]->k=k;
          temp[i]->inrow=p/n;*/
        tempatomic[i] = createnode();
        tempatomic[i]->k = i;
        pthread_create(&threadsatomic[i], NULL, multiplychunkatomic, tempatomic[i]);
    }
    // auto start_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        pthread_join(threadsatomic[i], NULL);
    }
    auto end_time = chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationc = end_time - start_time;
    ofstream output("chunka3atom.output");
    // auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    output << "Time elapsed for atomic section: " << durationc.count() << " seconds" << endl;
    cout << "Time elapsed for atomic section: " << durationc.count() << " seconds" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p / n; j++)
        {
            output << resultatom[i][j] << " ";
        }
        output << "\n";
    }
    output.close();
    return nullptr;
}

/*void *mixed(void *args){
    cout << "mixed:\n";
    pthread_t threadsm[n];
    struct node* tempm[k];
    int rem=n%k;
    //cout << "chunk:\n";
    auto start_timea = chrono::high_resolution_clock::now();
 /*   for(int i=0;i<k;i++){

    tempm[i]=createnode();
    tempm[i]->start=i;
    tempm[i]->numrows=n/k+1;
    tempm[i]->k=k;
    tempm[i]->inrow=p/n;
    pthread_create(&threadsm[i],NULL,multiplymixed,tempm[i]);
}*/

int main()
{

    auto start_time = chrono::high_resolution_clock::now();

    // Read matrix from file
    ifstream input("input.txt");
    if (!input.is_open())
    {
        cerr << "Unable to open matrix.txt" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    vector<int> tokens;
    int g = 0;
    while (getline(input, line))
    {
        stringstream ss(line);
        string token;

        while (getline(ss, token, ' '))
        {
            int val = stoi(token);
            tokens.push_back(val);
            g++;
        }
    }
    n = tokens[0];
    num = n;
    k = tokens[1];
    step = tokens[2];
    p = n * n;
    int o = 0;
    int a, b, var, s, ss;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            given[i][j] = tokens[3 + o];
            s = s + tokens[3 + o];
            ss = ss + tokens[3 + o] * tokens[3 + o];
            o++;
        }
    }

    input.close();

    pthread_t tech[4];
    pthread_create(&tech[0], NULL, chunktas, NULL);

    pthread_join(tech[0], NULL);
    counter = 0;
    pthread_create(&tech[1], NULL, chunkcas, NULL);
    pthread_join(tech[1], NULL);
    counter = 0;
    pthread_create(&tech[2], NULL, chunkbcas, NULL);
    pthread_join(tech[2], NULL);
    counteratom = 0;
    pthread_create(&tech[3], NULL, chunkatomic, NULL);
    pthread_join(tech[3], NULL);

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;
    //    cout << "Total time elapsed: " << duration.count() << " seconds" << endl;

    return 0;
}