#include "test.h"

using namespace std;




test::test()
{
}



void test::runHelper2() {
    cout <<  "step1" << endl;
    cout << "step2"  << endl;
    sleep(3);

}
void* runHelper1(void* arg) {
    test* ts =  (test*) arg;
    ts->runHelper2();
}

void test::run() {
    belong->createJob(1,1,pool,runHelper1,this);
}


