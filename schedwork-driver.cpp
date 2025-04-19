// main() program to test your schedule function.
// Feel free to modify this program in any way you need for testing.
// It will not be graded. 
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "schedwork.h"

using namespace std;

void printSchedule(const DailySchedule& sched);

int main()
{
    // Replace the matrix below with your test case
    AvailabilityMatrix avail = { 
        {1, 1, 1, 1},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {1, 0, 0, 1}
    };
    DailySchedule sched;
    bool solutionFound = schedule(avail, 2, 2, sched);
    if(solutionFound)
    {
        printSchedule(sched);
    }
    else
    {
        cout << "No solution found!" << endl;
    }
    return 0;
}

void printAvailability(const AvailabilityMatrix& avail)
{

    cout << "worker: ";
    for (int i = 0; i < avail.size(); i++) {
        cout << i << " ";
    }
    cout << endl;

    int day = 0;
    for(vector<bool> d : avail)
    {
        cout << "Day " << day << ": ";
        for(bool aval : d)
        {
            cout << boolalpha << aval << " ";
        }
        cout << endl;
        day++;
    }
}

void printSchedule(const DailySchedule& sched)
{
    int day = 0;
    for(auto s : sched)
    {
        cout << "Day " << day << ": ";
        for(auto nurse : s)
        {
            cout << nurse << " ";
        }
        cout << endl;
        day++;
    }
}
