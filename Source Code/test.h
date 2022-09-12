#ifndef TEST_H
#define TEST_H


#include "unistd.h"
#include "header.h"

class test
{
public:
    test();
    void runHelper2();
    void run();
    Process *belong;
    threadpool *pool;
};

void* runHelper1(void* arg);
#endif // TEST_H
