//
// Created by jinjianglun on 2017/5/22.
//

#ifndef THREADPOOL_CWORKERTHREAD_H
#define THREADPOOL_CWORKERTHREAD_H

#include <thread>
#include <condition_variable>

#include <stdio.h>
#include "CThread.h"
#include "CJob.h"
#include "CThreadMutex.h"
#include "CCondition.h"
#include "CThreadPool.h"

class CWorkerThread: public CThread {
private:
    CThreadPool * m_ThreadPool;
    CJob *m_Job;/*任务*/
    void *m_JobData;/*任务数据*/
    CThreadMutex m_VarMutex;
    bool m_IsEnd;
    bool m_IsRunning;

public:

    CWorkerThread();
    virtual ~CWorkerThread();

    std::condition_variable_any m_JobCond;
    std::mutex m_WorkMutex;


    bool Start();

    //线程运行
    void Run(void);

    //设置或获取任务
    void SetJob(CJob* job,void* jobdata);
    CJob* GetJob()
    {
        return m_Job;
    }

    //设置或获取线程所在的线程池
    void SetThreadPool(CThreadPool * thrpool);

    inline CThreadPool * GetThreadPool(){return m_ThreadPool;}
};


#endif //THREADPOOL_CWORKERTHREAD_H
