#ifndef BBP_SYSCALLS_ARGS_H
#define BBP_SYSCALLS_ARGS_H

#include "List.h"
#include "Memory.h"

namespace BBP
{
	namespace system
	{

		// Type used to refer to system calls
		using syscall_t = std::index_t;

		// Forward declaration
		struct syscall_args_t;

		// Get a system call
		template<typename T>
		T &getInValue(syscall_args_t &args, std::index_t idx);

		template<typename T>
		T &getOutValue(syscall_args_t &args, std::index_t idx);

		// A structure used to store values between calls
		template<typename... T>
		struct syscall_arg_data
		{
			// Empty data
			std::PAGE<void *> dataArray;
		};

		// Every other generic data container
		template<typename T, typename... K>
		struct syscall_arg_data<T, K...>
		{
		private:

			// Initializer structure
			template<std::index_t idx, typename... P>
			struct initializer;

			// Specialize last case
			template<typename P>
			struct initializer<0, P>
			{
				static void set(syscall_arg_data<T, K...> &data, std::PAGE<void *> &arr)
				{
					// Now set last
					arr[0] = data.list.template getPtr<0>();
				}
			};

			// Specialize any other case
			template<std::index_t idx, typename P, typename... L>
			struct initializer<idx, P, L...>
			{
				// Set element and recurse
				static void set(syscall_arg_data<T, K...> &data, std::PAGE<void *> &arr)
				{
					// Set element
					arr[idx] = data.list.template getPtr<idx>();

					// Recurse
					initializer<idx - 1, L...>::set(data, arr);
				}
			};

		public:
			// Constructor
			syscall_arg_data(T first, K... otherData)
				:	objects(sizeof...(K)+1),
				list(first, otherData...)
			{
				// Initialize
				initializer<sizeof...(K),T,K...>::set(*this, dataArray);
			}

			// Get a pointer to a value
			void *ptr (std::index_t idx)
			{
				// Check if operation is valid
				if (idx >= objects)
					std::exception("Invalid index: Out of range", ERANGE);

				// Return value of object
				return dataArray[idx];
			}

			// Get object
			template<std::index_t idx>
			auto &get()
			{
				return list.template get<idx>();
			}

			// Get pointers
			void *operator[] (std::index_t idx)
			{
				return ptr(idx);
			}


			// Get count of objects
			std::size_t objectCount()
			{
				return objects;
			}
			
			// Void pointers to objects within
			std::STATIC_PAGE<void *, 1 + sizeof...(K)> dataArray;

			// Store object count
			std::size_t objects;

			// Private list
			std::List<T, K...> list;
		};

		// Type used to denote the type used between system calls
		struct syscall_args_t
		{
			// Function return type
			std::errno_t errno;

			// Arguments and return values
			std::PAGE <void *> *returnValues;
			std::PAGE <void *> *argumentValues;

			syscall_args_t(std::PAGE<void *> &returnVal, std::PAGE<void *> &argumentVal)
				: errno(0),
				returnValues(&returnVal),
				argumentValues(&argumentVal)
			{}

			template<template<typename...> class P, typename... T, typename... K>
			syscall_args_t(P<T...> &, P<K...> &) = delete;

			template<typename... T, typename... K>
			syscall_args_t(syscall_arg_data<T...> &out, syscall_arg_data<K...> &in)
				: errno(0),
				returnValues(&out.dataArray),
				argumentValues(&in.dataArray)
			{}

		};

	}
}

#include "SyscallArgs.hxx"

#endif