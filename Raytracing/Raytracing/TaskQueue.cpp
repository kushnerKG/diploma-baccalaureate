#include "TaskQueue.h"

TaskQueue::TaskQueue(int height, int threadCount)
{
	int delta = height / (threadCount * TASK_ON_THREAD);
	for (int i = 0; i < height; i += delta)
	{
		queue.push(Task(i, i + delta < height ? i + delta : height));
	}
}

TaskQueue::~TaskQueue()
{
	
}

Task TaskQueue::popTask()
{
	mutex.lock();
	Task task = !queue.empty() ? queue.front() : Task(-1, -1);
	if (!queue.empty())
	{
		queue.pop();
	}
	mutex.unlock();

	return task;
}
