//
// Created by jinjianglun on 2017/5/22.
//

#ifndef THREADPOOL_CTHREAD_H
#define THREADPOOL_CTHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
typedef enum{
    Aborted,
    AbortRequested,
    Running,
    Stopped,
    StopRequested,
    Suspended,
    SuspendRequested,
    Unstarted,
    WaitSleepJoin
}ThreadState;

typedef enum{
    Lowest      = 15,
    BelowNormal = 23,
    Normal      = 31,
    AboveNormal = 39,
    Highest     = 47
}ThreadPriority;

#ifdef  APPLE
#include <semaphore.h>
#else
typedef struct {
    int value, wakeups;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Semaphore;

Semaphore *make_semaphore(int value);
void semaphore_wait(Semaphore *semaphore);
void semaphore_signal(Semaphore *semaphore);
void semaphore_destroy(Semaphore *semaphore);
#endif



class CThread
{
private:
    int m_ErrCode;
#ifdef APPLE
    sem_t m_ThreadSemaphore;
#else
    Semaphore m_ThreadSemaphore;    //the inner semaphore, which is used to realize
#endif

    pthread_t m_ThreadID;
    bool m_Detach;                  //线程分离
    bool m_CreateSuspended;         //创建之后即挂起
    char* m_ThreadName;             //线程名称
    ThreadState m_ThreadState;      //线程状态

protected:
    //设置错误码
    void SetErrcode(int errcode)
    {
        m_ErrCode = errcode;
    }

    static void* ThreadFunction(void* args);

public:
    CThread();
    CThread(bool createSuspended, bool detach);

    virtual ~CThread();
    //定义接口
    virtual void Run(void) = 0;

    //设置或获取线程的状态
    void SetThreadState(ThreadState state)
    {
        m_ThreadState = state;
    }

    ThreadState GetThreadState(void)
    {
        return m_ThreadState;
    }

    //终止线程
    int Terminate();
    //执行线程
    bool Start();
    void Exit();
    void Wakeup();

    int GetLastError(void)
    {
        return m_ErrCode;
    }

    //获取或设置线程名称
    void SetThreadName(char* thrname)
    {
        strcpy(m_ThreadName,thrname);
    }

    char* GetThreadName()
    {
        return m_ThreadName;
    }

    //获取线程ID
    pthread_t GetThreadID()
    {
        return m_ThreadID;
    }

    //获取或设置优先级
    int SetPriority(int priority);
    int  GetPriority();
    int  GetConcurrency();
    void SetConcurrency(int num);
    int Detach();
    int Join();
    void Yield(void);
    pthread_t  Self(void);
};


#endif //THREADPOOL_CTHREAD_H
