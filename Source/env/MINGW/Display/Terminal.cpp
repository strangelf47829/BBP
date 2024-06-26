#include "../include/ScreenData.h"

OS::Terminal::GTerm Host::gterm;
BBP::std::ResourceManager manager;
BBP::std::Terminal::TerminalApplication Host::terminal({ Host::tty_y, Host::tty_x }, 500, &manager, &Host::gterm);

