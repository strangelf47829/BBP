#ifndef BBP_STD_ASYNC_H
#define BBP_STD_ASYNC_H

#include "List.h"

namespace BBP
{
	namespace std
	{

		// Type used to store list of items
		template<typename... T>
		using async_stack_t = std::List<T...>;

		// Function declaration
		template<typename R, typename A>
		struct async_func_t;

		// Function signature
		template<typename R, typename...A>
		struct async_signature_t
		{
			// Return type
			using signature_return_t = R;

			// For a given stack, this is the function being used
			template<typename... S>
			using async_function = async_func_t<async_signature_t<R, A...>, async_stack_t<S...>>;

		};

		// Specialized type
		template<typename R, typename... A, typename... K>
		struct async_func_t<async_signature_t<R, A...>, async_stack_t<K...>>
		{
			// The type used for the function
			using lambda_t = R(*)(async_stack_t<K...> &, async_stack_t<A...> &);

			// The asynchronous stack that simulates a 'stack'
			async_stack_t<K...> stack;

			// The arguments page (Must be set later)
			async_stack_t<A...> *args;

			// List of functions
			std::PAGE<lambda_t> funcs;

			// The index currently at
			std::index_t idx;

			// Constructor
			async_func_t(std::size_t fncCount, lambda_t *fncVector, K... values)
				: stack(values...),
				args(nullptr),
				funcs(fncCount, fncVector),
				idx(fncCount -1),
				default_idx(fncCount -1)
			{}

			// Default constructor
			async_func_t(std::size_t fncCount, lambda_t *fncVector)
				: stack(),
				args(nullptr),
				funcs(fncCount, fncVector),
				idx(fncCount - 1),
				default_idx(fncCount - 1)
			{}

			// Reset this 'job'
			void Reset()
			{
				idx = default_idx + 1;
			}

			std::index_t reverse(std::index_t ix)
			{
				return default_idx - ix;
			}

		private:

			// The index specified by the constructor
			const std::index_t default_idx;

		};

		// Declare class that will be the abstract interface
		template<typename T, typename K>
		class abstract_async_call;

		// Then define specialization for propper values
		template<typename R, typename... A, typename... K>
		class abstract_async_call<async_signature_t<R, A...>, async_stack_t<K...>>
		{
		public:

			// 'Call' the function asynchronously (Called from the caller side)
			virtual void Async(A...) = 0;

			// Synchronous call. Waits until the operation is completed.
			virtual R Sync(A...) = 0;

			// 'Step' the function in time (this is what makes this asynchronous). Returns false on completion
			virtual bool Step() = 0;

			// 'Await' waits until the function has completed running, in which case it will return the value.
			virtual R Await() = 0;

			// 'Yield' yields the operation of the async call (and anything inside of that thread), called from the function itself.
			virtual void Yield() = 0;

			// 'Kill' completely kills the call.
			virtual void Kill() = 0;
		};

		// Declare class that will be the abstract interface
		template<typename... T>
		class async_task;

		// Then define specialization for propper values
		template<typename R, typename... A, typename... K>
		class async_task<R (*)(A...), K...> : public abstract_async_call<async_signature_t<R, A...>, async_stack_t<K...>>
		{
			// Private buffer
			async_stack_t<A...> argsStack;

		public:

			// Define the functor
			using functor_t = async_func_t<async_signature_t<R, A...>, async_stack_t<K...>>;

			// Define lambda type
			using lambda_t = async_func_t<async_signature_t<R, A...>, async_stack_t<K...>>::lambda_t;

			// If pages are known
			async_task(std::size_t fncCount, lambda_t *fncList, K...args)
				: functor(fncCount, fncList, args...)
			{}

			// If default values exist
			async_task(std::size_t fncCount, lambda_t *fncList)
				: functor(fncCount, fncList)
			{}

			// Must respect functor constructor
			async_task( K...args)
				: functor(0, nullptr, args...)
			{}

			// 'Call' the function asynchronously (Called from the caller side)
			void Async(A... args) override
			{
				// Create new arguments
				argsStack = async_stack_t<A...>(args...);

				// Set arguments
				functor.args = &argsStack;

				// Reset functor
				functor.Reset();
			}

			// Synchronous call. Waits until the operation is completed.
			R Sync(A... args) override
			{}

			// 'Step' the function in time (this is what makes this asynchronous). Returns false on completion
			bool Step() override
			{
				// If already at index 0 do nothing
				if (functor.idx == 0)
					return false;

				// Then reduce index
				functor.idx--;

				// Call functors
				returnVal = functor.funcs[functor.reverse(functor.idx)](functor.stack, *functor.args);
				
				// Return if anything is left
				return functor.idx;
			}

			// 'Await' waits until the function has completed running, in which case it will return the value.
			R Await() override
			{
				// While stepping...
				while (Step());

				// Only return when done
				return returnVal;
			}

			// 'Yield' yields the operation of the async call (and anything inside of that thread), called from the function itself.
			void Yield() override
			{ }

			// 'Kill' completely kills the call.
			void Kill() override
			{}

			~async_task()
			{
				// Default behaviour is to wait.
				Await();
			}

		private:

			// Define functor
			functor_t functor;

			// Return value
			R returnVal;

		};



	}
}

#endif