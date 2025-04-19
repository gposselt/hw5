#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

#include <ext/concurrence.h>

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
    ) {
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    createAndFillSchedule(avail, dailyNeed, sched);

    return scheduleInteral(avail, dailyNeed, maxShifts, sched);

}

bool scheduleInteral(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t current_day,
    size_t current_shift
    ) {

    if (current_shift >= dailyNeed) {
        current_day += 1;
        current_shift = 0;

        if (current_day >= avail.size()) {
            return schedueSatisfiesConstraints(avail, dailyNeed, maxShifts, sched, current_day, current_shift);
        }
    }

    const std::vector<bool>& currentDayAvailability = avail[current_day];


    for (Worker_T currentWorkerID = 0; currentWorkerID < currentDayAvailability.size(); currentWorkerID++) {

        if (!currentDayAvailability[currentWorkerID]) continue;

        sched[current_day][current_shift] = currentWorkerID;

        if (!schedueSatisfiesConstraints(avail, dailyNeed, maxShifts, sched, current_day, current_shift)) {

            continue;
        }

        if (scheduleInteral(avail, dailyNeed, maxShifts, sched, current_day, current_shift + 1)) {
            return true;
        }

    }


    // sched[current_day][current_day] = INVALID_ID;

    return false;
}


bool schedueSatisfiesConstraints(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    const DailySchedule& sched,
    const size_t to_day,
    const size_t to_shift
    ) {
    std::vector<size_t> worker_shifts;
    for (size_t i = avail[0].size(); i --> 0;)
        worker_shifts.push_back(0u);


    std::set<Worker_T> workersPerDay{};
    return scheduleConformsToMaxShifts(avail, dailyNeed, maxShifts, sched, to_day, to_shift, workersPerDay, worker_shifts, 0, 0);


}

bool scheduleConformsToMaxShifts(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    const DailySchedule& sched,
    const size_t to_day,
    const size_t to_shift,
    std::set<Worker_T>& workersPerDay,
    std::vector<size_t>& worker_shifts,
    size_t current_day,
    size_t current_shift
    ){

    //iterated through the section we need to and found no errors
    if (current_shift >= to_shift && current_day >= to_day) {
        return true;
    }

    //advance day
    if (current_shift >= sched[current_day].size()) {
        current_day += 1;
        current_shift = 0;
        workersPerDay.clear();
    }

    //finished iterating
    if (current_day >= sched.size()) {
        return true;
    }

    Worker_T current_worker = sched[current_day][current_shift];
    std::pair<std::set<Worker_T>::iterator, bool> insertResult = workersPerDay.insert(current_worker);

    if (!insertResult.second)
        return false;

    if (current_worker != INVALID_ID) {
        worker_shifts[current_worker]++;
    }else {
        //check for invalid ids
        return false;
    }

    if (worker_shifts[current_worker] > maxShifts) {
        return false;
    }

    return scheduleConformsToMaxShifts(
        avail,
        dailyNeed,
        maxShifts,
        sched,
        to_day,
        to_shift,
        workersPerDay,
        worker_shifts,
        current_day,
        current_shift + 1
        );
}

void createAndFillSchedule(
    const AvailabilityMatrix& avail,
    const size_t daily_need,
    DailySchedule& sched,
    size_t current_day,
    size_t current_shift
) {

    if (current_day >= avail.size()) {
        return;
    }

    if (current_shift >= daily_need) {
        current_day += 1;
        current_shift = 0;

        if (current_day >= avail.size()) {
            return;
        }

        sched.emplace_back();
    }else if (current_day == 0 && current_shift == 0) {
        sched.emplace_back();
    }

    sched[current_day].push_back(INVALID_ID);

    createAndFillSchedule(avail, daily_need, sched, current_day, current_shift + 1);

}



