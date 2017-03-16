#ifndef _MUTEX_SEM_THREAD_WL_H_
#define _MUTEX_SEM_THREAD_WL_H_
//mutex semaphore and thread operation define

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)  
//Windows threads.
#include <windows.h>
typedef HANDLE Thread;
typedef HANDLE Mutex;
typedef HANDLE Semaphore;
#else
//POSIX threads.
#include <pthread.h>
#include <semaphore.h>
typedef pthread_t Thread;
typedef pthread_mutex_t Mutex;
typedef sem_t Semaphore;

#endif

//Mutex and Semaphore
void ThreadMutexInit(Mutex & mutex);
void ThreadMutexLock(Mutex & mutex);
void ThreadMutexUnlock(Mutex & mutex);
void ThreadMutexDestroy(Mutex & mutex);
void ThreadSemInit(Semaphore & sem, int init, int max);
void ThreadSemWait(Semaphore & sem);
void ThreadSemRelease(Semaphore & sem);
void ThreadSemDestroy(Semaphore & sem);

//Thread
Thread ThreadCreate(void* func, void *data);
void ThreadWait(Thread thread);
void ThreadForceClose(Thread thread);
void ThreadsWait(const Thread * threads, int num);

#endif