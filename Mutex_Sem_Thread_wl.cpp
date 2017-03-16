#include "Mutex_Sem_Thread_wl.h"
#include<iostream>

//Mutex and Semaphore
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
void ThreadMutexInit(Mutex & mutex)
{
	mutex = ::CreateMutex(NULL, FALSE, NULL);
}
void ThreadMutexLock(Mutex & mutex)
{
	::WaitForSingleObject(mutex, INFINITE);
}
void ThreadMutexUnlock(Mutex & mutex)
{
	::ReleaseMutex(mutex);
}
void ThreadMutexDestroy(Mutex & mutex)
{
	CloseHandle(mutex);
}
void ThreadSemInit(Semaphore & sem, int init, int max)
{
	sem = ::CreateSemaphore(NULL, init, max, NULL);
}
void ThreadSemWait(Semaphore & sem)
{
	::WaitForSingleObject(sem, INFINITE);
}
void ThreadSemRelease(Semaphore & sem)
{
	::ReleaseSemaphore(sem, 1, NULL);
}
void ThreadSemDestroy(Semaphore & sem)
{
	CloseHandle(sem);
}
#else
void ThreadMutexInit(Mutex& mutex){
	pthread_mutex_init(&mutex, NULL);
}
void ThreadMutexLock(Mutex& mutex){
	pthread_mutex_lock(&mutex);
}
void ThreadMutexUnlock(Mutex& mutex){
	pthread_mutex_unlock(&mutex);
}
void ThreadMutexDestroy(Mutex & mutex)
{
	pthread_mutex_destroy(&mutex);
}
//the parameter max is unnecessary in linux ThreadSemInit
void ThreadSemInit(Semaphore & sem, int init, int max)
{
	if (0 != sem_init(&sem, 0, init))
		std::cerr << "semaphore init error!" << std::endl;
}
void ThreadSemWait(Semaphore & sem)
{
	sem_wait(&sem);
}
void ThreadSemRelease(Semaphore & sem)
{
	sem_post(&sem);
}
void ThreadSemDestroy(Semaphore & sem)
{
	sem_destroy(&sem);
}
#endif

//thread
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)  
//Windows threads. 
Thread ThreadCreate(void * func, void * data){
	return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, data, 0, NULL);
}

//Wait for the thread's finish and then close it
void ThreadWait(Thread thread){      
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
}

void ThreadForceClose(Thread thread){    //force close the thread
	TerminateThread(thread, 0);
	CloseHandle(thread);
}

void ThreadsWait(const Thread * threads, int num){   //Wait for multiple threads
	WaitForMultipleObjects(num, threads, true, INFINITE);
	for (int i = 0; i < num; i++)
		CloseHandle(threads[i]);
}
#else
Thread ThreadCreate(void * func, void * data){    //Create thread
	pthread_t thread;
	pthread_create(&thread, NULL, func, data);
	return thread;
}
//Wait for the thread's finish and then close it
void ThreadWait(Thread thread){
	pthread_join(thread, NULL);
}

void ThreadForceClose(Thread thread){    //force close the thread
	pthread_cancel(thread);
}
void ThreadsWait(const CUTThread * threads, int num){    //Wait for multiple threads
	for (int i = 0; i < num; i++)
		pthread_join(threads[i]);
}
#endif