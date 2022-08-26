#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <thread>
#include <sstream> 
#include <chrono> 
#include <ctime>
#include <mutex>

using namespace std;

int ACCOUNT = 0;
bool stop = false;
bool changed = false;

mutex accountMutex;
mutex stopMutex;
mutex changedMutex;

void incThreadFunction() {

    srand(time(0));

    while (!stop) {

        int pause = 100 + rand() % 3001;
        int increment = 1 + rand() % 101;

        accountMutex.lock();
        ACCOUNT += increment;
        accountMutex.unlock();

        changedMutex.lock();
        changed = true;
        changedMutex.unlock();


        this_thread::sleep_for(chrono::milliseconds(pause));
    }
}

void decThreadFunction() {

    while (!stop) {

        int pause = 100 + rand() % 3001;
        int decrement = 1 + rand() % 101;

        accountMutex.lock();
        ACCOUNT -= decrement;
        accountMutex.unlock();

        changedMutex.lock();
        changed = true;
        changedMutex.unlock();

        this_thread::sleep_for(chrono::milliseconds(pause));
    }
}

void printThreadFunction() {

    while (!stop) {

        if (changed) {

            cout << ACCOUNT << endl;

            changedMutex.lock();
            changed = false;
            changedMutex.unlock();
        }

        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

int main()
{
    thread inc = thread(incThreadFunction);
    thread dec = thread(decThreadFunction);
    thread print = thread(printThreadFunction);

    while (true){

        if (ACCOUNT < -10000 || ACCOUNT > 10000) {

            stopMutex.lock();
            stop = true;
            stopMutex.unlock();

            break;
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    system("pause");
    return 0;
}
