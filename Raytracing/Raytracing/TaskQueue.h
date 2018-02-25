#pragma once
#include <queue>
#include <mutex>
#include "Task.h"

class TaskQueue
{
public:
	TaskQueue(int height, int threadCount);
	~TaskQueue();

	Task popTask();

private:
	std::queue<Task> queue;

	std::mutex mutex;

	const static int TASK_ON_THREAD = 6;
};
