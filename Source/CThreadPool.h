//
// Created by jinjianglun on 2017/5/22.
//

#ifndef THREADPOOL_CTHREADPOOL_H
#define THREADPOOL_CTHREADPOOL_H

#include <stdio.h>
#include<vector>

#include "CJob.h"
#include "CThreadMutex.h"
#include "CCondition.h"

class CWorkerThread;

class CThreadPool{
public:
    CThreadPool();
    CThreadPool(int initnum);

    virtual ~CThreadPool();

    //访问忙碌队列的互斥锁
    CThreadMutex m_BusyMutex;
    //访问空闲队列的互斥锁
    CThreadMutex m_IdleMutex;
    //访问工作队列
    CThreadMutex m_JobMutex;
    CThreadMutex m_VarMutex;

    //访问忙碌队列的条件变量
    CCondition m_BusyCond;
    //访问空闲队列的条件变量
    CCondition m_IdleCond;

    //同步工作队列
    CCondition m_IdleJobCond;
    CCondition m_MaxNumCond;

    std::vector<CWorkerThread*>   m_ThreadList;
    //忙碌队列
    std::vector<CWorkerThread*>   m_BusyList;
    //空闲队列
    std::vector<CWorkerThread*>   m_IdleList;

    //设置或获取线程池允许存在的最大的线程数
    void SetMaxNum(int maxNum)
    {
        m_MaxNum = maxNum;
    }
    int GetMaxNum(void)
    {
        return m_MaxNum;
    }

    //设置或获取最小需要有多少个线程可用，如果数量太少，则创建
    void SetAvailLowNum(int minNum)
    {
        m_AvailLow = minNum;
    }
    int GetAvailLowNum(void)
    {
        return m_AvailLow;
    }

    //设置或获取最多需要有多少个线程可用，如果太多，则删除
    void SetAvailHighNum(int highNum)
    {
        m_AvailHigh = highNum;
    }
    int GetAvailHighNum()
    {
        return m_AvailHigh;
    }

    //获得实际可用的线程数量（处于空闲状态）
    int GetActualAvailNum()
    {
        return m_AvailNum;
    }
    //获得所有线程的数量
    int GetAllNum()
    {
        return m_ThreadList.size();
    }
    //获得运行状态的线程数量
    int GetBusyNum()
    {
        return m_BusyList.size();
    }

    //设置或获取初始时线程的数量
    void SetInitNum(int initnum)
    {
        m_InitNum = initnum;
    }
    int GetInitNum()
    {
        return m_InitNum;
    }

    //终止所有线程
    void TerminateAll();
    //执行任务
    void Run(CJob* job,void* jobdata);

    friend class CWorkerThread;

private:

    //允许创建的最大线程数量
    unsigned int m_MaxNum;
    //允许的最少空闲线程数量
    unsigned int m_AvailLow;
    //允许的最大空闲线程数量
    unsigned int m_AvailHigh;
    //现有的空闲线程数量
    unsigned int m_AvailNum;
    //初始时的线程数量
    unsigned int m_InitNum;  //Normal thread num;

protected:
    CWorkerThread* GetIdleThread(void);

    //增加工作线程到空闲队列
    void AppendToIdleList(CWorkerThread* jobthread);
    //移动一个空闲线程到忙碌队列
    void MoveToBusyList(CWorkerThread* idlethread);
    //移动一个忙碌线程到空闲队列
    void MoveToIdleList(CWorkerThread* busythread);
    //删除空闲线程
    void DeleteIdleThread(int num);
    //创建空闲线程
    void CreateIdleThread(int num);
};


#endif //THREADPOOL_CTHREADPOOL_H
