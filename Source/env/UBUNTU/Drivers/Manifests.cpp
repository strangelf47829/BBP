#include "../include/Environment.h"
#include "../include/FileDriver.h"
#include "../include/KeyboardDriver.h"
#include "../include/ScreenDriver.h"
#include "../../../include/UEFI.h"

namespace Keyboard = Environment::Drivers::keyboard;
namespace Screen = Environment::Drivers::screen;
namespace FS = Environment::Drivers::Filesystem;

BBP::system::HardwareCmd BBP::system::keyboardCMD[4] = { Keyboard::connectKeyboard, Keyboard::disconnectKeyboard, Keyboard::enableKeyboardInput, Keyboard::disableKeyboardInput };
Environment::Drivers::Manifest Environment::Drivers::keyboardManifest = { {Keyboard::sendDataToKeyboard, Keyboard::receiveDataFromKeyboard, Keyboard::receiveKeyboardMetadata}, 4, BBP::system::keyboardCMD, Keyboard::loadKeyboardDriver };

BBP::system::HardwareCmd BBP::system::screenCMD[2] = { Screen::connectToScreen, Screen::clearScreen };
Environment::Drivers::Manifest Environment::Drivers::screenManifest = { {Screen::sendDataToScreen, Screen::receiveDataScreen, Screen::receiveScreenMetadata}, 2, BBP::system::screenCMD, Screen::loadScreenDriver };

BBP::system::HardwareCmd BBP::system::fileSystemCMD[4] = { FS::initializeFilesystem, FS::deinitializeFilesystem, FS::setModeLoadPath, FS::queryFileMetadata };
Environment::Drivers::Manifest Environment::Drivers::fileManifest = { {FS::sendDataToFileSystem, FS::receiveDataFromFileSystem, FS::receiveFileMetadata}, 4, BBP::system::fileSystemCMD, FS::loadFileSystem };


BBP::std::size_t BBP::system::keyboardCMDCount = 4;
BBP::std::size_t BBP::system::screenCMDCount = 2;
BBP::std::size_t BBP::system::fileSystemCMDCount = 2;

BBP::system::HardwareAction BBP::system::keyboardActions[3] = { Environment::Drivers::keyboardManifest.actions[0],Environment::Drivers::keyboardManifest.actions[1],Environment::Drivers::keyboardManifest.actions[2] };
BBP::system::HardwareAction BBP::system::screenActions[3] = { Environment::Drivers::screenManifest.actions[0],Environment::Drivers::screenManifest.actions[1],Environment::Drivers::screenManifest.actions[2] };
BBP::system::HardwareAction BBP::system::fileSystemActions[3] = { Environment::Drivers::fileManifest.actions[0],Environment::Drivers::fileManifest.actions[1],Environment::Drivers::fileManifest.actions[2] };