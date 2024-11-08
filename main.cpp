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
    Process(int id, std::string name, int arrival, int burst, int memory,int Position,int priority)
        : ID(id), NameProcess(name), ArrivalTime(arrival), BurstTime(burst), WaitingTime(0),
          MemoryUsed(memory), Position(Position), RemainingTime(burst), Priority(priority), InQueue(false) {}

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
        std::cout << "Priority:" << Priority <<"\n"<<std::endl;
    }

};

// Round-Robin scheduling function
void RoundRobin(vector<Process>& processes, int quantum) {
    std::queue<Process> ReadyQueue;
    int currentTime = 0;

    // Sort processes based on ArrivalTime
    sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
            return p1.ArrivalTime < p2.ArrivalTime;
    });

    cout << "========RR SCHEDULING==========\n";
    //Will be used to contain processes to be executed
    vector<Process> Copy_processes = processes;
    //To register the time at which the next process to be scheduled will enter
    int EntryTime = currentTime;
    int TimeSlice;
    //Adding the first process to the ready queue
    ReadyQueue.push(Copy_processes[0]);
    Copy_processes[0].InQueue = true;

    while(true)
    {

        if (Copy_processes.size() == 1)
        {
            currentTime = ReadyQueue.front().RemainingTime + EntryTime;
            Copy_processes[0].RemainingTime = 0;

            Copy_processes[0].WaitingTime = currentTime - Copy_processes[0].ArrivalTime - Copy_processes[0].BurstTime;
            Copy_processes[0].display();
            cout<<"=======Execution completed for process "<<ReadyQueue.front().NameProcess<<"==========\n";
            break;
        }
        //Executing the first process in the ready queue
        TimeSlice = std::min(quantum, ReadyQueue.front().RemainingTime);
        ReadyQueue.front().RemainingTime = ReadyQueue.front().RemainingTime - TimeSlice;
        currentTime += TimeSlice;
        EntryTime = currentTime;
        ReadyQueue.front().display();
        cout<<"Current time:"<<currentTime<<"\n";

        //Adding newly arrived processes to the queue
        for (size_t i = 0; (i<Copy_processes.size()) && (Copy_processes[i].ArrivalTime <= EntryTime) && (Copy_processes[i].InQueue == false);i++)
        {
            ReadyQueue.push(Copy_processes[i]);
            Copy_processes[i].InQueue = true;
        }

        //Checking if the process still has to execute
        if (ReadyQueue.front().RemainingTime>0)
        {
            ReadyQueue.push(ReadyQueue.front());
            ReadyQueue.pop();
        }
        else
        {
        //Removing a process after its execution
            for (size_t i = 0; i<Copy_processes.size();i++)
                {
                    if(Copy_processes[i].ArrivalTime == ReadyQueue.front().ArrivalTime)
                    {
                        ReadyQueue.front().WaitingTime = currentTime - ReadyQueue.front().ArrivalTime - ReadyQueue.front().BurstTime;
                        ReadyQueue.front().display();
                        cout<<"Current time:"<<currentTime<<"\n";
                        cout<<"=======Execution completed for process "<<ReadyQueue.front().NameProcess<<"==========\n";
                        ReadyQueue.pop();
                        Copy_processes.erase(Copy_processes.begin() + i);
                        break;
                    }

                }
        }
    }
    cout<<"END OF SCHEDULING";
}

int main()
{
       // Array of processes
    vector<Process> processes;

    // Number of processes
    int N,quantumTime;
    cout << "Enter number of processes: ";
    cin >> N;

    // Input process details
    for (int i = 0; i < N; ++i) {
        int id, arrival, burst, memory, position, priority;
        string name;

        cout << "Enter Process ID: ";
        cin >> id;
        cout << "Enter Process Name: ";
        cin >> name;
        cout << "Enter Arrival Time: ";
        cin >> arrival;
        cout << "Enter Burst Time: ";
        cin >> burst;
        cout << "Enter Memory Used: ";
        cin >> memory;
        cout << "Enter Position: ";
        cin >> position;
        cout << "Enter Priority: ";
        cin >> priority;

        processes.push_back(Process(id, name, arrival, burst, memory, position, priority));
    }
    // Input quantum time
    cout << "Enter the quantum time: ";
    cin >> quantumTime;
    // Apply RoundRobin scheduling
    RoundRobin(processes, quantumTime);
    return 0;
}
