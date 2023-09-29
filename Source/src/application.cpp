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

BBP::application **BBP::Services::ApplicationServices::applicationsVector = nullptr;
unsigned int BBP::Services::ApplicationServices::applicationsCount = 0;

void BBP::Services::ApplicationServices::Lua::loadApplicationLibrary(BBP::application *app)
{
	static const luaL_Reg test[]
	{

		{"echo", l_a_echo},

		{"noAudit", l_a_noAudit},
		{"audit", l_a_Audit},
		{"setTimeout", l_a_setTimeout},
		{NULL, NULL}
	};

	lua_newtable(app->applicationState);

	luaL_setfuncs(app->applicationState, test, 0);
	lua_setglobal(app->applicationState, "app");
}

void BBP::Services::ApplicationServices::Lua::loadGraphicsLibrary(BBP::application *app)
{
	static const luaL_Reg test[]
	{
		{"fill", l_g_fill},
		{"stroke", l_g_stroke},

		{"noFill", l_g_noFill},
		{"noStroke", l_g_noStroke},

		{"strokeWeight", l_g_strokeWeight},
		{"setFontSize", l_g_setFontSize},
		{"setCursorPos", l_g_setCursorPos},

		{"line", l_g_line},
		{"rect", l_g_rect},
		{"frame", l_g_frame},
		{"box", l_g_box},

		{"Hrule", l_g_Hrule},
		{"Vrule", l_g_Vrule},

		{"background", l_g_background},
		{"update", l_g_update},

		{"print", l_g_print},
		{"println", l_g_println},
		{NULL, NULL}
	};

	lua_newtable(app->applicationState);

	luaL_setfuncs(app->applicationState, test, 0);
	lua_setglobal(app->applicationState, "graphics");
}

void BBP::Services::ApplicationServices::Lua::loadIOLibrary(BBP::application *app)
{

}

void BBP::Services::ApplicationServices::Lua::loadSystemLibrary(BBP::application *app)
{

	static const luaL_Reg test[]
	{
		{"millis", l_s_millis},
		{"delta", l_s_delta},
		{"delay", l_s_delay},

		{"createInterrupt", l_s_createInterrupt},
		{"attachTimer", l_s_attachTimer},
		{"attachAction", l_s_attachAction},

		{"Poll", l_s_PollInterrupt},
		{"Clear", l_s_ClearInterrupt},
		{"silentClear", l_s_silentClearInterrupt},

		{"TimeLeft", l_s_TimeLeft},
		{"updateInterrupts", l_s_updateInterrupts},
		{NULL, NULL}
	};

	lua_newtable(app->applicationState);

	luaL_setfuncs(app->applicationState, test, 0);
	lua_setglobal(app->applicationState, "system");
}

void BBP::Services::ApplicationServices::Lua::loadLuaLibraries(BBP::application *app)
{
	// Call every function that loads libraries
	loadApplicationLibrary(app);
	loadGraphicsLibrary(app);
	loadIOLibrary(app);
	loadSystemLibrary(app);
}

bool BBP::Services::ApplicationServices::Lua::Execute(BBP::application *app, char *str, bool autoClear)
{
	// if application is invalid, return
	if (app == nullptr)
		return false;

	// if lua environment is invalid, return
	if (app->applicationState == nullptr)
		return false;

	// Doing something while the interpreter is busy is a bad idea. Only execute when the interpreter is not busy.
	if (!app->busy)
	{
		// Flag the lua state
		app->busy = true;

		// Set auditing data
		app->executingSince = BBP::Services::millis();
		app->lastAudit = app->executingSince;

		// Execute string
		luaL_dostring(app->applicationState, str);

		// The lua state is no longer busy
		app->busy = false;

		// Clear any backlogged functions if autoClear is enabled
		if(autoClear)
			return BBP::Services::ApplicationServices::Lua::ClearBackLog(app);


		return true;
	}
	else
	{
		return false;
	}
}

int BBP::Services::ApplicationServices::createApplicationFromFile(BBP::IO::File::FILE_HANDLE handle)
{
	// Create a new application
	application *app = new application
		{
			BBP::Services::ApplicationServices::applicationsCount,						/*PID*/
			(char *)(BBP::Services::c_alloc(handle.fileNameLength, sizeof(char))),		/*Name*/
			handle.fileNameLength,														/*Name length*/
			BBP::Graphics::window(),													/*Renderer*/
			nullptr																		/* Lua state*/
		};

	// Create a new lua state and load the libraries
	app->applicationState = luaL_newstate();
	BBP::Services::ApplicationServices::Lua::loadLuaLibraries(app);

	// TODO initialize lua values such as screen_width


	// Do the entire text file
	BBP::Services::ApplicationServices::Lua::Execute(app, (char*)handle.fileVector, false);

	// Copy over the name from the handle to the application name
	for (int i = 0; i < handle.fileNameLength; i++)
		app->name[i] = handle.fileName[i];

	// Register the new application
	BBP::Services::ApplicationServices::registerApplication(app);

	// Return the created applications PID
	return app->PID;
}

bool BBP::Services::ApplicationServices::closeApplication(BBP::application *app, bool forceQuit)
{
	// Cannot close app if already closed. Return true to prevent any issues
	if (app->applicationState == nullptr)
		return true;

	// Call the application destructor, then delete lua
	if(!forceQuit)
		BBP::Services::ApplicationServices::Lua::Execute(app, "close()", false);
	lua_close(app->applicationState);

	// For each interrupt, clear it if it is associated with this application
	for (int i = 0; i < BBP::Services::Interrupts::Lua::lua_interrupts_count; i++)
	{
		// If interrupt invalid, skip
		if (BBP::Services::Interrupts::Lua::lua_interrupts[i] == nullptr)
			continue;

		// If interrupt is not about this application, skip
		if (BBP::Services::Interrupts::Lua::lua_interrupts[i]->owner != app)
			continue;

		// Clear this interrupt
		BBP::Services::Interrupts::Lua::Clear(i);
	}

	// We can now also free the application name data, since it will no longer be used
	BBP::Services::Free(app->name);

	// Since the application 'no longer exists', we might as well delete it from the list.
	BBP::Services::ApplicationServices::deleteApplication(app);
	app->applicationState = nullptr;

	// The application has now closed. Return true
	return true;
}

bool BBP::Services::ApplicationServices::deleteApplication(BBP::application *app)
{

	// If there is no application present, return here
	if (app == nullptr)
		return false; // Invalid application

	// Free the backlog
	if (app->backlog != nullptr)
		BBP::Services::Free(app->backlog);


	// Free the window stuff
	// TODO implement

	// Delete the application from memory
	delete(app);

	// The application has been deleted.
	return true;
}

bool BBP::Services::ApplicationServices::registerApplication(BBP::application *app)
{
	// If the parameter is invalid, exit
	if (app == nullptr)
		return false;

	// Create a new array 
	BBP::application **appList = (BBP::application **)BBP::Services::c_alloc(BBP::Services::ApplicationServices::applicationsCount + 1, sizeof(BBP::application *));

	// Copy over all the data from the old array to the new one
	for (int i = 0; i < BBP::Services::ApplicationServices::applicationsCount; i++)
		appList[i] = BBP::Services::ApplicationServices::applicationsVector[i];

	// Set the last element to the parameter
	appList[BBP::Services::ApplicationServices::applicationsCount++] = app;

	// Free the old array
	BBP::Services::Free(BBP::Services::ApplicationServices::applicationsVector);

	// Set the global list to the new array
	BBP::Services::ApplicationServices::applicationsVector = appList;

	// New app has been registered
	return true;
}

void BBP::Services::ApplicationServices::Lua::Audit(BBP::application *app, b_time_t now)
{
	// If the application is invalid, return.
	if (app == nullptr)
		return;

	// If the app has no lua environment, return.
	if (app->applicationState == nullptr)
		return;

	// If the app is not busy, return.
	if (!app->busy)
		return;

	// If the app has audit disabled, return.
	if (!app->auditActive)
		return;

	// If the timeout setting is set to 0 (disabled), return
	if (app->timeout == 0)
		return;

	// Get the difference in time between now and the last time the app has responded.
	b_time_t difference = now - app->lastAudit;


	// If the difference is not greater than the timeout, return
	if (difference < app->timeout)
		return;

	// At this point, the application has not responded in time. Force quit application

	// Display crash screen
	BBP::Graphics::window *appRenderer = &app->renderer;
	appRenderer->fontSize = 1;
	BBP::Graphics::R2D::background(appRenderer, 255, 255, 255);
	BBP::Graphics::R2D::fill(appRenderer, 0, 0, 255);
	BBP::Graphics::R2D::setCursorPos(appRenderer, 10, 10);


	BBP::Graphics::R2D::print(appRenderer, "Application has crashed because it was unresponsive.\n");

	BBP::Graphics::Driver::drawWindow(appRenderer);

	BBP::Services::ApplicationServices::closeApplication(app, true);
}

void BBP::Services::ApplicationServices::Lua::AuditAll()
{
	// fetch the current time
	b_time_t now = BBP::Services::millis();

	// loop over each application and audit
	for (int i = 0; i < BBP::Services::ApplicationServices::applicationsCount; i++)
	{
		// If the application is invalid, continue
		if (BBP::Services::ApplicationServices::applicationsVector[i] == nullptr)
			continue;
		else // Otherwise, audit the application
			BBP::Services::ApplicationServices::Lua::Audit(BBP::Services::ApplicationServices::applicationsVector[i], now);
	}
}

bool BBP::Services::ApplicationServices::Lua::ClearBackLog(BBP::application *app) 
{

	// If application is invalid return
	if (app == nullptr)
		return false;

	// If application backlog is invalid return
	if (app->backlog == nullptr)
		return false;

	// If application backlog length is 0 return
	if (app->backlog_c == 0)
		return true;

	/*
	printf("clearing backlog:\n-------\n", app->backlog_len);
	for (int i = 0; i < app->backlog_len; i++)
	{
		if (app->backlog[i] != BBP::Encodings::Roman::returncarriage)
			printf("%c", app->backlog[i]);
		else
			printf("\n");
	}
	printf("\n-------\n");*/

	if (BBP::Services::ApplicationServices::Lua::Execute(app, app->backlog, false))
	{
		// Success
		// Now reset backlog
		BBP::Services::Free(app->backlog);
		app->backlog = nullptr;

		app->backlog_c = 0;
		app->backlog_len = 0;

		return true;
	}

	// Backlog has not been cleared
	return false;
}

void BBP::Services::ApplicationServices::Lua::AddBackLog(BBP::application *app, char *action)
{
	// If application is invalid, return
	if (app == nullptr)
		return;

	// Calculate string length
	int actionLength = 0;
	while (action[actionLength++]) {}

	// Create new string
	char *newBacklog = (char *)BBP::Services::c_alloc(app->backlog_len + actionLength + 1, sizeof(char));



	// Copy over data, except for terminating null
	for (int i = 0; i < app->backlog_len; i++)
		if (app->backlog[i] != 0)
			newBacklog[i] = app->backlog[i];
		else
			newBacklog[i] = BBP::Encodings::Roman::returncarriage;

	// Now copy over new data, including terminating null
	for (int i = app->backlog_len; i < app->backlog_len + actionLength; i++)
		if (action[i - app->backlog_len] != 0)
			newBacklog[i] = action[i - app->backlog_len];
		else
			newBacklog[i] = BBP::Encodings::Roman::returncarriage;

	// Last byte is always null
	newBacklog[app->backlog_len + actionLength];

	// If the backlog is allocated, free it
	if (app->backlog != nullptr)
		BBP::Services::Free(app->backlog);

	// Set new data
	app->backlog = newBacklog;
	app->backlog_c++;
	app->backlog_len += actionLength + 1;
}