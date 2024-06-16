#include "../../../include/ScreenDriver.h"
#include "../../../../include/drvcmd.h"
#include "../../include/ScreenData.h"

// Reduces amount of characters needed to type
namespace TTY = Host::Drivers::Screen;

// Amount of screen commands
constexpr BBP::std::size_t screenCMDsize = 4;

// List of screen commands and actions
BBP::system::HardwareCmd screenCMD[screenCMDsize] = { TTY::connectToScreen, TTY::disconnectFromScreen, TTY::clearScreen, TTY::printSplash };
BBP::system::HardwareAction screenActions[3] = { TTY::sendDataToScreen, TTY::receiveDataScreen, TTY::receiveScreenMetadata };

void Host::Drivers::Screen::loadScreenDriver(BBP::system::DeviceDriver &driver)
{
	// Set commands
	driver.hardwareDriver.setHandleData(screenActions, screenCMDsize, screenCMD);
}