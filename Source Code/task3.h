#ifndef TASK3_H
#define TASK3_H

#include "unistd.h"
#include "header.h"
#include "calculator.h"

class task3: public systemProcess
{
public:
    task3(Memory* m , calculator* window);
    ~task3();
    void run();
    string programintroduction;
    void terminate();
    calculator* window3;
};

void* runHelperT3(void* arg);

#endif // TASK3_H
