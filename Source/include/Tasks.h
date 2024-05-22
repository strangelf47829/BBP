#ifndef BBP_STD_TASKS_H
#define BBP_STD_TASKS_H

#include "Async.h"
#include "Memory.h"

namespace BBP
{

	namespace std
	{

		// Define the interface used to 
		class TaskFlowInterface
		{
		public:
			// Renew current task, spawn, then step. Returns true if could step, otherwise false.
			virtual bool Step() = 0;

			// Kill this task
			virtual void Kill() = 0;

			// Tell this task to await execution of a different task
			virtual void Await(TaskFlowInterface *) = 0;

			// Tell this task to block execution
			virtual void Block() = 0;

			// Tell this task to unblock execution
			virtual void Unblock() = 0;

			// If cold, task does not exist anymore.
			virtual bool Cold() = 0;
		};

		// Declare the interface used to ...
		template<typename R, typename... A>
		class TaskInterface
		{
		public:

			// Create a new task
			virtual TaskFlowInterface *operator() (A...) = 0;

			// Retrieve task
			virtual R &Join() = 0;
		};

		// This is a class that instances a task
		template<typename FuncSig, std::size_t N, typename... Stack_t>
		class TaskTemplate
		{
		public:

			// Using this type as its atomic elements
			using atomic_t = std::async_task<FuncSig, Stack_t...>::lambda_t;

		private:

			// Declare list of functors
			STATIC_PAGE<atomic_t, N> functors;

		public:

			// Constructor
			TaskTemplate(const atomic_t args[N])
			{
				for (std::index_t idx = 0; idx < N; idx++)
					functors[idx] = args[idx];
			}

			// Fetch a copy of
			void ShallowCopy(PAGE<atomic_t> &page)
			{
				page = PAGE<atomic_t>(functors);
			}

		};

		// A generic task that runs until completion
		template<typename... T>
		class Task;

		// A task that repeats until explicitly stopped
		template<typename... T>
		class RepeatingTask;

		// Specialization of task
		template<typename FuncReturn, typename... FuncArgs, typename... Stack_t>
		class Task<FuncReturn(*)(FuncArgs...), Stack_t...> : public TaskFlowInterface, public TaskInterface<FuncReturn, FuncArgs...>
		{
			// Using this type as its atomic elements
			using atomic_t = std::async_task<FuncReturn(*)(FuncArgs...), Stack_t...>::lambda_t;

			// This is the page of functors
			std::PAGE<atomic_t> functors;

			// The amount of functors
			std::size_t functorCount;

			// The index of current functor
			std::index_t activeFunctor;

			// Active stack
			async_stack_t<Stack_t...> stackList;

			// Active argument list
			async_stack_t<FuncArgs...> argsList;

			// is blocked?
			bool isBlocked;

			// is waiting for?
			bool isWaiting;

			// Who are we waiting for?
			TaskFlowInterface *waitingFor;

			// Return object
			FuncReturn returnObj;

		public:

			// Create a new task
			TaskFlowInterface *operator() (FuncArgs... args) override
			{
				// Copy over each thing
				argsList = async_stack_t<FuncArgs...>(args...);

				// Then set stack
				stackList = async_stack_t<Stack_t...>();

				// Set index at 0
				activeFunctor = 0;

				// Set functor count
				functorCount = std::seqlen(functors);

				// set blocked and waiting to false
				isBlocked = false;
				isWaiting = false;
				waitingFor = nullptr;

				// Return 'this'
				return this;
			};

			// Retrieve task
			FuncReturn &Join() override
			{
				// Run until not able to
				while (Step());

				// Return object
				return returnObj;
			}

			// Get page
			std::PAGE<atomic_t> &getList()
			{
				return functors;
			}

			// Renew current task, spawn, then step. Returns true if could step, otherwise false.
			bool Step() override
			{
				// If blocked, do nothing
				if (isBlocked)
					return false;

				// If isWaiting,
				if (isWaiting)
				{
					// If waiting for nothing, do nothing
					if (waitingFor == nullptr)
						return false;

					// Otherwise if not cold, return false
					if (waitingFor->Cold() == false)
						return false;

					// Reset waiting for
					isWaiting = false;
					waitingFor = nullptr;
				}

				// If cold, do nothing
				if (Cold())
					return false;

				// Get atomic value
				atomic_t functor = functors[activeFunctor++];

				// Do function
				returnObj = functor(stackList, argsList);

				// Step happened
				return true;
			};

			// Kill this task
			void Kill() override
			{
				// Set activeFunctor equal to functorCount
				activeFunctor = functorCount;
			}

			// Tell this task to await execution of a different task
			void Await(TaskFlowInterface *ptr) override
			{
				// Set is waiting to true
				isWaiting = true;

				// Set pointer
				waitingFor = ptr;
			}

			// Tell this task to block execution
			void Block() override
			{
				isBlocked = true;
			}

			// Tell this task to unblock execution
			void Unblock() override
			{
				isBlocked = false;
			}

			// If cold, task does not exist anymore.
			bool Cold() override
			{
				// If activeFunctor is greater than, ...
				if (activeFunctor >= functorCount)
					return true;

				return false;
			}
		};

		// This is a task that repeats until given a stop command
		template<typename FuncReturn, typename... FuncArgs, typename... Stack_t>
		class RepeatingTask<FuncReturn(*)(FuncArgs...), Stack_t...> : public TaskFlowInterface, public TaskInterface<FuncReturn, FuncArgs...>
		{
			// Using this type as its atomic elements
			using atomic_t = std::async_task<FuncReturn(*)(TaskFlowInterface *, FuncArgs...), Stack_t...>::lambda_t;

			// This is the page of functors
			atomic_t functor;

			// Active stack
			async_stack_t<Stack_t...> stackList;

			// Active argument list
			async_stack_t<TaskFlowInterface *, FuncArgs...> argsList;

			// is blocked?
			bool isBlocked;

			// is waiting for?
			bool isWaiting;

			// If active
			bool isActive;

			// Who are we waiting for?
			TaskFlowInterface *waitingFor;

			// Return object
			FuncReturn returnObj;

		public:

			// Set atomic
			void SetFunctor(atomic_t ptr)
			{
				functor = ptr;
			}

			// Create a new task
			TaskFlowInterface *operator() (FuncArgs... args) override
			{
				// Copy over each thing
				argsList = async_stack_t<TaskFlowInterface *, FuncArgs...>(this, args...);

				// Then set stack
				stackList = async_stack_t<Stack_t...>();

				// Set active
				isActive = true;

				// set blocked and waiting to false
				isBlocked = false;
				isWaiting = false;
				waitingFor = nullptr;

				// Return 'this'
				return this;
			};

			// Stop this task
			FuncReturn &Join() override
			{
				// Kill
				Kill();

				// Return object
				return returnObj;
			}

			// Renew current task, spawn, then step. Returns true if could step, otherwise false.
			bool Step() override
			{
				// If blocked, do nothing
				if (isBlocked)
					return false;

				// If isWaiting,
				if (isWaiting)
				{
					// If waiting for nothing, do nothing
					if (waitingFor == nullptr)
						return false;

					// Otherwise if not cold, return false
					if (waitingFor->Cold() == false)
						return false;

					// Reset waiting for
					isWaiting = false;
					waitingFor = nullptr;
				}

				// If cold, do nothing
				if (Cold())
					return false;

				// Run functor
				returnObj = functor(stackList, argsList);

				// Step happened
				return true;
			};

			// Kill this task
			void Kill() override
			{
				// set inactive
				isActive = false;
			}

			// Tell this task to await execution of a different task
			void Await(TaskFlowInterface *ptr) override
			{
				// Set is waiting to true
				isWaiting = true;

				// Set pointer
				waitingFor = ptr;
			}

			// Tell this task to block execution
			void Block() override
			{
				isBlocked = true;
			}

			// Tell this task to unblock execution
			void Unblock() override
			{
				isBlocked = false;
			}

			// If cold, task does not exist anymore.
			bool Cold() override
			{
				// If not active this task is cold
				return isActive == false;
			}
		};


	}

}


#endif