//
// Created by jinjianglun on 2017/5/22.
//

#ifndef THREADPOOL_MYJOB_H
#define THREADPOOL_MYJOB_H

#include "CJob.h"


class MyJob:CJob{
public:
    void Run ( void *ptr );
};


#endif //THREADPOOL_MYJOB_H
