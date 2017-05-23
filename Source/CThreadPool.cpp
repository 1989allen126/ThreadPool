//
// Created by jinjianglun on 2017/5/22.
//

#include "CThreadPool.h"

#include <assert.h>
#include <iostream>
#include "CWorkerThread.h"

CThreadPool::CThreadPool()
{
    m_MaxNum = 50;
    m_AvailLow = 5;
    m_InitNum=m_AvailNum = 10 ;
    m_AvailHigh = 20;

    m_BusyList.clear();
    m_IdleList.clear();

    //创建初始数量的工作线程
    for(int i=0; i<m_InitNum; i++)
    {
        CWorkerThread* workThread = new CWorkerThread();
        workThread->SetThreadPool(this);
        //添加到空闲队列
        AppendToIdleList(workThread);
        workThread->Start();
    }
}

CThreadPool::CThreadPool(int initnum)
{
    //用宏进行判断
    assert(initnum>0 && initnum<=30);
    m_MaxNum = 30;
    m_AvailLow = initnum-10>0?initnum-10:3;
    m_InitNum=m_AvailNum = initnum ;
    m_AvailHigh = initnum+10;

    m_BusyList.clear();
    m_IdleList.clear();

    //创建初始数量的工作线程
    for(int i=0;i<m_InitNum;i++)
    {
        CWorkerThread* workThread = new CWorkerThread();
        //添加到空闲队列中
        AppendToIdleList(workThread);
        workThread->SetThreadPool(this);
        workThread->Start();       //begin the thread,the thread wait for job
    }
}

CThreadPool::~CThreadPool()
{
    TerminateAll();
}

//终止所有线程
void CThreadPool::TerminateAll()
{
    //唤醒所有等待的线程
    m_IdleCond.Broadcast();

    for(int i=0; i< m_ThreadList.size(); i++)
    {
        CWorkerThread* workThread = m_ThreadList[i];
        //等待线程终止
        workThread->Join();
    }
}

//获得空闲线程
CWorkerThread* CThreadPool::GetIdleThread(void)
{
    //判断条件，并获取所
    while(m_IdleList.size() ==0)
        m_IdleCond.Wait(m_IdleMutex.getLock());
    m_IdleMutex.Lock();

    if(m_IdleList.size() > 0)
    {
        //返回空闲队列首部的成员
        CWorkerThread * workThread = (CWorkerThread*) m_IdleList.front();
//        printf("Get Idle thread %d/n",workThread->GetThreadID());
        m_IdleMutex.Unlock();
        return workThread;
    }

    //获取失败
    m_IdleMutex.Unlock();
    return NULL;
}

//增加一个工作线程到空闲队列
void CThreadPool::AppendToIdleList(CWorkerThread* jobthread)
{
    m_IdleMutex.Lock();
    //在所有的线程队列和空闲队列尾部增加一个线程
    m_ThreadList.push_back(jobthread);
    m_IdleList.push_back(jobthread);

    m_IdleMutex.Unlock();
}

//把空闲线程移动到忙碌队列中
void CThreadPool::MoveToBusyList(CWorkerThread* idlethread)
{
    //在忙碌队列尾部添加线程
    m_BusyMutex.Lock();
    m_BusyList.push_back(idlethread);
    m_AvailNum--;
    m_BusyMutex.Unlock();

    //在空闲队列中寻找线程，并删除
    m_IdleMutex.Lock();
    std::vector<CWorkerThread*>::iterator pos;
    pos = find(m_IdleList.begin(), m_IdleList.end(), idlethread);
    if(pos !=m_IdleList.end())
        m_IdleList.erase(pos);
    m_IdleMutex.Unlock();
}

//把忙碌线程移动到空闲队列中
void CThreadPool::MoveToIdleList(CWorkerThread* busythread)
{
    //在空闲队列尾部添加线程
    m_IdleMutex.Lock();
    m_IdleList.push_back(busythread);
    m_AvailNum++;
    m_IdleMutex.Unlock();

    //在忙碌队列中寻找线程，并删除
    m_BusyMutex.Lock();
    std::vector<CWorkerThread*>::iterator pos;
    pos = find(m_BusyList.begin(),m_BusyList.end(),busythread);
    if(pos!=m_BusyList.end())
        m_BusyList.erase(pos);
    m_BusyMutex.Unlock();

    m_IdleCond.Signal();
    m_MaxNumCond.Signal();
}

//创建指定数量的线程，并添加到空闲队列中
void CThreadPool::CreateIdleThread(int num)
{
    for(int i=0; i<num; i++)
    {
        CWorkerThread* newThread = new CWorkerThread();
        newThread->SetThreadPool(this);
        //把线程加入到空闲队列中
        AppendToIdleList(newThread);

        m_VarMutex.Lock();
        m_AvailNum++;
        m_VarMutex.Unlock();

        newThread->Start();       //begin the thread,the thread wait for job
    }
}

//删除指定数量的线程
void CThreadPool::DeleteIdleThread(int num)
{
    printf("Enter into CThreadPool::DeleteIdleThread/n");
    printf("Delete Num is %d/n", num);

    m_IdleMutex.Lock();
    for(int i=0; i<num; i++)
    {
        CWorkerThread* removeThread;
        if( m_IdleList.size() > 0 )
        {
            removeThread = (CWorkerThread*) m_IdleList.front();
//            printf("Get Idle thread %d/n",removeThread->GetThreadID());
        }

        //在空闲队列中查找，并删除
        std::vector<CWorkerThread*>::iterator pos;
        pos = find(m_IdleList.begin(),m_IdleList.end(), removeThread);
        if(pos!=m_IdleList.end())
            m_IdleList.erase(pos);

        m_AvailNum--;

        printf("The idle thread available num:%d /n", m_AvailNum);
        printf("The idlelist num:%d /n", m_IdleList.size());
    }

    m_IdleMutex.Unlock();
}


//找出空闲线程，以执行任务
void CThreadPool::Run(CJob* job,void* jobdata)
{
    assert(job!=NULL);
    //if the busy thread num adds to m_MaxNum,so we should wait

    if(GetBusyNum() == m_MaxNum)
        m_MaxNumCond.Wait(m_VarMutex.getLock());

    if(m_IdleList.size())
    {
        if(GetAllNum()+m_InitNum-m_IdleList.size() < m_MaxNum )
            CreateIdleThread(m_InitNum-m_IdleList.size());
        else
            CreateIdleThread(m_MaxNum-GetAllNum());
    }

    CWorkerThread*  idlethr = GetIdleThread();
    if(idlethr != NULL)
    {
        MoveToBusyList(idlethr);
        idlethr->SetThreadPool(this);
        job->SetWorkThread((CThread*)idlethr);
        std::cout<<"Job is set to thread"<<(idlethr->GetThreadID())<<std::endl;
        idlethr->SetJob(job,jobdata);
    }
}