#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // for sorting
using namespace std;

class Process {
public:
    // Public attributes
    int ID;              // Unique identifier for the process
    std::string NameProcess;  // Name of the process
    int ArrivalTime;      // Time at which the process arrives
    int BurstTime;        // Time required for the process to complete execution
    int WaitingTime;      // Time the process has waited in the ready queue
    int MemoryUsed;       // Memory used by the process
    int Position;         // Position in a queue or other data structure
    int RemainingTime;    // Time remaining for the process to finish execution
    int Priority;         // Priority of the process (lower value indicates higher priority)
    bool InQueue;          //Specifying whether a process is in the ready queue or not

    // Constructor to initialize the attributes
    Process(int id, std::string name, int arrival, int burst, int memory, int position, int priority)
        : ID(id), NameProcess(name), ArrivalTime(arrival), BurstTime(burst), WaitingTime(0),
          MemoryUsed(memory), Position(position), RemainingTime(burst), Priority(priority), InQueue(false) {}

    // Method to display process information
    void display() {
        std::cout << "Process ID: " << ID << std::endl;
        std::cout << "Process Name: " << NameProcess << std::endl;
        std::cout << "Arrival Time: " << ArrivalTime << std::endl;
        std::cout << "Burst Time: " << BurstTime << std::endl;
        std::cout << "Waiting Time: " << WaitingTime << std::endl;
        std::cout << "Memory Used: " << MemoryUsed << " MB" << std::endl;
        std::cout << "Position: " << Position << std::endl;
        std::cout << "Remaining Time: " << RemainingTime << std::endl;
        std::cout << "Priority: " << Priority << std::endl;
    }

};

// Round-Robin scheduling function
void RoundRobin(vector<Process>& processes, int quantum) {
    std::queue<Process*> ReadyQueue;
    int currentTime = 0;

    // Sort processes based on ArrivalTime
    sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.ArrivalTime < p2.ArrivalTime;
    });

    cout << "========RR SCHEDULING==========\n";

    // Adding the first process to the ready queue
    ReadyQueue.push(&processes[0]);
    processes[0].InQueue = true;

    while (!ReadyQueue.empty()) {
        Process* currentProcess = ReadyQueue.front();
        ReadyQueue.pop();

        int TimeSlice = std::min(quantum, currentProcess->RemainingTime);
        currentProcess->RemainingTime -= TimeSlice;
        currentTime += TimeSlice;
        currentProcess->display();
        cout << "Current time: " << currentTime << "\n";

        // Adding newly arrived processes to the queue
        for (size_t i = 0; i < processes.size(); ++i) {
            if (!processes[i].InQueue && processes[i].ArrivalTime <= currentTime) {
                ReadyQueue.push(&processes[i]);
                processes[i].InQueue = true;
            }
        }

        // If the process still has remaining time, add it back to the queue
        if (currentProcess->RemainingTime > 0) {
            ReadyQueue.push(currentProcess);
        } else {
            currentProcess->WaitingTime = currentTime - currentProcess->ArrivalTime - currentProcess->BurstTime;
            currentProcess->display();
            cout << "======= Execution completed for process " << currentProcess->NameProcess << " =======\n";
        }
    }

    cout << "END OF SCHEDULING\n";
}

int main() {
    // Array of processes
    vector<Process> processes;

    // Number of processes
    int N = 5, quantumTime;
    srand(time(0));

    // Input process details
    for (int i = 0; i < N; ++i) {
        processes.push_back(Process(i, "P" + to_string(i), rand() % 10, rand() % 10 + 1, rand() % 100 + 30, rand() % 10, rand() % 10 + 1));
    }

    // Input quantum time
    cout << "Enter the quantum time: ";
    cin >> quantumTime;

    // Apply RoundRobin scheduling
    RoundRobin(processes, quantumTime);
    return 0;
}
