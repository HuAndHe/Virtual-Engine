#pragma once
#include "../Common/TypeWiper.h"
#include <thread>
#include <mutex>
#include <condition_variable>
class TaskThread
{
private:
	std::thread thd;
	std::mutex mtx;
	std::mutex mainThreadMtx;
	std::condition_variable cv;
	std::condition_variable mainThreadCV;
	bool enabled;
	bool runNext;
	bool mainThreadLocked;
	void(*funcData)(void*);
	void* funcBody;
	static void RunThread(TaskThread* ptr);
	template <typename T>
	inline static void Run(void* ptr)
	{
		(*(T*)ptr)();
	}
public:
	TaskThread();
	void ExecuteNext();
	void Complete();
	bool IsCompleted() const { return !mainThreadLocked; }
	~TaskThread();
	template <typename T>
	void SetFunctor(T& func)
	{
		using Type = typename std::remove_const_t<T>;
		funcData = Run<Type>;
		funcBody = &((Type&)func);
	}
};
