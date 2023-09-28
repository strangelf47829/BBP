#include "../include/Kernel.h"

#include <stdio.h>

unsigned int BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(lua_State *L)
{
	// ! Checking the argument for nullptr has been omitted on purpose !
	// This is because this function can then also find any applications which have a nullptr, IE invalid applications. They could then be cleaned up.
	
	// Loop over each registered application and return the one whose applicationState matches the parameter L
	for (int i = 0; i < BBP::Services::ApplicationServices::applicationsCount; i++)
	{
		// If the application does not exist, continue.
		if (BBP::Services::ApplicationServices::applicationsVector[i] == nullptr)
			continue;

		// If the applicationState matches the parameter, return this index.
		if (BBP::Services::ApplicationServices::applicationsVector[i]->applicationState == L)
			return i;
	}

	// Not a single application exists with this lua environment. Return an invalid argument.
	return 0xFFFF; // The caller should check if this specific value is returned.
}

#pragma region GRAPHICS

int l_g_fill(lua_State *L) 
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	int b = lua_tonumber(L, -1);
	int g = lua_tonumber(L, -2);
	int r = lua_tonumber(L, -3);

	BBP::Graphics::R2D::fill(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), r, g, b);

	return 0;
}
int l_g_stroke(lua_State *L) 
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char b = lua_tonumber(L, -1);
	u_char g = lua_tonumber(L, -2);
	u_char r = lua_tonumber(L, -3);

	BBP::Graphics::R2D::stroke(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), r, g, b);

	return 0;
}

int l_g_noFill(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	BBP::Graphics::R2D::noStroke(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer));

	return 0;
}
int l_g_noStroke(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	BBP::Graphics::R2D::noFill(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer));

	return 0;
}

int l_g_strokeWeight(lua_State *L) 
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char w = lua_tonumber(L, -1);

	BBP::Graphics::R2D::strokeWeight(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), w);

	return 0;
}
int l_g_setFontSize(lua_State *L) 
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char size = lua_tonumber(L, -1);

	BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer.fontSize = size;

	return 0;
}
int l_g_setCursorPos(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char y = lua_tonumber(L, -1);
	u_char x = lua_tonumber(L, -2);

	BBP::Graphics::R2D::setCursorPos(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), x, y);

	return 0;
}

int l_g_line(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char y2 = lua_tonumber(L, -1);
	u_char x2 = lua_tonumber(L, -2);
	u_char y1 = lua_tonumber(L, -3);
	u_char x1 = lua_tonumber(L, -4);

	BBP::Graphics::R2D::Line(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), x1, y1, x2, y2);

	return 0;
}
int l_g_rect(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char h = lua_tonumber(L, -1);
	u_char w = lua_tonumber(L, -2);
	u_char y = lua_tonumber(L, -3);
	u_char x = lua_tonumber(L, -4);

	BBP::Graphics::R2D::Rect(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), x, y, w, h);

	return 0;
}
int l_g_frame(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char h = lua_tonumber(L, -1);
	u_char w = lua_tonumber(L, -2);
	u_char y = lua_tonumber(L, -3);
	u_char x = lua_tonumber(L, -4);

	BBP::Graphics::R2D::Frame(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), x, y, w, h);

	return 0;
}
int l_g_box(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char h = lua_tonumber(L, -1);
	u_char w = lua_tonumber(L, -2);
	u_char y = lua_tonumber(L, -3);
	u_char x = lua_tonumber(L, -4);

	BBP::Graphics::R2D::Box(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), x, y, w, h);

	return 0;
}

int l_g_Hrule(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char y = lua_tonumber(L, -2);
	u_char x2 = lua_tonumber(L, -3);
	u_char x1 = lua_tonumber(L, -4);

	BBP::Graphics::R2D::Hrule(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), x1, x2, y);

	return 0;
}
int l_g_Vrule(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char x = lua_tonumber(L, -2);
	u_char y2 = lua_tonumber(L, -3);
	u_char y1 = lua_tonumber(L, -4);

	BBP::Graphics::R2D::Vrule(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), y1, y2, x);

	return 0;
}

int l_g_background(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	u_char b = lua_tonumber(L, -1);
	u_char g = lua_tonumber(L, -2);
	u_char r = lua_tonumber(L, -3);

	BBP::Graphics::R2D::background(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), r, g, b);

	return 0;
}
int l_g_update(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	BBP::Graphics::Driver::drawWindow(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer));

	return 0;
}

int l_g_print(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	const char *f = lua_tostring(L, -1);

	BBP::Graphics::R2D::print(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), f);

	return 0;
}

int l_g_println(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	const char *f = lua_tostring(L, -1);

	BBP::Graphics::R2D::print(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), f);
	BBP::Graphics::R2D::print(&(BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->renderer), '\n');

	return 0;
}

#pragma endregion

#pragma region SYSTEM

/* Vars:
* screen_width: number
*	The width of the display (not window) in pixels
*
* screen_height: number
*	The height of the display (not window) in pixels
*/

int l_s_millis(lua_State *L)
{
	lua_pushnumber(L, BBP::Services::millis());
	return 1;
}

b_time_t lastTimeCalled = 0;
int l_s_delta(lua_State *L)
{
	// Get the current time
	b_time_t now = BBP::Services::millis();

	// Calculate difference
	b_time_t delta = now - lastTimeCalled;

	// Update 'lastTimeCalled'
	lastTimeCalled = now;

	// Push number to stack
	lua_pushnumber(L, delta);

	return 1;
}

int l_s_delay(lua_State *L) 
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	int delay = lua_tonumber(L, -1);

	BBP::Services::Delay(delay);

	return 0;
}

int l_s_createInterrupt(lua_State *L)
{
	lua_Number interrupt = BBP::Services::Interrupts::createInterrupt();

	lua_pushnumber(L, interrupt);

	return 1;
}
int l_s_attachTimer(lua_State *L)
{
	bool persistent = lua_toboolean(L, -1);
	int delay = lua_tonumber(L, -2);
	int attachedTo = lua_tonumber(L, -3);

	lua_Number interrupt = BBP::Services::Interrupts::Timer::attachTimerInterruptTo(attachedTo, delay, persistent);

	lua_pushnumber(L, interrupt);

	return 1;
}
int l_s_attachAction(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	bool addToBacklog = lua_toboolean(L, -1);
	const char *action = lua_tostring(L, -2);
	int attachedTo = lua_tonumber(L, -3);

	// Get the length of the string that was being input
	int length = 0;
	while (action[length++]) {}

	// Haha wouldnt want that now would we?
	if (length >= 512)
		return 0;

	// Copy string
	char *str = (char *)BBP::Services::c_alloc(length, sizeof(char));
	for (int i = 0; i < length; i++)
		str[i] = action[i];

	lua_Number interrupt = BBP::Services::Interrupts::Lua::attachLuaInterruptTo(attachedTo, BBP::Services::ApplicationServices::applicationsVector[applicationIndex], str, addToBacklog);

	lua_pushnumber(L, interrupt);

	return 1;
}

int l_s_PollInterrupt(lua_State *L)
{
	int pollee = lua_tonumber(L, -1);

	int pollresult = (int)BBP::Services::Interrupts::Poll(pollee);

	lua_pushboolean(L, pollresult);

	return 1;
}
int l_s_ClearInterrupt(lua_State *L)
{
	int clearee = lua_tonumber(L, -1);

	BBP::Services::Interrupts::Clear(clearee, true);

	return 0;
}

int l_s_silentClearInterrupt(lua_State *L)
{
	int clearee = lua_tonumber(L, -1);

	BBP::Services::Interrupts::Clear(clearee, false);

	return 0;
}

int l_s_TimeLeft(lua_State *L)
{
	int pollee = lua_tonumber(L, -1);

	int timeLeft = BBP::Services::Interrupts::Timer::timers[pollee]->timeLeft;

	lua_pushnumber(L, timeLeft);

	return 1;
}

int l_s_updateInterrupts(lua_State *L)
{
	BBP::Services::Interrupts::updateInterrupts();
	return 0;
}

#pragma endregion

#pragma region APPLICATION

int l_a_noAudit(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	// Disable audit
	BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->auditActive = false;

	return 0;
}

int l_a_Audit(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	// Enable audit
	BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->auditActive = true;
	//BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->lastAudit = BBP::Services::millis();
	
	return 0;
}

int l_a_setTimeout(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	int timeout = lua_tonumber(L, -1);

	// Update timeout
	BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->timeout = timeout;

	return 0;
}

int l_a_echo(lua_State *L)
{
	unsigned int applicationIndex = BBP::Services::ApplicationServices::Lua::fetchIndexForLuaInstance(L);
	if (applicationIndex == 0xFFFF) // Invalid argument
		return 0;

	// Set the last time responded to now
	BBP::Services::ApplicationServices::applicationsVector[applicationIndex]->lastAudit = BBP::Services::millis();

	return 0;
}

#pragma endregion