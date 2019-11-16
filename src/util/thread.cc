#include "thread.h"
// abc

#ifdef _WIN32

// Windows version
#define NOMINMAX 
#include <Windows.h>
namespace lib_calvin
{
	thread_type create_thread(thread_function_type function, void *arg) {
		return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)function, (LPVOID)arg, 0, NULL);
	}

	void wait_for_thread(thread_type thread) {
		WaitForSingleObject(thread, INFINITE);
	}

	void thread_yield() {
		SwitchToThread(); 
	}

	lock_type create_lock() {
		return CreateMutex(NULL, false, NULL);
	}

	void acquire_lock(lock_type lock) {
		WaitForSingleObject(lock, INFINITE); 
	}

	void release_lock(lock_type lock) {
		ReleaseMutex(lock);
	}

	void destroy_lock(lock_type lock) {
		CloseHandle(lock);
	}
}

#else
// Unix version
namespace lib_calvin
{
	thread_type create_thread(thread_function_type function, void * arg) {
		pthread_t newThread;
		pthread_create(&newThread, NULL, function, arg);
		return newThread;
	}

	void wait_for_thread(thread_type thread) {
		pthread_join(thread, NULL);
	}

	lock_type create_lock() {
		lock_type lock;
		return lock;
	}

	void acquire_lock(lock_type lock) {
		pthread_mutex_lock(&lock);
	}

	void release_lock(lock_type lock) {
		pthread_mutex_unlock(&lock);
	}

	void destroy_lock(lock_type lock) {
	}
}

#endif

