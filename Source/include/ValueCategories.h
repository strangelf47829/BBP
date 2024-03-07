#ifndef BBP_LIB_VALUECATEGORIES
#define BBP_LIB_VALUECATEGORIES

#include "Instructions.h"
#include "StateMachine.h"

namespace BBP
{
	namespace userspace
	{

		class pvalue
		{
		protected:
			// Functions used throughout everything
			bool isArgumentLValue(userspace::Instruction::Arg &);
			bool isArgumentRValue(userspace::Instruction::Arg &);

			bool isAddressRegisterAddress(std::address_t);
			register_t *getAddressForRegister(userspace::StateMachine &, std::address_t);

		public:
			
			// Functions used to reference, dereference, and resolve a value for
			virtual void dereference(userspace::StateMachine &state, std::address_t) = 0;
			virtual void reference(userspace::StateMachine &state, pvalue &) = 0;

			virtual std::word resolve(userspace::StateMachine &state) = 0;
			virtual void assign(userspace::StateMachine &state, userspace::pvalue &, std::byte bytes) = 0;
			virtual void assign(userspace::StateMachine &state, std::word, std::byte bytes) = 0; // Quickly assign value

			// Get own address
			virtual std::address_t getOwnPhysicalAddress() = 0;
			virtual std::address_t getOwnVirtualAddress() = 0;

			// User page protections
			virtual bool canUserWriteTo(userspace::StateMachine& state) = 0;
			virtual bool canUserReadFrom(userspace::StateMachine& state) = 0;
			virtual bool canUserExecuteFrom(userspace::StateMachine& state) = 0;

		};

		class rvalue : public virtual pvalue
		{
		protected:
			std::word data;
			bool isLiteral;

			std::address_t referencedAddress;
			std::address_t physicalAddress;
			bool isReferenceToAddress;

		public:
			rvalue();
			rvalue(std::word);
			rvalue(userspace::StateMachine &state, userspace::Instruction::Arg &);

			// Functions to reference and dereference
			void dereference(userspace::StateMachine &state, std::address_t);
			void reference(userspace::StateMachine &state, pvalue &);

			// Reading and writing
			std::word resolve(userspace::StateMachine &state);
			void assign(userspace::StateMachine &state, userspace::pvalue &, std::byte bytes);
			void assign(userspace::StateMachine &state, std::word, std::byte bytes); // Quickly assign value

			// Functions to resolve
			std::address_t getOwnPhysicalAddress();
			std::address_t getOwnVirtualAddress();

			// User page protections
			bool canUserWriteTo(userspace::StateMachine& state);
			bool canUserReadFrom(userspace::StateMachine& state);
			bool canUserExecuteFrom(userspace::StateMachine& state);
		};

		class lvalue : public virtual pvalue
		{
		protected:
			// Register lvalues
			register_t *_register;
			bool isRegister;

			// Binary lvalues
			std::address_t PhysicalAddress;
			std::address_t VirtualMemory;
			bool isAddress;

		public:
			// Constructors
			lvalue();
			lvalue(userspace::StateMachine &state, userspace::Instruction::Arg &);

			// Stores a virtual address
			lvalue(userspace::StateMachine &state, std::word);

			// Functions to reference and dereference
			void dereference(userspace::StateMachine &state, std::address_t);
			void reference(userspace::StateMachine &state, pvalue &);

			// Reading and writing
			std::word resolve(userspace::StateMachine &state);
			void assign(userspace::StateMachine &state, userspace::pvalue &, std::byte bytes);
			void assign(userspace::StateMachine &state, std::word, std::byte bytes); // Quickly assign value

			// Functions to resolve
			std::address_t getOwnPhysicalAddress();
			std::address_t getOwnVirtualAddress();

			// User page protections
			bool canUserWriteTo(userspace::StateMachine& state);
			bool canUserReadFrom(userspace::StateMachine& state);
			bool canUserExecuteFrom(userspace::StateMachine& state);
		};



		class xvalue : public virtual pvalue
		{
		protected:
			bool isLvalue;
			bool isRvalue;

			lvalue lval;
			rvalue rval;
			

		public:
			xvalue();
			xvalue(userspace::StateMachine &state, userspace::Instruction::Arg &);

			// Functions to reference and dereference
			void dereference(userspace::StateMachine &state, std::address_t);
			void reference(userspace::StateMachine &state, pvalue &);

			// Reading and writing
			std::word resolve(userspace::StateMachine &state);
			void assign(userspace::StateMachine &state, userspace::pvalue &, std::byte bytes);
			void assign(userspace::StateMachine &state, std::word, std::byte bytes); // Quickly assign value

			// Functions to resolve
			std::address_t getOwnPhysicalAddress();
			std::address_t getOwnVirtualAddress();

			// User page protections
			bool canUserWriteTo(userspace::StateMachine& state);
			bool canUserReadFrom(userspace::StateMachine& state);
			bool canUserExecuteFrom(userspace::StateMachine& state);

		};

	}
}

#endif