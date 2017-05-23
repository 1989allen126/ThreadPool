#include <iostream>
#include <stdio.h>
#include<pthread.h>
#include <thread>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>

#include "CThreadMutex.h"
#include "CCondition.h"
#include "CWorkerThread.h"

#include "MyJob.h"
#import "CThreadManage.h"

CCondition *pCond;
CThreadMutex mutex;
static CThreadManage manage;




int main() {
    MyJob job;
    manage.Run((CJob*)&job, NULL);
    while (1);
    return 0;
}
