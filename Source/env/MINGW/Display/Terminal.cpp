#include "../include/ScreenData.h"

OS::Terminal::GTerm Host::gterm;
BBP::std::ResourceManager manager;
BBP::std::Terminal::TerminalApplication Host::terminal({ 40, 110 }, 60, &manager, &Host::gterm);

