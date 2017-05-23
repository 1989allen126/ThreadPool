//
// Created by jinjianglun on 2017/5/22.
//

#ifndef THREADPOOL_CCONDITION_H
#define THREADPOOL_CCONDITION_H

#include <stdio.h>
#include <pthread.h>

class CCondition{
private:
    static pthread_cond_t m_pthreadCond;      /** 线程同步的条件变量 */
public:
    CCondition();
    ~CCondition();
    void Signal();
    void Wait(pthread_mutex_t mutex);
    void Broadcast();
    void Destroy();
};

#endif //THREADPOOL_CCONDITION_H
