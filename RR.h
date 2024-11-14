#ifndef RR_H
#define RR_H

#include <iostream>
#include <climits>
#include <vector>
#include "Processus.h"

class RR {
public:
    void findWaitingTimeRR(std::vector<Processus>& proc, int quantum, int currentTime) {
        int t = 0; // temps actuel
        std::vector<int> rem_bt(proc.size());
        for (size_t i = 0; i < proc.size(); i++) {
            rem_bt[i] = proc[i].burstTime;
        }
        while (true) {
            bool done = true;
            for (size_t i = 0; i < proc.size(); i++) {
                if (rem_bt[i] > 0) {
                    done = false; // Il reste des Processus à traiter
                    if (rem_bt[i] > quantum) {
                        t += quantum;
                        rem_bt[i] -= quantum;
                    } else {
                        t += rem_bt[i];
                        proc[i].waitingTime = t - proc[i].burstTime;
                        rem_bt[i] = 0;
                    }
                }
            }
            if (done) break;
        }
    }

    void findTurnAroundTimeRR(std::vector<Processus>& proc) {
        for (auto& p : proc) {
            p.turnAroundTime = p.burstTime + p.waitingTime;
        }
    }

    void findAvgTimeRR(std::vector<Processus>& proc, int quantum) {
        int totalWaitingTime = 0, totalTurnAroundTime = 0;
        findWaitingTimeRR(proc, quantum, 0);
        findTurnAroundTimeRR(proc);

        std::cout << "Processus\t\tBurst Time\t\tWaiting Time\t\tTurnaround Time\n";

        for (const auto& p : proc) {
            totalWaitingTime += p.waitingTime;
            totalTurnAroundTime += p.turnAroundTime;
            std::cout << "    " << p.pid << "   \t\t\t" << p.burstTime << "   \t\t\t" << p.waitingTime<< "   \t\t\t" << p.turnAroundTime << std::endl;

        }

        std::cout << "Average waiting time = " << static_cast<float>(totalWaitingTime) / proc.size() << std::endl;
        std::cout << "Average turnaround time = " << static_cast<float>(totalTurnAroundTime) / proc.size() << std::endl;
    }

    void runRR(std::vector<Processus>& proc, int quantum) {
        std::sort(std::begin(proc), std::end(proc), [](const Processus& a, const Processus& b) {
            return a.arrivalTime < b.arrivalTime;
        });
        findAvgTimeRR(proc, quantum);
    }

    void printGanttChart(const std::vector<Processus>& proc, int quantum) {
        std::cout << "\nGantt Chart:\n";
        int t = 0;
        std::vector<std::string> chart;
        std::vector<int> rem_bt(proc.size());
        for (size_t i = 0; i < proc.size(); i++) {
            rem_bt[i] = proc[i].burstTime;
        }
        while (true) {
            bool done = true;
            for (size_t i = 0; i < proc.size(); i++) {
                if (rem_bt[i] > 0) {
                    done = false; // Il reste des Processus à traiter
                    if (rem_bt[i] > quantum) {
                        t += quantum;
                        chart.push_back("P" + std::to_string(proc[i].pid));
                        rem_bt[i] -= quantum;
                    } else {
                        t += rem_bt[i];
                        chart.push_back("P" + std::to_string(proc[i].pid));
                        rem_bt[i] = 0;
                    }
                }
            }
            if (done) break;
        }
        for (const auto& p : chart) {
            std::cout << "| " << p << " ";
        }
        std::cout << "|\n0";
        t = 0;
        for (const auto& p : chart) {
            t += quantum;
            std::cout << "    " << t;
        }
        std::cout << "\n";
    }
};

int mainRR(std::vector<Processus> proc, int quantum) {
    int currentTime = 0;
    RR rr;
    rr.runRR(proc, quantum);
    rr.printGanttChart(proc, quantum);
    return 0;
}

#endif // RR_H
