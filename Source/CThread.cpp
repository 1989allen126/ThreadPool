//
// Created by jinjianglun on 2017/5/22.
//

#include "CThread.h"

#include <pthread.h>
#include <iostream>

#ifdef APPLE
#else
Semaphore *make_semaphore(int value)
{
    Semaphore *semaphore = (Semaphore*)malloc(sizeof(Semaphore));
    semaphore->value = value;
    semaphore->wakeups = 0;
    pthread_mutex_init(&semaphore->mutex, NULL);
    pthread_cond_init(&semaphore->cond, NULL);
    return semaphore;
}


void semaphore_wait(Semaphore *semaphore)
{
    pthread_mutex_lock(&semaphore->mutex);
    semaphore->value --;

    if (semaphore->value < 0) {
        do {
            pthread_cond_wait(&semaphore->cond, &semaphore->mutex);
        } while (semaphore->wakeups < 1);
        semaphore->wakeups--;
    }
    pthread_mutex_unlock(&semaphore->mutex);
}

void semaphore_signal(Semaphore *semaphore)
{
    pthread_mutex_lock(&semaphore->mutex);
    semaphore->value ++;

    if (semaphore->value <= 0) {
        semaphore->wakeups++;
        pthread_cond_signal(&semaphore->cond);
    }

    pthread_mutex_unlock(&semaphore->mutex);
}

void semaphore_destroy(Semaphore *semaphore)
{
    if (semaphore == nullptr) return ;

    pthread_mutex_destroy(&semaphore->mutex);
    pthread_cond_destroy(&semaphore->cond);
}
#endif

void* CThread::ThreadFunction(void* args)
{
    return NULL;
}

CThread::CThread()
{
#ifdef APPLE
    sem_unlink("m_ThreadSemaphore");
    m_ThreadSemaphore = sem_open("m_ThreadSemaphore", O_CREAT, 0, 1);
#else
    m_ThreadSemaphore = *make_semaphore(1);
#endif
    pthread_create(&m_ThreadID, NULL, ThreadFunction, NULL);
}

CThread::~CThread()
{
#ifdef APPLE
    sem_close(&m_ThreadSemaphore);
#else
    semaphore_destroy(&m_ThreadSemaphore);
#endif

}

CThread::CThread(bool createSuspended, bool detach){
    m_CreateSuspended = createSuspended;
    m_Detach = detach;
    if(detach)
    {
        Detach();
    }
}

int CThread::Terminate()
{
    return pthread_kill(m_ThreadID, 0);
}

bool CThread::Start(){
    return true;
}

void CThread::Exit(){
    pthread_exit(NULL);
}

void CThread::Wakeup(){
#ifdef APPLE
    sem_post(&m_ThreadSemaphore);
#else
    semaphore_signal(&m_ThreadSemaphore);
#endif
}

int CThread::SetPriority(int priority){
    struct sched_param    param;
    param.sched_priority =  priority;
    int                   policy = SCHED_OTHER;
    return pthread_setschedparam(m_ThreadID, policy, &param);
}

int  CThread::GetPriority(){
    struct sched_param   param;
    int                  policy;

    pthread_getschedparam(m_ThreadID, &policy, &param);
    return param.sched_priority;
}

int  CThread::GetConcurrency(){
    return pthread_getconcurrency();
}

void CThread::SetConcurrency(int num){
    pthread_setconcurrency(num);
}

int CThread::Detach(){
    return pthread_detach(m_ThreadID);
}

int CThread::Join(){
    return pthread_join(m_ThreadID, NULL);
}

void CThread::Yield(void){
    pthread_yield_np();
}

pthread_t  CThread::Self(void){
    return pthread_self();
}