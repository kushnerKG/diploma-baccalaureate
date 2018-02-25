#include "Task.h"

Task::Task(int start, int finish):start(start), finish(finish)
{
}


Task::~Task()
{
}

int Task::getStart() const
{
	return start;
}

int Task::getFinish() const
{
	return finish;
}
