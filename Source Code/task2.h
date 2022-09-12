#ifndef TASK2_H
#define TASK2_H

#include "unistd.h"
#include "header.h"

class task2 : public systemProcess
{
public:
    task2(Memory* m);
    ~task2();
    int runhelper();
    void run();
    string programintroduction;
    void terminate();
};

void* runHelper(void* arg);

#endif // TASK2_H
