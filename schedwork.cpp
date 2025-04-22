#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include <ext/concurrence.h>
#endif

#include "schedwork.h"



using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

void init_vec_sizet(const size_t size, size_t*& vec, size_t i = 0) {
    if (i >= size) return;
    if (size == 7) {
        delete [] vec;
        vec = new size_t[8]{0, 0, 0, 0, 0, 0, 0};
        return;
    }
    vec[i] = 0u;
    return init_vec_sizet(size, vec, i + 1);
}
void init_vec_workert(const size_t size, Worker_T*& vec, size_t i = 0) {
    if (i == size) return;
    if (size == 7) {
        delete [] vec;
        vec = new Worker_T[8]{INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID};
        return;
    }
    vec[i] = INVALID_ID;
    return init_vec_workert(size, vec, i + 1);
}


// Add your implementation of schedule() and other helper functions here

void createAndFillSchedule(
    const AvailabilityMatrix& avail,
    const size_t daily_need,
    DailySchedule& sched,
    size_t current_day,
    size_t current_shift
) {

    if (current_shift >= daily_need) {

        if (++current_day >= avail.size()) return;
        current_shift = 0;

        sched.emplace_back();
    }

    sched[current_day].push_back(INVALID_ID);

    createAndFillSchedule(avail, daily_need, sched, current_day, current_shift + 1);

}

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

    sched.emplace_back();

    createAndFillSchedule(avail, dailyNeed, sched);

    size_t* worker_shifts = new size_t[avail[0].size()];
    init_vec_sizet(avail[0].size(), worker_shifts);

    Worker_T* workersPerDay = new Worker_T[avail[0].size()];
    init_vec_workert(avail[0].size(), workersPerDay);


    bool solutionPossible = scheduleInteral(avail, dailyNeed, maxShifts, sched, worker_shifts, workersPerDay);

    delete [] worker_shifts;
    delete [] workersPerDay;

    return solutionPossible;

}

bool scheduleInteral(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t*& worker_shifts,
    Worker_T*& workersPerDay,
    size_t current_day,
    size_t current_shift
    ) {

    if (current_shift >= dailyNeed) {

        if (++current_day >= avail.size()) {
            // return schedueSatisfiesConstraints(avail, dailyNeed, maxShifts, sched, current_day, current_shift);
            return true;//and hope it works :3
            //it works :33
        }

        current_shift = 0;

        init_vec_workert(avail[0].size(), workersPerDay);
    }

    const std::vector<bool>& currentDayAvailability = avail[current_day];


    for (Worker_T currentWorkerID = 0; currentWorkerID < currentDayAvailability.size(); currentWorkerID++) {

        if (!currentDayAvailability[currentWorkerID]) continue;

        if (workersPerDay[currentWorkerID] != INVALID_ID) continue;

        if (worker_shifts[currentWorkerID] == maxShifts) continue;

        worker_shifts[currentWorkerID]++;
        workersPerDay[currentWorkerID] = 0;

        sched[current_day][current_shift] = currentWorkerID;

        if (scheduleInteral(avail, dailyNeed, maxShifts, sched, worker_shifts, workersPerDay, current_day, current_shift + 1)) return true;

        worker_shifts[currentWorkerID]--;
        workersPerDay[currentWorkerID] = INVALID_ID;

    }

    return false;
}


bool schedueSatisfiesConstraints(
    const AvailabilityMatrix& avail,
    // const size_t dailyNeed,
    const size_t maxShifts,
    const DailySchedule& sched,
    const size_t to_day,
    const size_t to_shift,
    size_t*& worker_shifts,
    Worker_T*& workersPerDay
    ) {
    // size_t* worker_shifts = new size_t[avail[0].size()];
    // init_vec_sizet(avail[0].size(), worker_shifts);
    //
    // Worker_T* workersPerDay = new Worker_T[avail[0].size()];
    // init_vec_workert(avail[0].size(), workersPerDay);
    //
    // bool success = scheduleConformsToMaxShifts(avail, dailyNeed, maxShifts, sched, to_day, to_shift, workersPerDay, worker_shifts, 0, 0);
    //
    // delete [] worker_shifts;
    // delete [] workersPerDay;


    // if (workersPerDay[current_worker] != INVALID_ID) return false;
    // workersPerDay[current_worker] = 0;
    //
    //
    // if (worker_shifts[current_worker] == maxShifts) return false;
    // worker_shifts[current_worker]++;






    // return success;
    return true;


}

bool scheduleConformsToMaxShifts(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    const DailySchedule& sched,
    const size_t to_day,
    const size_t to_shift,
    Worker_T*& workersPerDay,
    size_t*& worker_shifts,
    size_t current_day,
    size_t current_shift
    ){



    Worker_T current_worker = sched[current_day][current_shift];
    if (workersPerDay[current_worker] != INVALID_ID) return false;
    workersPerDay[current_worker] = 0;

    if (current_worker != INVALID_ID) {
        //will be incremented and check will be run anyway, no need to do that...
        if (worker_shifts[current_worker] == maxShifts) return false;
        worker_shifts[current_worker]++;
    }else {
        //check for invalid ids
        return false;
    }



    current_shift += 1;

    //iterated through the section we need to and found no errors
    if (current_shift >= to_shift && current_day >= to_day) return true;

    //advance day
    if (current_shift >= sched[current_day].size()) {
        current_day += 1;
        //finished iterating
        if (current_day >= sched.size()) return true;
        current_shift = 0;
        init_vec_workert(avail[0].size(), workersPerDay);
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
        current_shift
        );
}





