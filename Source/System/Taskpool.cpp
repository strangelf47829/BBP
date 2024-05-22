#include "../include/TaskPool.h"

BBP::std::index_t BBP::std::TaskPool::lookFor(TaskFlowInterface *ptr)
{
	// Look for
	std::size_t size = seqlen(tasks);

	// Go through
	for (std::index_t idx = 0; idx < size; idx++)
	{
		// If pointer equals, return index
		if (tasks[idx] == ptr)
			return idx;
	}

	// Could not located 
	throw std::exception("Could not locate pointer", ENODATA);
}

void BBP::std::TaskPool::Add(TaskFlowInterface *task)
{
	// Look for empty task
	std::index_t emptyIndex = lookFor(nullptr);

	// then assign task to empty index
	tasks[emptyIndex] = task;
}

void BBP::std::TaskPool::Remove(TaskFlowInterface *task)
{
	// Look for task
	std::index_t taskIndex = lookFor(task);

	// then assign nullptr to index
	tasks[taskIndex] = nullptr;
}

bool BBP::std::TaskPool::Step()
{
	// If blocked, return false
	if (isBlocked)
		return false;

	// If is waiting, check if waiting has finished
	if (isWaiting)
	{
		// Check for valid pointer
		if (waitingFor == nullptr)
			return false;

		// If not cold return false
		if (waitingFor->Cold() == false)
			return false;

		// Reset waiting for
		isWaiting = false;
		waitingFor = nullptr;
	}

	// Keep track of
	bool stepValue = false;

	// Get task count
	std::size_t size = seqlen(tasks);
	
	// Loop over
	for (std::index_t idx = 0; idx < size; idx++)
	{
		// Get task
		TaskFlowInterface *ptr = tasks[idx];

		// if none assigned, skip
		if (ptr == nullptr)
			continue;

		// Step over
		stepValue |= ptr->Step();
	}

	// Return stepValue
	return stepValue;
}

// Kills 
void BBP::std::TaskPool::Kill()
{
	// Get task count
	std::size_t size = seqlen(tasks);

	// Loop over
	for (std::index_t idx = 0; idx < size; idx++)
	{
		// Get task
		TaskFlowInterface *ptr = tasks[idx];

		// if none assigned, skip
		if (ptr == nullptr)
			continue;

		// Kill task, then assign nullptr
		ptr->Kill();
		tasks[idx] = nullptr;
	}
}

// Wait for task to finish
void BBP::std::TaskPool::Await(TaskFlowInterface *ptr)
{
	// Set flags
	isWaiting = true;
	waitingFor = ptr;
}


void BBP::std::TaskPool::Block()
{
	// Set flags
	isBlocked = true;
}

void BBP::std::TaskPool::Unblock()
{
	isBlocked = false;
}

bool BBP::std::TaskPool::Cold()
{
	// Go through each task. If no tasks are available, return true.
		// Get task count
	std::size_t size = seqlen(tasks);

	// Loop over
	for (std::index_t idx = 0; idx < size; idx++)
	{
		// Get task
		TaskFlowInterface *ptr = tasks[idx];

		// if none assigned, skip
		if (ptr == nullptr)
			continue;

		// If not cold, return false
		if (ptr->Cold() == false)
			return false;
	}

	// Cold
	return true;
}