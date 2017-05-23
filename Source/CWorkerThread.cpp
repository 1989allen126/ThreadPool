//
// Created by jinjianglun on 2017/5/22.
//

#include <iostream>
#include<unistd.h>
#include "CWorkerThread.h"

using namespace std;


CWorkerThread::CWorkerThread()
{
    m_Job = NULL;
    m_JobData = NULL;
    m_ThreadPool = NULL;
    m_IsEnd = false;

    m_IsRunning = false;
}

CWorkerThread::~CWorkerThread()
{
    if(NULL != m_Job)
        delete m_Job;
    m_Job = NULL;

    if(NULL != m_ThreadPool)
        delete m_ThreadPool;
    m_ThreadPool = NULL;
    m_IsRunning = false;
}

bool CWorkerThread::Start() {
    if(m_IsRunning) return m_IsRunning;

    m_IsRunning = true;
    auto t = std::thread(std::bind(&CWorkerThread::Run,this),NULL);
    t.detach();
    return m_IsRunning;
}

/**
 * 运行线程
 */
void CWorkerThread::Run(void)
{
    SetThreadState(Running);
    for(;;)
    {
        std::lock_guard<std::mutex> lock(m_WorkMutex);
        while (m_Job == NULL)
        {
            m_JobCond.wait(m_WorkMutex);
        }

        //执行具体任务
        m_Job->Run(m_JobData);

        //任务执行完毕后的处理
        m_Job->SetWorkThread(NULL);
        m_Job = NULL;
        m_ThreadPool->MoveToIdleList(this);

        //如果线程池中的线程数量过多，则删除一定数量的空闲线程
        if(m_ThreadPool->m_IdleList.size() > m_ThreadPool->GetAvailHighNum())
        {
            m_ThreadPool->DeleteIdleThread(m_ThreadPool->m_IdleList.size()-m_ThreadPool->GetInitNum());
        }
    }
}

//设置要执行的任务
void CWorkerThread::SetJob(CJob* job, void* jobArgs)
{
    m_VarMutex.Lock();
    m_Job = job;
    m_JobData = jobArgs;
    job->SetWorkThread(this);
    m_VarMutex.Unlock();
    m_JobCond.notify_one();
}

//设置线程所在的线程池
void CWorkerThread::SetThreadPool(CThreadPool* thrpool)
{
    m_VarMutex.Lock();
    m_ThreadPool = thrpool;
    m_VarMutex.Unlock();
}