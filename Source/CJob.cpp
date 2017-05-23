//
// Created by jinjianglun on 2017/5/22.
//

#include <stdlib.h>
#include "CJob.h"


CJob::CJob():m_pWorkThread(NULL),m_JobNo(0),m_JobName(NULL)
{}

CJob::~CJob()
{
    if(NULL != m_JobName)
        free(m_JobName);
}

void CJob::SetJobName(char* jobname)
{
    if(NULL !=m_JobName)
    {
        free(m_JobName);
        m_JobName = NULL;
    }

    if(NULL !=jobname)
    {
        m_JobName = (char*)malloc(strlen(jobname)+1);
        strcpy(m_JobName,jobname);
    }
}