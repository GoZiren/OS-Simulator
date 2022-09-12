#include"header.h"

Memory::Memory(){
    void *place = (void*)malloc(1000);
    memoryTable.data = new Frame[3 * 20]; //容量是3S，但是S之后的不去初始化。方便后面生成碎片不用对array扩容
    memoryTable.size = 20;
    memoryTable.data[0].ini_pos = 0;
    memoryTable.data[0].addr = (char*)place;
    memoryTable.data[0].size = 10;
    memoryTable.data[0].occupy = false;
    memoryTable.data[0].process = NULL;

    //TODO: os占用一块内存
    for (int i = 1; i < memoryTable.size; ++i){
            memoryTable.data[i].ini_pos = i;
            //std::cout<< memoryTable.data[i].addr;
            memoryTable.data[i].addr = memoryTable.data[i-1].addr + memoryTable.data[i-1].size;
            memoryTable.data[i].size = memoryTable.data[i-1].size + 30;
            memoryTable.data[i].occupy = false;
        }
    }

Memory::~Memory(){
    delete[] memoryTable.data;
}

int Memory::findSpace(int job_length){
    int pos = -1;
    for (int i = 0; i < memoryTable.size; ++i){
        if (!memoryTable.data[i].occupy && memoryTable.data[i].size >= job_length) {
            pos = i;
            break;
        }
    }
    return pos;
}

void Memory::requestMemory(Process *process, job *job, int job_length) {
    int pos = findSpace(job_length);
    if (pos == -1){cout << "no space available"; return;}

    if(memoryTable.data[pos].size > job_length){
        //裁剪原先的内存块，一部分满足需求，另一部分成为碎片
        memoryTable.size++;
        //第一部分满足需求
        int original_size = memoryTable.data[pos].size;
        memoryTable.data[pos].size = job_length;
        memoryTable.data[pos].occupy = true;
        memoryTable.data[pos].process = process;
        memoryTable.data[pos].job = job;
        //生成碎片
        Frame tmp, tmp2;

        std::cout<< memoryTable.data[pos + 1].addr;
        if (pos < memoryTable.size - 1) {tmp = memoryTable.data[pos+1];}
        memoryTable.data[pos + 1].addr = memoryTable.data[pos].addr + job_length;
        memoryTable.data[pos + 1].size = original_size - job_length;
        memoryTable.data[pos + 1].ini_pos = memoryTable.data[pos].ini_pos;
        //剩下所有的frame往后推
        for (int j = pos + 2; j < memoryTable.size; j++){
            tmp2 = memoryTable.data[j]; //当前位置
            memoryTable.data[j] = tmp; //上一个位置
            tmp = tmp2; //当前位置，下一步的前位置
        }
    }else{
        //分区大小恰好等于需求
        memoryTable.data[pos].occupy = true;
        memoryTable.data[pos].process = process;
        memoryTable.data[pos].job = job; //
    }
//    cout<<memoryTable.data[1].job<<endl;
    //showMemory();
    return;
}

void Memory::releaseMemory(Process *process){
    std::vector<struct job*> jobList = process -> jobList;
    for (unsigned int i = 0; i < jobList.size(); i++){

        releaseMemory(process, jobList[i]);
    }
    return;
}

void Memory::releaseMemory(Process * process, job * job){
    int pos = -1;
    for (int i = 0; i < memoryTable.size; i++){
        if ((memoryTable.data[i].process == process) & (memoryTable.data[i].job == job)) pos = i;
    }

    if (pos == -1) {cout<<"releaseMemory: process not found" << endl; return;}

    memoryTable.data[pos].occupy = false;
    memoryTable.data[pos].process = NULL;
    memoryTable.data[pos].job = NULL;
    //检查下一个frame是否源于同一个初始frame
    if (pos < memoryTable.size - 1){
        if ((memoryTable.data[pos].ini_pos == memoryTable.data[pos+1].ini_pos) & (memoryTable.data[pos+1].occupy==0)){
            mergeWithNext(pos);
        }
    }

    if ((memoryTable.data[pos].ini_pos == memoryTable.data[pos-1].ini_pos) & (memoryTable.data[pos-1].occupy==0)){
        mergeWithNext(pos-1);
    }

    //showMemory();
    return;
}

void Memory::mergeWithNext(int pos){
    memoryTable.data[pos].size = memoryTable.data[pos].size + memoryTable.data[pos+1].size;
    for (int i = pos + 1; i < memoryTable.size - 1; i++){
        memoryTable.data[i] = memoryTable.data[i+1];
    }
    memoryTable.size--;
    return;
}


void Memory::showMemory(){
    std::cout << "---------------------------------"<<std::endl;
    std::cout<<"ini_pos|starting address|size|occupy"<<std::endl;
    for (int i = 0; i < memoryTable.size; ++i){
        printf("%-4d%-15p%-5d%-2d\n",memoryTable.data[i].ini_pos,memoryTable.data[i].addr, memoryTable.data[i].size, memoryTable.data[i].occupy);
    }
    std::cout<<"-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-"<<std::endl;
}


QString Memory::getTable(){
    QString v;
    for (int i = 0; i < memoryTable.size; i++){
        QString pos =  QString::number(memoryTable.data[i].ini_pos);
        QString add = QString::fromStdString(std::to_string((uintptr_t) memoryTable.data[i].addr));
        QString siz =  QString::number(memoryTable.data[i].size);
        QString occ;
        if (memoryTable.data[i].occupy == 1){occ = "1";}
        else {occ = "0";}
        v = v + "," + pos + "," + add + "," + siz + "," + occ;
    }
    return v;
}
