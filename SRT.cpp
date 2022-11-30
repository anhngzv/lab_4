#include <iostream>
#include <queue>
#include <string.h>
#include <limits.h>
#include <algorithm>
using namespace std;

const int MAXN = 1000;
const float INF = 1e9;
using namespace std;

struct process {
    string name;
    int id;
    float arrival_time, burst_time;
    float response_time = 0, turn_arround_time = 0, waiting_time = 0;
};

bool operator < (const process &A, const process &B) {
    return A.burst_time > B.burst_time; //for priority_queue
}

int number_of_processes;
vector<process> P;

bool compareFunc(process &a, process &b) {
    return a.arrival_time < b.arrival_time;
}

void print_result(vector<process> &P) {
    float avg_waiting_time = 0, avg_turn_arround_time = 0;

    for(int i=0; i < number_of_processes; i++) {
        cout << "\n";
        cout << "Process id: " << P[i].id +1<< "\n";
        cout << "Process name: " << P[i].name << "\n";
        cout << "Responese time: " << P[i].response_time << "\n";
        cout << "Waiting time: " << P[i].waiting_time << "\n";
        cout << "Turn-arround time: " << P[i].turn_arround_time << "\n";
        avg_waiting_time += P[i].waiting_time;
        avg_turn_arround_time += P[i].turn_arround_time;
    }
    cout << "\n";
    cout << "Average waiting time: " << avg_waiting_time/number_of_processes << "\n";
    cout << "Average turn-arround time: " << avg_turn_arround_time/number_of_processes << "\n\n";
}

void SRT_algo(vector<process> &P)
{
    vector<process> p(P.begin(), P.end()); 
    std::sort(p.begin(), p.end(), compareFunc);
    priority_queue<process> Queue;

    int number_of_finished_processes = 0, pointer = 0;
    float elapsed_time = 0;
    float next_arrival_time = 0;


    while (true) {
        bool inserted = false;
        while (pointer < number_of_processes && p[pointer].arrival_time <= elapsed_time) {
            Queue.push(p[pointer]);
            pointer++;
            if (pointer==number_of_processes) next_arrival_time = INF;
            else next_arrival_time = p[pointer].arrival_time;
            inserted = true;
        } 
        if (!inserted && pointer < number_of_processes) {
            elapsed_time = p[pointer].arrival_time;
            continue;
        }
        if (!Queue.size()) break;

        process current_process = Queue.top();
        Queue.pop();
        float expexted_time = elapsed_time + current_process.burst_time;
        if (expexted_time <= next_arrival_time) {
            elapsed_time += current_process.burst_time;
            if (P[current_process.id].response_time) P[current_process.id].response_time = elapsed_time;
            P[current_process.id].turn_arround_time = max(current_process.burst_time, elapsed_time - P[current_process.id].arrival_time);
            P[current_process.id].waiting_time = P[current_process.id].turn_arround_time - P[current_process.id].burst_time;
            number_of_finished_processes++;
        }
        else {
            process tmp = current_process;
            if (P[current_process.id].response_time) P[current_process.id].response_time = elapsed_time;
            tmp.burst_time = current_process.burst_time - (next_arrival_time - elapsed_time);
            Queue.push(tmp);
            elapsed_time = next_arrival_time;
        }
    }
    print_result(P); 
}

int main (int argc, char *argv[])
{
    std::cout << "Enter the number of processes:";
    std::cin >> number_of_processes;
    P.resize(number_of_processes);
    for(int i=0; i < number_of_processes; i++) {
        cout << "\n*Process " << i+1 << ":\n";
        cout << "Enter Process Name: ";
        cin >> P[i].name;
        cout << "Enter arrival time: ";
        cin >> P[i].arrival_time;
        cout << "Enter burst time: ";
        cin >> P[i].burst_time;
        P[i].id = i;
    }
    SRT_algo(P);
    return 0;
}
