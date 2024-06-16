#include "../include/ScreenData.h"

OS::Terminal::GTerm Host::gterm;
BBP::std::ResourceManager manager;
BBP::std::Terminal::TerminalApplication Host::terminal({ 30, 80 }, 60, &manager, &Host::gterm);

