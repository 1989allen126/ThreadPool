//
// Created by jinjianglun on 2017/5/22.
//

#ifndef THREADPOOL_CTHREADMANAGE_H
#define THREADPOOL_CTHREADMANAGE_H


#include <stdio.h>
#include "CThreadPool.h"
#include "CJob.h"

class CThreadManage
{
private:
    CThreadPool* m_Pool;
    int m_NumOfThread;
protected:
public:
    void SetParallelNum(int num);
    CThreadManage();
    CThreadManage(int num);
    virtual ~CThreadManage();
    void Run(CJob* job,void* jobdata);
    void TerminateAll(void);
};

#endif //THREADPOOL_CTHREADMANAGE_H
