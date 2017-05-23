//
// Created by jinjianglun on 2017/5/22.
// 条件变量

#include "CCondition.h"

pthread_cond_t CCondition::m_pthreadCond = PTHREAD_COND_INITIALIZER;

/**
 * 构造函数
 */
CCondition::CCondition(){
    pthread_cond_init(&m_pthreadCond, NULL);
}

/**
 * 析构函数
 */
CCondition::~CCondition(){
    pthread_cond_destroy(&m_pthreadCond);
}

void CCondition::Signal(){
    pthread_cond_signal(&m_pthreadCond);
}

void CCondition::Wait(pthread_mutex_t mutex){
    pthread_cond_wait(&m_pthreadCond, &mutex);
}

void CCondition::Broadcast(){
    pthread_cond_broadcast(&m_pthreadCond);
}

void CCondition::Destroy(){
    pthread_cond_destroy(&m_pthreadCond);
}