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
#include <sched.h>
#include <semaphore.h>
#include <sys/time.h>
#include <chrono>
#include <random>
using namespace std;
std::random_device rd;
std::mt19937 gen(rd());
double average1 = 10000;
double average2 = 5000;
double lambda1 = 1.0 / average1;
double lambda2= 1.0 / average2;
std::exponential_distribution<> dist1(lambda1);//functions for random exponential function
std::exponential_distribution<> dist2(lambda2);
int nw, nr, kw, kr;
int ww, onr,inc,afc;
sem_t writechange, sread, swrite, readchange, printing;
ofstream output1("RWlog.output");
ofstream output2("fairRWlog.output");
double reader1=0,writer1=0,reader2=0,writer2=0,maxr1=0,maxw1=0,maxr2=0,maxw2=0;
//chrono::steady_clock::time_point programStart,programStart2; // To store the start time of the program
chrono::high_resolution_clock::time_point programStart1;
chrono::high_resolution_clock::time_point programStart2;
void* reader(void *args) {
    pthread_t *thread = (pthread_t *)args;
    pthread_t id = pthread_self();
  //  auto programStart1 = chrono::high_resolution_clock::now(); // Record the start time of the reader function
    for(int i = 0; i < kr; i++) {
      //  cout << dist1(gen) <<endl;
        sem_wait(&printing);//for printing mutually exclusive in file
        auto start_time = chrono::high_resolution_clock::now();
        output1 << i << "th CS request by reader thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(start_time - programStart1).count() << "ms" << endl;
        sem_post(&printing);
        sem_wait(&sread);//access if no writer is waiting
        sem_wait(&printing);
        auto entry_time = chrono::high_resolution_clock::now();
        output1 << i << "th CS entry by reader thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(entry_time - programStart1).count() << "ms" << endl;
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(entry_time - start_time).count(); 
        reader1 += elapsed; //helps to find average
        if (elapsed > maxr1) {//check for worst case
            maxr1 = elapsed;
        }
        sem_post(&printing);
        if (onr == 0) {
            sem_wait(&swrite);//first reader thread makes further requesting writer threads to wait
        }
        onr++;
        sem_post(&sread);
        usleep(dist1(gen));
        sem_wait(&readchange);
        if (onr == 1) {
            sem_post(&swrite);//last reader thread allowing further requesting writer thread to access critical section
        }
        onr--;
        sem_wait(&printing);//for printing mutually exclusive in file
        auto exit_time = chrono::high_resolution_clock::now();
        output1 << i << "th CS exit by reader thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(exit_time - programStart1).count() << "ms" << endl;
        sem_post(&printing);
        sem_post(&readchange);
        usleep(dist2(gen));
    }
    return NULL;
}

void* writer(void *args) {
    pthread_t *thread = (pthread_t *)args;
    pthread_t id = pthread_self();
    //struct timeval req_time, start_time, end_time;
    
    for(int i = 0; i < kw; i++) { 
        sem_wait(&printing);//for printing mutually exclusive in file
        auto req_time = chrono::high_resolution_clock::now();   
        output1 << i << "th CS request by writer thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(req_time -programStart1).count() << "ms" << endl;
        sem_post(&printing);
        sem_wait(&writechange);//checking ww mutually exclusive
        if(ww == 0) {
            sem_wait(&sread);//first waiting thread blocks further requesting reader thread,waits if readers are in critical section
        }
        ww++;

        sem_post(&writechange);
        sem_wait(&swrite);//allow only 1 writer thread in critical section
        sem_wait(&printing);//for printing mutually exclusive in file
        auto start_time = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(start_time - req_time).count(); // Elapsed time in microseconds
        writer1 = writer1+ elapsed;//sum to find average time to access critical section
        output1 << i << "th CS entry by writer thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(start_time - programStart1).count() <<"ms" <<endl; // Elapsed time in microseconds - programStart).count() << "ms" << endl;
        if(elapsed>maxw1){
            maxw1=elapsed;
        }
        sem_post(&printing);
        usleep(dist1(gen));//randomly generated time
        sem_wait(&writechange);
        ww--;
        if (ww == 0) {
            int value;
            sem_post(&sread);//if no writer thread is waiting,allows reader to access critical section
        }
        sem_post(&writechange);
        auto end_time=chrono::high_resolution_clock::now();
        sem_wait(&printing);//for printing mutually exclusive in file
        output1 << i << "th CS exit by writer thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(end_time - programStart1).count() << "ms" << endl;
        sem_post(&printing);
        sem_post(&swrite);
        usleep(dist2(gen));
    }
    return NULL;
}

sem_t common, fwriter, countreaders, freader;
int waitingreaders = 0, workingreaders = 0;

void* readerfair(void *args) {
    pthread_t *thread = (pthread_t *)args;
    pid_t id = pthread_self();
    struct timeval req_time, start_time, end_time;
    for(int i = 0; i < kr; i++) {
        sem_wait(&printing);
        auto req_time=chrono::high_resolution_clock::now();
        output2 << i << "th CS request by reader thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(req_time - programStart2).count() << "ms" << endl;
        sem_post(&printing);//for printing mutually exclusive in file
        sem_wait(&common);//sleep if any writer requested before or reader is about to enter critical section
        sem_wait(&freader);//update working readers
        waitingreaders--;
        if(workingreaders == 0) {
            sem_wait(&fwriter);//first reader does not allow writer to enter critical section
        }
        workingreaders++;
        sem_post(&common);
        sem_post(&freader);
        sem_wait(&printing);//for printing mutually exclusive in file
        auto start_time=chrono::high_resolution_clock::now();
        auto elapsed=chrono::duration_cast<std::chrono::milliseconds>(start_time -req_time).count();
        reader2 += elapsed;//sum time to find average
        output2 << i << "th CS entry by reader thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(start_time - programStart2).count() << "ms" << endl;
        if(elapsed>maxr2){//update worst case time after checking
            maxr2=elapsed;
        }
        sem_post(&printing);
        usleep(dist1(gen));
        sem_wait(&freader);//updating working readers
        workingreaders--;
        
        sem_wait(&printing);//for printing mutually exclusive in file
        auto end_time=chrono::high_resolution_clock::now();
        output2 << i << "th CS exit by reader thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(end_time - programStart2).count()  << "ms" << endl;
        sem_post(&printing);
        if(workingreaders == 0) {
            sem_post(&fwriter);//if all readers exit critical section,allow writer thread to enter
        }
        sem_post(&freader);
        usleep(dist2(gen));
    }
    return NULL;
}

void* writerfair(void* args) {
    pthread_t* thread = (pthread_t*)args;
    pid_t id = pthread_self();
    struct timeval req_time, start_time, end_time;
    for(int i = 0; i < kw; i++) {
        auto req_time = chrono::high_resolution_clock::now();
        sem_wait(&printing);//for printing mutually exclusive in file
        output2 << i << "th CS request by writer thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(req_time - programStart2).count() << "ms" << endl;
        sem_post(&printing);
        sem_wait(&common);  //sleep if any reader requested earlier 

        sem_wait(&fwriter);  // to allow only one writer in critical section

        auto start_time = chrono::high_resolution_clock::now();
        sem_wait(&printing);//for printing mutually exclusive in file
        auto elapsed=chrono::duration_cast<std::chrono::milliseconds>(start_time -req_time).count();
        writer2 += elapsed;//sum time to access critical section
        output2 << i << "th CS entry by writer thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(start_time - programStart2).count() << "ms" << endl;
        if(elapsed>maxw2){//update worst case time
            maxw2=elapsed;
        }
        sem_post(&printing);
        usleep(dist1(gen));
        sem_wait(&printing);//for printing mutually exclusive in file
        auto end_time=chrono::high_resolution_clock::now();
        output2 << i << "th CS exit by writer thread" << id << " at " << chrono::duration_cast<chrono::milliseconds>(end_time - programStart2).count() << "ms" << endl;
        sem_post(&printing);
        sem_post(&fwriter);//allow waiting writer to enter
            sem_post(&common);

        usleep(dist2(gen));
    }
    return NULL;
}

int main() {
   programStart1 = chrono::high_resolution_clock::now();

    ww = 0;//initialise variables 
    onr = 0;
   // nw = 2, nr = 3, kw = 2, kr = 1;
   ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Unable to open matrix.txt" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    vector<int> tokens;
    int g=0;
    while (getline(input, line)) {//extract inputs
        stringstream ss(line);
        string token;

        while (getline(ss, token, ' ')) {
            int val = stoi(token);
            tokens.push_back(val);
            g++;
        }
    }
    nr = tokens[0];//assign inputs to variables
    nw = tokens[1];
    kr=tokens[2];
    kw=tokens[3];
    inc=tokens[4];
    afc=tokens[5];
    average1=inc;
    average2=afc;
    lambda1=1.0/average1;
    lambda2=1.0/average2;
    dist1 = std::exponential_distribution<>(lambda1); // Update dist1
    dist2 = std::exponential_distribution<>(lambda2); // Update dist2
    sem_init(&writechange, 0, 1);//initialise semaphores to use them
    sem_init(&sread, 0, 1);
    sem_init(&swrite, 0, 1);
    sem_init(&readchange, 0, 1);
    sem_init(&printing, 0, 1);

    pthread_t threadw[nw];//create reader and writer threads
    pthread_t threadr[nr];
    for(int i = 0; i < nw; i++) {
        pthread_create(&threadw[i], NULL, writer, NULL);
    }
    for(int i = 0; i < nr; i++) {
        pthread_create(&threadr[i], NULL, reader, NULL);
    }
    for(int i = 0; i < nw; i++) {
        pthread_join(threadw[i], NULL);//join threads
    }
    for(int i = 0; i < nr; i++) {
        pthread_join(threadr[i], NULL);
    }
    
    sem_init(&freader, 0, 1);//initialise semaphores for second algorithm
    sem_init(&fwriter, 0, 1);
    sem_init(&countreaders, 0, 1);
    sem_init(&common, 0, 1);
    programStart2 = chrono::high_resolution_clock::now();

    for(int i = 0; i < nw; i++) {
        pthread_create(&threadw[i], NULL, writerfair, NULL);
    }
    for(int i = 0; i < nr; i++) {
        pthread_create(&threadr[i], NULL, readerfair, NULL);
    }
    for(int i = 0; i < nw; i++) {
        pthread_join(threadw[i], NULL);
    }
    for(int i = 0; i < nr; i++) {
        pthread_join(threadr[i], NULL);
    }
    ofstream output3("Average time.txt");//calculate average
   // output3 << "reader : " <<programStart1.count()/nr << endl;
   /*cout << average1<<" "<<average2<<endl;
    for(int i=0;i<10;i++){
        cout << dist1(gen) <<" "<< dist2(gen)<<endl;
    }*/
    output3 << "average time to access critical section for reader in writer preference reader writer problem in ms per access:" <<reader1/(nr*kr) << endl;
    output3 << "average time to access critical section for writer in writer preference reader writer problem in ms per access:" <<writer1/(nw*kw) << endl;
    output3 << "average time to access critical section for reader in fair reader writer problem in ms per access:" <<reader2/(nr*kr) << endl;
    output3 << "average time to access critical section for writer in fair reader writer problem in ms per access:" <<writer2/(nw*kw) << endl;
    output3 << "average time to access critical section after request for writer preference reader writer problem in ms per thread:" <<(reader1+writer1)/(nr+nw);
    output3 << "average time to access critical section after request for fair reader writer problem in ms per thread:" <<(reader2+writer2)/(nr+nw);
    cout << "worst case entry time for reader in writer preference reader writer problem in ms:" << maxr1 <<endl;
    cout << "worst case entry time for writer in writer preference reader writer problem in ms:" << maxw1 <<endl;
    cout << "worst case entry time for reader in fair reader writer problem in ms:" << maxr2 <<endl;
    cout << "worst case entry time for writer in fair reader writer problem in ms:" << maxw2 <<endl;
    return 0;
}
