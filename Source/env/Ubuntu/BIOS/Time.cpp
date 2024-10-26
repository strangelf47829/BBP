#include "../include/BIOS.h"
#include <chrono>

BBP::std::time_t startup_time_micros;
BBP::std::time_t startup_time_millis;

// Get current milliseconds from system
BBP::std::time_t now_millis()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

// Get current microseconds from system
BBP::std::time_t now_micros()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

// Set 'timenow'
void Host::BIOS::initClocks()
{
	startup_time_micros = now_micros();
	startup_time_millis = now_millis();
}

// Get microseconds
BBP::std::word Host::BIOS::microseconds()
{
	return now_micros() - startup_time_micros;
}

// Get milliseconds
BBP::std::word Host::BIOS::milliseconds()
{
	return now_millis() - startup_time_millis;
}