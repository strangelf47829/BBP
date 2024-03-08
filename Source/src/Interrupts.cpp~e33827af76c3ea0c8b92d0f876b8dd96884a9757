/*

Copyright 2023 PCiD

This file is part of BBP.

BBP is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BBP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with BBP. If not, see
<https://www.gnu.org/licenses/>.

Author: Rafael de Bie

*/


#include "../include/Kernel.h"

BBP::Services::Interrupts::Interrupt_handle *BBP::Services::Interrupts::kernelInterrupts = nullptr;
BBP::Services::Interrupts::Interrupt_handle **BBP::Services::Interrupts::interrupts = nullptr;
BBP::Services::Interrupts::Lua::Lua_Interrupt_handle **BBP::Services::Interrupts::Lua::lua_interrupts = nullptr;
BBP::Services::Interrupts::Timer::Timer_Interrupt_handle **BBP::Services::Interrupts::Timer::timers = nullptr;

u_char BBP::Services::Interrupts::interrupt_count = 0;
u_char BBP::Services::Interrupts::Lua::lua_interrupts_count = 0;
u_char BBP::Services::Interrupts::Timer::timers_count = 0;

bool BBP::Services::Interrupts::Poll(int index)
{
	// If the index is invalid, return true because returning false may cause an infinite loop
	if (BBP::Services::Interrupts::interrupts[index] == nullptr)
		return true;

	// If the interrupt has not been triggered, return false
	if (!BBP::Services::Interrupts::interrupts[index]->triggered)
		return false;

	// If this point is reached, we know the interrupt has been triggered. Return true and clear if this interrupt has a timer associated with it that is not persistent.
	if (BBP::Services::Interrupts::interrupts[index]->attachedTimerInterrupt == nullptr)
	{
		// This interrupt has no timer associated with it, so clear and return true.
		BBP::Services::Interrupts::Clear(index, true);
		return true;
	}

	// This interrupt has a timer associated with it. If it is not persistent, return true and clear
	if (!(*BBP::Services::Interrupts::interrupts[index]->attachedTimerInterrupt)->persistent)
	{
		// Timer is not persistent. Clear and return true
		BBP::Services::Interrupts::Clear(index, true);
		return true;
	}

	// This interrupt has a timer associated with it that is also persistent.
	// In this case, trigger the lua interrupt if one is associated to this interrupt.
	// Then reset the interrupt.

	if(BBP::Services::Interrupts::interrupts[index]->attachedLuaInterrupt != nullptr)
		BBP::Services::Interrupts::Lua::Trigger(BBP::Services::Interrupts::interrupts[index]->attachedLuaInterrupt);

	// Get reference to timer interrupt
	BBP::Services::Interrupts::Timer::Timer_Interrupt_handle **handle = BBP::Services::Interrupts::interrupts[index]->attachedTimerInterrupt;
	
	// Reset timer interrupt values
	(*handle)->atTime = BBP::Services::millis();
	(*handle)->timeLeft = (*handle)->length;
	
	// Reset this interrupt
	BBP::Services::Interrupts::interrupts[index]->triggered = false;

	return true;
}

bool BBP::Services::Interrupts::Clear(int index, bool triggerOnClear)
{
	// Get an index to myself
	BBP::Services::Interrupts::Interrupt_handle *handle = BBP::Services::Interrupts::interrupts[index];

	// If the interrupt has a lua interrupt associated with it, find it.
	if (handle->attachedLuaInterrupt != nullptr)
	{
		// The index is really just the difference between the interrupt pointer and the base pointer
		int luaIndex = handle->attachedLuaInterrupt - BBP::Services::Interrupts::Lua::lua_interrupts;

		// If the index is valid, trigger the interrupt and then clear it
		if (luaIndex >= 0 && luaIndex < BBP::Services::Interrupts::Lua::lua_interrupts_count)
		{
			if (triggerOnClear)	// Only trigger lua interrupt if triggerOnClear is on
				BBP::Services::Interrupts::Lua::Trigger(luaIndex); // Execute lua trigger
			BBP::Services::Interrupts::Lua::Clear(luaIndex);
			
		}
	}

	// If the interrupt has a timer interrupt associated with it, find it and clear it.
	if (handle->attachedTimerInterrupt != nullptr)
	{
		// The index is really just the difference between the interrupt pointer and the base pointer
		int timerIndex = handle->attachedTimerInterrupt - BBP::Services::Interrupts::Timer::timers;

		// If the index is valid, clear it
		if (timerIndex >= 0 && timerIndex < BBP::Services::Interrupts::Timer::timers_count)
			BBP::Services::Interrupts::Timer::Clear(timerIndex);
	}

	// Now delete the specified interrupt.
	delete(handle);

	// Set the specified index to a nullptr to 'deallocate' it.
	BBP::Services::Interrupts::interrupts[index] = nullptr;

	return true;
}

void BBP::Services::Interrupts::Timer::Clear(int index)
{
	// Since the timer interrupt handle will be invalidated, we also need to update the underlying handle, if it exists
	if (BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Timer::timers[index]->handle] != nullptr)
		BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Timer::timers[index]->handle]->attachedTimerInterrupt = nullptr;

	// Since this timer has no pointers within it, just delete the structure altogether
	delete(BBP::Services::Interrupts::Timer::timers[index]);

	// now set the pointer at the index to a nullptr. 'BBP::Services::Interrupts::Timer::timers_count--' HAS PURPOSEFULLY BEEN OMITTED
	BBP::Services::Interrupts::Timer::timers[index] = nullptr;
}

void BBP::Services::Interrupts::Lua::Clear(int index)
{
	// Free the char pointer
	BBP::Services::Free(BBP::Services::Interrupts::Lua::lua_interrupts[index]->functionName);

	// Since the lua interrupt handle will be invalidated, we also need to update the underlying handle, if it exists
	if (BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Lua::lua_interrupts[index]->handle] != nullptr)
		BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Lua::lua_interrupts[index]->handle]->attachedLuaInterrupt = nullptr;

	// Now delete the interrupt altogether
	delete(BBP::Services::Interrupts::Lua::lua_interrupts[index]);

	// Now set the pointer at the index to a nullptr. 'BBP::Services::Interrupts::Lua::lua_interrupts_count--' HAS PURPOSEFULLY BEEN OMITTED
	BBP::Services::Interrupts::Lua::lua_interrupts[index] = nullptr;
}

bool BBP::Services::Interrupts::Lua::Trigger(int index)
{
	// Check if the lua interrupt is valid
	if (BBP::Services::Interrupts::Lua::lua_interrupts[index] == nullptr)
		return false;

	// Check if the lua interrupt has a valid owner
	if (BBP::Services::Interrupts::Lua::lua_interrupts[index]->owner == nullptr)
		return false;
	
	// Just do whatever the func string is
	return BBP::Services::ApplicationServices::Lua::Execute(BBP::Services::Interrupts::Lua::lua_interrupts[index]->owner, BBP::Services::Interrupts::Lua::lua_interrupts[index]->functionName, false);
}

bool BBP::Services::Interrupts::Lua::Trigger(BBP::Services::Interrupts::Lua::Lua_Interrupt_handle **handle)
{
	// Check if the argument is valid
	if (handle == nullptr)
		return false;

	// Check if the thing the argument is pointing to is valid
	if (*handle == nullptr)
		return false;

	// Check if the lua interrupt has a valid owner
	if ((*handle)->owner == nullptr)
		return false;

	// Attempt to execute the action. If the application is already busy, return false. Otherwise return true
	return BBP::Services::ApplicationServices::Lua::Execute((*handle)->owner, (*handle)->functionName, false);
}

bool BBP::Services::Interrupts::initializeInterrupts()
{
	BBP::Services::Interrupts::kernelInterrupts = (BBP::Services::Interrupts::Interrupt_handle*)BBP::Services::c_alloc(10, sizeof(BBP::Services::Interrupts::Interrupt_handle));
	BBP::Services::Interrupts::interrupts = (BBP::Services::Interrupts::Interrupt_handle**)BBP::Services::c_alloc(250,sizeof(BBP::Services::Interrupts::Interrupt_handle*));
	BBP::Services::Interrupts::Lua::lua_interrupts = (BBP::Services::Interrupts::Lua::Lua_Interrupt_handle**)BBP::Services::c_alloc(48,sizeof(BBP::Services::Interrupts::Lua::Lua_Interrupt_handle*));
	BBP::Services::Interrupts::Timer::timers = (BBP::Services::Interrupts::Timer::Timer_Interrupt_handle**)BBP::Services::c_alloc(24,sizeof(BBP::Services::Interrupts::Timer::Timer_Interrupt_handle*));
	
	BBP::Services::Interrupts::interrupt_count = 250;
	BBP::Services::Interrupts::Lua::lua_interrupts_count = 48;
	BBP::Services::Interrupts::Timer::timers_count = 24;

	for (int i = 0; i < BBP::Services::Interrupts::interrupt_count; i++)
		BBP::Services::Interrupts::interrupts[i] = nullptr;

	for (int i = 0; i < BBP::Services::Interrupts::Lua::lua_interrupts_count; i++)
		BBP::Services::Interrupts::Lua::lua_interrupts[i] = nullptr;

	for (int i = 0; i < BBP::Services::Interrupts::Timer::timers_count; i++)
		BBP::Services::Interrupts::Timer::timers[i] = nullptr;
	
	return true;
}

void BBP::Services::Interrupts::terminateInterrupts()
{
	BBP::Services::Free(BBP::Services::Interrupts::kernelInterrupts);
	BBP::Services::Free(BBP::Services::Interrupts::interrupts);
	BBP::Services::Free(BBP::Services::Interrupts::Lua::lua_interrupts);
	BBP::Services::Free(BBP::Services::Interrupts::Timer::timers);

	BBP::Services::Interrupts::interrupt_count = 0;
	BBP::Services::Interrupts::Lua::lua_interrupts_count = 0;
	BBP::Services::Interrupts::Timer::timers_count = 0;
}

void BBP::Services::Interrupts::Timer::UpdateTimers()
{
	// Update timers
	for (int i = 0; i < BBP::Services::Interrupts::Timer::timers_count; i++)
	{
		// If the timer is invalid, continue
		if (BBP::Services::Interrupts::Timer::timers[i] == nullptr)
			continue;

		// Get a reference to the interrupt
		BBP::Services::Interrupts::Timer::Timer_Interrupt_handle *handle = BBP::Services::Interrupts::Timer::timers[i];

		// Update the 'timeLeft' field of the struct
		handle->timeLeft = (handle->atTime + handle->length) - BBP::Services::millis();

		// If the timer has no corresponding interrupt, skip it
		if (BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Timer::timers[i]->handle] == nullptr)
			continue;

		// If the specified time has passed, set the stored boolean to true
		if (handle->timeLeft <= 0)
		{
			// Trigger interrupt
			BBP::Services::Interrupts::interrupts[handle->handle]->triggered = true;
		}

	}
}

void BBP::Services::Interrupts::updateInterrupts()
{
	// Call external interrupt function
	BBP::Services::Interrupts::preupdateInterruptsExternal();

	// Update the timers
	BBP::Services::Interrupts::Timer::UpdateTimers();

	// Audit each program
	BBP::Services::ApplicationServices::Lua::AuditAll();

	// For each Lua interrupt, check if it has been triggered
	for (int i = 0; i < BBP::Services::Interrupts::Lua::lua_interrupts_count; i++)
	{
		// If the interrupt is invalid, skip it
		if (BBP::Services::Interrupts::Lua::lua_interrupts[i] == nullptr)
			continue;

		// If the underlying interrupt is invalid, skip it
		if (BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Lua::lua_interrupts[i]->handle] == nullptr)
			continue;

		// If the underlying interrupt has been triggered, trigger the lua event and clear the interrupt
		if (BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Lua::lua_interrupts[i]->handle]->triggered)
		{
			if (BBP::Services::Interrupts::Lua::Trigger(i))
				continue; // The interrupt must be cleared by the lua application
			
			// Add interrupt contents to backlog if the interrupt has that enabled
			if (BBP::Services::Interrupts::Lua::lua_interrupts[i]->addToBacklog)
				BBP::Services::ApplicationServices::Lua::AddBackLog(BBP::Services::Interrupts::Lua::lua_interrupts[i]->owner, BBP::Services::Interrupts::Lua::lua_interrupts[i]->functionName);

		}
	}

	// Reset any persistent timers now. This is in case a poll was missed, which should have been caught anyway in the loop above.
	for (int i = 0; i < BBP::Services::Interrupts::Timer::timers_count; i++)
	{
		// If invalid timer, skip
		if (BBP::Services::Interrupts::Timer::timers[i] == nullptr)
			continue;

		// If timer is not persistent, continue
		if (!BBP::Services::Interrupts::Timer::timers[i]->persistent)
			continue;

		// If timer is not active, continue
		if (!BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Timer::timers[i]->handle]->triggered)
			continue;

		// Reset persistent timer
		BBP::Services::Interrupts::Timer::timers[i]->atTime = BBP::Services::millis();
		BBP::Services::Interrupts::Timer::timers[i]->timeLeft = BBP::Services::Interrupts::Timer::timers[i]->length;
		BBP::Services::Interrupts::interrupts[BBP::Services::Interrupts::Timer::timers[i]->handle]->triggered = false;
	}

	// Call external interrupt function
	BBP::Services::Interrupts::postupdateInterruptsExternal();
}

int BBP::Services::Interrupts::createInterrupt()
{
	// Find the first index with a nullptr
	int index = -1;
	for (int i = 1; i < BBP::Services::Interrupts::interrupt_count; i++) /* i starts at 1 because handle 0 is actually the 'invalid' handle */
	{
		// If the index is a nullptr, set index to i and exit
		if (BBP::Services::Interrupts::interrupts[i] == nullptr)
		{
			index = i;
			break;
		}
	}
	
	// If index is still -1, there is no space for the interrupt. In this case, throw an error since an invalid interrupt may cause infinte looping.
	if (index == -1)
	{
		BBP::Services::Throw(KERNEL_INTERRUPT_OUTOFMEMORY);
		return -1;
	}

	// Since we know that at this point the index is valid, create a new interrupt at that index
	BBP::Services::Interrupts::interrupts[index] = new BBP::Services::Interrupts::Interrupt_handle();

	// Nothing special needs to be done here. Just return the index
	return index;
}

int BBP::Services::Interrupts::Lua::createLuaInterrupt(BBP::application *owner, char *action)
{
	// Find the first index with a nullptr
	int index = -1;
	for (int i = 0; i < BBP::Services::Interrupts::Lua::lua_interrupts_count; i++)
	{
		// If the index is a nullptr, set index to i and exit
		if (BBP::Services::Interrupts::Lua::lua_interrupts[i] == nullptr)
		{
			index = i;
			break;
		}
	}

	// If index is still -1, there is no space for the interrupt. In this case, throw an error since an invalid interrupt may cause infinte looping.
	if (index == -1)
	{
		BBP::Services::Throw(KERNEL_INTERRUPT_OUTOFMEMORY);
		return -1;
	}

	// Since we know that at this point the index is valid, create a new interrupt at that index
	BBP::Services::Interrupts::Lua::lua_interrupts[index] = new BBP::Services::Interrupts::Lua::Lua_Interrupt_handle();

	// Set the relevant fields here
	BBP::Services::Interrupts::Lua::lua_interrupts[index]->functionName = action;
	do
	{
		BBP::Services::Interrupts::Lua::lua_interrupts[index]->functionNameLength++;
	} while (action[BBP::Services::Interrupts::Lua::lua_interrupts[index]->functionNameLength]);
	BBP::Services::Interrupts::Lua::lua_interrupts[index]->owner = owner;
	BBP::Services::Interrupts::Lua::lua_interrupts[index]->handle = 0;

	// Then return the index here
	return index;
}

int BBP::Services::Interrupts::Timer::createTimerInterrupt(long long delay, bool persistent)
{
	// Find the first index with a nullptr
	int index = -1;
	for (int i = 0; i < BBP::Services::Interrupts::Timer::timers_count; i++)
	{
		// If the index is a nullptr, set index to i and exit
		if (BBP::Services::Interrupts::Timer::timers[i] == nullptr)
		{
			index = i;
			break;
		}
	}

	// If index is still -1, there is no space for the interrupt. In this case, throw an error since an invalid interrupt may cause infinte looping.
	if (index == -1)
	{
		BBP::Services::Throw(KERNEL_INTERRUPT_OUTOFMEMORY);
		return -1;
	}

	// Since we know that at this point the index is valid, create a new interrupt at that index
	BBP::Services::Interrupts::Timer::timers[index] = new BBP::Services::Interrupts::Timer::Timer_Interrupt_handle();

	// Set the relevant fields here
	BBP::Services::Interrupts::Timer::timers[index]->atTime = BBP::Services::millis();
	BBP::Services::Interrupts::Timer::timers[index]->timeLeft = delay;
	BBP::Services::Interrupts::Timer::timers[index]->length = delay;
	BBP::Services::Interrupts::Timer::timers[index]->handle = 0;
	BBP::Services::Interrupts::Timer::timers[index]->persistent = persistent;

	// Then return the index here
	return index;
}

int BBP::Services::Interrupts::Lua::attachLuaInterruptTo(int index, BBP::application *owner, char *action, bool addToBacklog)
{
	int ind = BBP::Services::Interrupts::Lua::createLuaInterrupt(owner, action);
	BBP::Services::Interrupts::Lua::lua_interrupts[ind]->handle = index;
	BBP::Services::Interrupts::interrupts[index]->attachedLuaInterrupt = &BBP::Services::Interrupts::Lua::lua_interrupts[ind];
	BBP::Services::Interrupts::Lua::lua_interrupts[ind]->addToBacklog = addToBacklog;
	return ind;
}

int BBP::Services::Interrupts::Timer::attachTimerInterruptTo(int index, long long delay, bool persistent)
{
	int ind = BBP::Services::Interrupts::Timer::createTimerInterrupt(delay, persistent);
	BBP::Services::Interrupts::Timer::timers[ind]->handle = index;
	BBP::Services::Interrupts::interrupts[index]->attachedTimerInterrupt = &BBP::Services::Interrupts::Timer::timers[ind];
	return ind;
}