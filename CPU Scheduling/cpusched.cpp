#include<iostream>
#include<cstdlib>
#include<pthread.h>
#include <chrono>
#include<string>
#include<cstring>
#include<vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <sched.h>
#include<sys/time.h>
using namespace std;
int n;
int k,p;
int cores,bt;
int result[10000][10000];
int resultm[10000][10000];
int resultnew[10000][10000];
int given[10000][10000];
int givennew[10000][10000];
int code[100000];
int sortedRowNumbers[10000];
double execution_timeschunk[10000];
double execution_timesmixed[10000];
cpu_set_t cpuset[8];
struct node{
    int start;
    int numrows;
    int k;
    int inrow;
    int ii;
};

struct node* createnode(){
    struct node* temp=new struct node;
    return temp;
}

void checkCPUAffinity(int it) {
    cpu_set_t mask;
    CPU_ZERO(&mask); // Initialize the CPU mask

    // Retrieve the CPU affinity mask for the current process
    if (sched_getaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
        std::cerr << "Error: Failed to retrieve CPU affinity mask" << std::endl;
        return;
    }

    std::cout << "CPU affinity mask for the process:" << std::endl;
    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &mask)) {
            std::cout << "CPU " << i << "for:" << it << " is available" << std::endl;
        }
    }
}

void *multiplymixed(void *args){
    struct node* temp=(struct node*)(args);
    int numrows=temp->numrows;
    int s=temp->start;
    int k=temp->k;
    int num=temp->inrow;
    int e=s+k*numrows+1;
    int sum,c;
    int i=temp->ii;
    struct timeval start_time[1000], end_time[1000];
    gettimeofday(&start_time[i], NULL);
    if(i<bt){
    CPU_ZERO(&cpuset[i]);
    if(bt>=cores){
    CPU_SET(i/(bt/cores), &cpuset[i]);
    }
    else{
        CPU_SET(i, &cpuset[i]);
    } 
   // cout << i/(bt/cores) << "dasdasdasdas\n";
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset[i]) != 0) {
        std::cerr << "Error: Failed to set CPU affinity for thread " << std::endl;
    }   
    }
       
    

    for(int r=s;r<e;r=r+k){
        for(int j=0;j<num;j++){
            sum=0;
            c=0;
            while(c!=num+1){
                sum=sum+given[r][c]*given[c][j];
                c++;
            }
            resultm[r][j]=sum;
        }
    }
    gettimeofday(&end_time[i], NULL);
    double start_seconds = start_time[i].tv_sec + (start_time[i].tv_usec / 1000000.0);
    double end_seconds = end_time[i].tv_sec + (end_time[i].tv_usec / 1000000.0);
    double durationcc = end_seconds - start_seconds;
    execution_timesmixed[i] = durationcc; 
    return nullptr;
}

void *multiplychunk(void *args) {
    struct node* temp = (struct node*)(args);
    int numrows = temp->numrows;
    int num = temp->inrow;
    int s = temp->start;
    int e = numrows + s - 1;
    int k = temp->k;
    int sum, c;
    int i = temp->ii;
    struct timeval start_time[1000], end_time[1000];
    gettimeofday(&start_time[i], NULL);
    
    // Set CPU affinity if within the bounded thread count
    if (i < bt) {
        CPU_ZERO(&cpuset[i]);
        if(bt>=cores){
        CPU_SET(i / (bt / cores), &cpuset[i]);
        }
        else{
            CPU_SET(i, &cpuset[i]);
        }
        if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset[i]) != 0) {
            std::cerr << "Error: Failed to set CPU affinity for thread " << std::endl;
        }
        //checkCPUAffinity(i);
    }
 //   checkCPUAffinity(i);

    for (int r = s; r <= e; r++) {
        for (int j = 0; j < num; j++) {
            c = 0;
            sum = 0;
            while (c != num) {
                sum += given[r][c] * given[c][j];
                c++;
            }
     //       cout << sum << " ";
            result[r][j] = sum;
        }
    }

    gettimeofday(&end_time[i], NULL);
    double start_seconds = start_time[i].tv_sec + (start_time[i].tv_usec / 1000000.0);
    double end_seconds = end_time[i].tv_sec + (end_time[i].tv_usec / 1000000.0);
    double durationcc = end_seconds - start_seconds;
    execution_timeschunk[i]=durationcc;
 //   cout << "Time elapsed: " << durationcc << " seconds" << endl;
    execution_timeschunk[i] = durationcc;

    return nullptr;
}

void *chunk(void *args){
    pthread_t threads[n];
    struct node* temp[k];
    //int rem=n%k;
    int t=n/k;
    auto start_time = chrono::high_resolution_clock::now();
  //  cout << "chunk:\n";
   
    int rem=n%k;
    for(int i=0;i<k;i++){
    if(i<rem){
    temp[i]=createnode();
    temp[i]->start=i*((n/k)+1);
    temp[i]->numrows=n/k+1;
    temp[i]->k=k;
    temp[i]->inrow=p/n;
    temp[i]->ii=i;
    pthread_create(&threads[i],NULL,multiplychunk,temp[i]);
    }
    else{
        temp[i]=createnode();
    temp[i]->start=rem*(n/k+1)+(i-rem)*(n/k);
    temp[i]->numrows=n/k;
    temp[i]->k=k;
    temp[i]->inrow=p/n;
    temp[i]->ii=i;
    pthread_create(&threads[i],NULL,multiplychunk,temp[i]);
    }
    
    }
    for(int i=0;i<k;i++){
        pthread_join(threads[i],NULL);
    }
    auto end_time = chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationc = end_time - start_time;
    ofstream output("chunk.output");
    //auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    double timeb = 0, timenonb = 0;

// Calculate total execution time for bounded and non-bounded threads
for (int tc = 0; tc < bt; tc++) {
    timeb+=execution_timeschunk[tc];
}
for (int tc = bt; tc < k; tc++) {
    timenonb+=execution_timeschunk[tc];
}
    output << "average time for bounded chunk threads: " << timeb/bt << " seconds" << endl;
    output << "average time for non-bounded chunk threads: " << timenonb/(k-bt)<< " seconds" << endl;
    output << "Time for chunk section: " << durationc.count() << " seconds" << endl;
    for(int i=0;i<n;i++){
        for(int j=0;j<p/n;j++){
            output << result[i][j] << " ";
        }
        output << "\n";
    }
    output.close();
    return nullptr;
}

void *mixed(void *args){
    //cout << "mixed:\n";
    pthread_t threadsm[n];
    struct node* tempm[k];
    int rem = n % k;
    struct timeval start_time[32], end_time[32];
    gettimeofday(&start_time[0], NULL); // Start timer for mixed section

    for(int i = 0; i < k; i++) {
        if(i < rem) {
            tempm[i] = createnode();
            tempm[i]->start = i;
            tempm[i]->numrows = n / k + 1;
            tempm[i]->ii = i;
        }
        else {
            tempm[i] = createnode();
            tempm[i]->start = i;
            tempm[i]->numrows = n / k;
            tempm[i]->ii = i;
        }
        tempm[i]->k = k;
        tempm[i]->inrow = p / n;
        pthread_create(&threadsm[i], nullptr, multiplymixed, tempm[i]);
    }

    for(int i = 0; i < k; i++) {
        pthread_join(threadsm[i], nullptr);
    }

    ofstream output("mixed.output");
    gettimeofday(&end_time[0], NULL); // End timer for mixed section
    double start_seconds = start_time[0].tv_sec + (start_time[0].tv_usec / 1000000.0);
    double end_seconds = end_time[0].tv_sec + (end_time[0].tv_usec / 1000000.0);
    double duration = end_seconds - start_seconds;

    
    double timeb=0,timenonb=0;
    for (int tc = 0; tc < bt; tc++) {
    timeb+=execution_timesmixed[tc];
}
for (int tc = bt; tc < k; tc++) {
    timenonb+=execution_timesmixed[tc];
}
    output << "Time elapsed for mixed section: " << duration << " seconds" << endl;
    //cout << "Time elapsed for mixed section: " << duration << " seconds" << endl;
    output << "average time for bounded mixed threads: " << timeb/bt  << "seconds" << endl;
    output << "average time for non-bounded mixed threads: " << timenonb/(k-bt)<< "seconds" << endl;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < p / n; j++) {
            output << resultm[i][j] << " ";
        }
        output << "\n";
    }
    
    
    
    // Print the result matrix
    output.close();    

    return nullptr;
}


void multiplynew(int i){
    int sum=0,c=0;
        for(int k=0;k<n;k++){
            while(c!=n){
            sum=sum+given[i][c]*given[c][k];
            c++;
            }
            resultnew[i][k]=sum;
            sum=0;
            c=0;
        }

}





int main() {
  //  checkCPUAffinity(-1);
    auto start_time = chrono::high_resolution_clock::now();


    // Read matrix from file
    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Unable to open matrix.txt" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    vector<int> tokens;
    int g=0;
    while (getline(input, line)) {
        stringstream ss(line);
        string token;

        while (getline(ss, token, ' ')) {
            int val = stoi(token);
            tokens.push_back(val);
            g++;
        }
    }
    n = tokens[0];
    k = tokens[1];
    cores=tokens[2];
    bt=tokens[3];
    cout << n << " " << k << " " << cores << " " << bt << "\n";
    p = n * n;
    int o = 0;
    int a,b,var,s,ss;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            given[i][j]=tokens[4+o];
            o++;
        }
       
        
    }

    input.close();

    pthread_t tech[3];
    pthread_create(&tech[0], NULL, chunk, NULL);
    
    
    pthread_join(tech[0], NULL);
    pthread_create(&tech[1], NULL, mixed, NULL);
    pthread_join(tech[1], NULL);


    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;
//    cout << "Total time elapsed: " << duration.count() << " seconds" << endl;
 /*   for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout << given[i][j] << " ";
        }
        cout << "\n";
    }*/
    return 0;
}
