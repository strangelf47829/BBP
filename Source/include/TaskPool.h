#ifndef BBP_STD_TASKPOOL_H
#define BBP_STD_TASKPOOL_H

#include "Tasks.h"
#include "Memory.h"

namespace BBP
{
	namespace std
	{
		// A taskpool tracks ..
		class TaskPool : public TaskFlowInterface
		{
			// Page for tasks
			std::PAGE<TaskFlowInterface *> tasks;

			// Look for the index of a task. invalid if none found
			std::index_t lookFor(TaskFlowInterface *);

			// Tracks the blocked-state of this pool
			bool isBlocked;

			// Tracks the waiting state
			bool isWaiting;

			// For who?
			TaskFlowInterface *waitingFor;

		public:

			// Constructor
			TaskPool(PAGE<TaskFlowInterface *> &p)
				: tasks(p),
				isBlocked(false),
				isWaiting(false),
				waitingFor(nullptr)
			{
				// Initialize everything as 0
				std::size_t size = seqlen(tasks);
				for (std::index_t idx = 0; idx < size; idx++)
					tasks[idx] = nullptr;
			}

			// Add task
			void Add(TaskFlowInterface *);

			// Remove task
			void Remove(TaskFlowInterface *);

			// Step over all tasks. Returns false if none stepped
			bool Step();

			// Remove each task
			void Kill();

			// Await the execution of a task before continuing execution
			void Await(TaskFlowInterface *);

			// Tell this task to block execution
			void Block();

			// Tell this task to unblock execution
			void Unblock();

			// If cold, task does not exist anymore.
			bool Cold();


		};

	}
}

#endif
