//
// Created by jinjianglun on 2017/5/22.
//

#ifndef THREADPOOL_CTHREADMUTEX_H
#define THREADPOOL_CTHREADMUTEX_H

#include <stdio.h>
#include <pthread.h>

class CThreadMutex {
private:
    static pthread_mutex_t m_pthreadMutex;    /** 线程同步锁 */
public:
    CThreadMutex();
    ~CThreadMutex();
    const pthread_mutex_t getLock();
    void Lock();
    void Unlock();
};

#endif //THREADPOOL_CTHREADMUTEX_H
