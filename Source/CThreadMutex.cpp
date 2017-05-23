//
// Created by jinjianglun on 2017/5/22.
//

#include "CThreadMutex.h"

pthread_mutex_t CThreadMutex::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;    /** 线程同步锁 */

/**
 * 创建线程
 */
CThreadMutex::CThreadMutex(){
    pthread_mutex_init(&m_pthreadMutex, NULL);
}

/**
 * 析构函数
 */
CThreadMutex::~CThreadMutex()
{
    pthread_mutex_destroy(&m_pthreadMutex);
}

const pthread_mutex_t  CThreadMutex::getLock()
{
    return m_pthreadMutex;
}

/**
 * 加锁
 */
void CThreadMutex::Lock()
{
    pthread_mutex_lock(&m_pthreadMutex);
}

/**
 * 解锁
 */
void CThreadMutex::Unlock(){
    pthread_mutex_unlock(&m_pthreadMutex);
}
