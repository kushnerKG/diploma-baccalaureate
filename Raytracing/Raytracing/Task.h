#pragma once
class Task
{
public:
	Task(int start, int finish);
	~Task();

	int getStart() const;
	int getFinish() const;

private:
	const int start;
	const int finish;
};

