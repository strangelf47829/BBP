#include "../include/Environment.h"
#include "../include/FileDriver.h"
#include "../include/KeyboardDriver.h"
#include "../include/ScreenDriver.h"
#include "../include/SystemDriver.h"
#include "../../../include/EFI.h"

namespace Keyboard = Environment::Drivers::keyboard;
namespace Screen = Environment::Drivers::screen;
namespace FS = Environment::Drivers::Filesystem;
namespace System = Environment::Drivers::System;

// Constant expressions
constexpr BBP::std::size_t keyboardCMDsize = 4;
constexpr BBP::std::size_t screenCMDsize = 4;
constexpr BBP::std::size_t fileSystemCMDsize = 14;
constexpr BBP::std::size_t systemCMDsize = 6;

BBP::system::HardwareCmd BBP::system::keyboardCMD[keyboardCMDsize] = { Keyboard::connectKeyboard, Keyboard::disconnectKeyboard, Keyboard::keyCount, Keyboard::captureKey };
Environment::Drivers::Manifest Environment::Drivers::keyboardManifest = { {Keyboard::sendDataToKeyboard, Keyboard::receiveDataFromKeyboard, Keyboard::receiveKeyboardMetadata}, keyboardCMDsize, BBP::system::keyboardCMD, Keyboard::loadKeyboardDriver };

BBP::system::HardwareCmd BBP::system::screenCMD[screenCMDsize] = { Screen::connectToScreen, Screen::disconnectFromScreen, Screen::clearScreen, Screen::printSplash };
Environment::Drivers::Manifest Environment::Drivers::screenManifest = { {Screen::sendDataToScreen, Screen::receiveDataScreen, Screen::receiveScreenMetadata}, screenCMDsize, BBP::system::screenCMD, Screen::loadScreenDriver};

BBP::system::HardwareCmd BBP::system::fileSystemCMD[fileSystemCMDsize] = { FS::initializeFilesystem, FS::deinitializeFilesystem, FS::setModeLoadPath, FS::queryFileMetadata, FS::getFileType, FS::openFile, FS::closeFile, FS::setReadMode, FS::setWriteMode, FS::pathInspect, FS::stepInspect, FS::emitNameInspect, FS::emitTypeInspect, FS::canStepInspect };
Environment::Drivers::Manifest Environment::Drivers::fileManifest = { {FS::sendDataToFileSystem, FS::receiveDataFromFileSystem, FS::receiveFileMetadata}, fileSystemCMDsize, BBP::system::fileSystemCMD, FS::loadFileSystem };

BBP::system::HardwareCmd BBP::system::systemCMD[systemCMDsize] = { System::connectSystem, System::disconnectSystem, System::handleSystemCall, System::initClock, System::getClockMillis, System::getClockMicros };
Environment::Drivers::Manifest Environment::Drivers::systemManifest = { {System::sendDataToSystem, System::receiveDataFromSystem, System::receiveSystemMetadata}, systemCMDsize, BBP::system::systemCMD, System::loadSystemDriver };

BBP::std::size_t BBP::system::keyboardCMDCount = keyboardCMDsize;
BBP::std::size_t BBP::system::screenCMDCount = screenCMDsize;
BBP::std::size_t BBP::system::fileSystemCMDCount = screenCMDsize;
BBP::std::size_t BBP::system::systemCMDCount = screenCMDsize;

BBP::system::HardwareAction BBP::system::keyboardActions[3] = { Environment::Drivers::keyboardManifest.actions[0],Environment::Drivers::keyboardManifest.actions[1],Environment::Drivers::keyboardManifest.actions[2] };
BBP::system::HardwareAction BBP::system::screenActions[3] = { Environment::Drivers::screenManifest.actions[0],Environment::Drivers::screenManifest.actions[1],Environment::Drivers::screenManifest.actions[2] };
BBP::system::HardwareAction BBP::system::fileSystemActions[3] = { Environment::Drivers::fileManifest.actions[0],Environment::Drivers::fileManifest.actions[1],Environment::Drivers::fileManifest.actions[2] };
BBP::system::HardwareAction BBP::system::systemActions[3] = { Environment::Drivers::systemManifest.actions[0],Environment::Drivers::systemManifest.actions[1],Environment::Drivers::systemManifest.actions[2] };
