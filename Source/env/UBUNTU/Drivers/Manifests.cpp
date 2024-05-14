#include "../include/Environment.h"
#include "../include/FileDriver.h"
#include "../include/KeyboardDriver.h"
#include "../include/ScreenDriver.h"
#include "../include/SystemDriver.h"
#include "../../../include/UEFI.h"

namespace Keyboard = Environment::Drivers::keyboard;
namespace Screen = Environment::Drivers::screen;
namespace FS = Environment::Drivers::Filesystem;
namespace System = Environment::Drivers::System;

BBP::system::HardwareCmd BBP::system::keyboardCMD[4] = { Keyboard::connectKeyboard, Keyboard::disconnectKeyboard, Keyboard::enableKeyboardInput, Keyboard::disableKeyboardInput };
Environment::Drivers::Manifest Environment::Drivers::keyboardManifest = { {Keyboard::sendDataToKeyboard, Keyboard::receiveDataFromKeyboard, Keyboard::receiveKeyboardMetadata}, 4, BBP::system::keyboardCMD, Keyboard::loadKeyboardDriver };

BBP::system::HardwareCmd BBP::system::screenCMD[2] = { Screen::connectToScreen, Screen::clearScreen };
Environment::Drivers::Manifest Environment::Drivers::screenManifest = { {Screen::sendDataToScreen, Screen::receiveDataScreen, Screen::receiveScreenMetadata}, 2, BBP::system::screenCMD, Screen::loadScreenDriver };

BBP::system::HardwareCmd BBP::system::fileSystemCMD[14] = { FS::initializeFilesystem, FS::deinitializeFilesystem, FS::setModeLoadPath, FS::queryFileMetadata, FS::getFileType, FS::openFile, FS::closeFile, FS::setReadMode, FS::setWriteMode, FS::pathInspect, FS::stepInspect, FS::emitNameInspect, FS::emitTypeInspect, FS::canStepInspect };
Environment::Drivers::Manifest Environment::Drivers::fileManifest = { {FS::sendDataToFileSystem, FS::receiveDataFromFileSystem, FS::receiveFileMetadata}, 14, BBP::system::fileSystemCMD, FS::loadFileSystem };

BBP::system::HardwareCmd BBP::system::systemCMD[2] = { System::connectSystem, System::disconnectSystem };
Environment::Drivers::Manifest Environment::Drivers::systemManifest = { {System::sendDataToSystem, System::receiveDataFromSystem, System::receiveSystemMetadata}, 2, BBP::system::systemCMD, System::loadSystemDriver };

BBP::std::size_t BBP::system::keyboardCMDCount = 4;
BBP::std::size_t BBP::system::screenCMDCount = 2;
BBP::std::size_t BBP::system::fileSystemCMDCount = 14;
BBP::std::size_t BBP::system::systemCMDCount = 2;

BBP::system::HardwareAction BBP::system::keyboardActions[3] = { Environment::Drivers::keyboardManifest.actions[0],Environment::Drivers::keyboardManifest.actions[1],Environment::Drivers::keyboardManifest.actions[2] };
BBP::system::HardwareAction BBP::system::screenActions[3] = { Environment::Drivers::screenManifest.actions[0],Environment::Drivers::screenManifest.actions[1],Environment::Drivers::screenManifest.actions[2] };
BBP::system::HardwareAction BBP::system::fileSystemActions[3] = { Environment::Drivers::fileManifest.actions[0],Environment::Drivers::fileManifest.actions[1],Environment::Drivers::fileManifest.actions[2] };
BBP::system::HardwareAction BBP::system::systemActions[3] = { Environment::Drivers::systemManifest.actions[0],Environment::Drivers::systemManifest.actions[1],Environment::Drivers::systemManifest.actions[2] };
