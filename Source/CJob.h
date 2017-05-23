//
// Created by jinjianglun on 2017/5/22.
//

#ifndef THREADPOOL_CJOB_H
#define THREADPOOL_CJOB_H

#include <stdio.h>
#include "CThread.h"


class CJob
{
private:
    int m_JobNo;                    //任务编号
    char * m_JobName;               //任务名称
    CThread * m_pWorkThread;        //The thread associated with the job

public:
    CJob();
    virtual ~CJob();

    //设置或获取任务编号
    int GetJobNo(void)
    {
        return m_JobNo;
    }

    void SetJobNo(int jobno)
    {
        m_JobNo = jobno;
    }

    //设置或获取任务名称
    char* GetJobName(void) const
    {
        return m_JobName;
    }
    void SetJobName(char* jobname);

    //获取或设置工作线程
    CThread * GetWorkThread()
    {
        return m_pWorkThread;
    }
    void SetWorkThread(CThread * pWorkThread)
    {
        m_pWorkThread = pWorkThread;
    }

    //指定Run接口
    virtual void Run ( void *ptr ) = 0;
};

#endif //THREADPOOL_CJOB_H
