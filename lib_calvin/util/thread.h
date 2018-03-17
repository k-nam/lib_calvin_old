#ifndef LIB_CALVIN__THREAD__COMMON_THREAD_H
#define LIB_CALVIN__THREAD__COMMON_THREAD_H

// Function type will be coerced to suitable type for each platform


#ifdef _WIN32

// Windows version
#include <Windows.h>
namespace lib_calvin_util
{
	typedef HANDLE thread_type;
	typedef HANDLE lock_type;
}

#else
// Unix version
namespace lib_calvin_thread 
{
	typedef ptheard_t thread_type;
	typedef pthread_mutex_lock *lock_type;
}

#endif

//Common part
namespace lib_calvin_util
{
	typedef void *(*thread_function_type)(void *) ;
	thread_type create_thread(thread_function_type function, void *arg);
	void wait_for_thread(thread_type thread);

	void thread_yield();

	lock_type create_lock();
	void acquire_lock(lock_type);
	void release_lock(lock_type);
	void destroy_lock(lock_type);
}


#endif