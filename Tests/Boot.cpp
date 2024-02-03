#include "../Source/include/stdio.h"
#include "../Source/include/Signal.h"
#include "../Source/include/Resources.h"
#include "../Source/include/ELF.h"
#include "../Source/include/FileSys.h"
#include "../Source/include/Lex.h"
#include "../Source/include/CPPApplications.h"
#include "../Source/include/Executable.h"
#include "../Source/include/Time.h"
#include "../Source/include/Graphics.h"
#include "../Source/include/Locale.h"

#include "../build/include/SDL.h"
#include "../Source/include/OS.h"


int BBP::boot_main(int argc, char **argv)
{   


    BBP::std::window *w = &BBP::std::kernelDisplay;

    std::R2D::fill(w, 0x00FF00FF);
    std::R2D::setCursorPos(w, 50, 50);
    w->fontSize = 6;

    std::R2D::print(w,"Hello!\n");

    std::R2D::fill(w, 0xFF00FFFF);
    std::R2D::print(w, "Pretty nice, huh?\n");

    BBP::std::Driver::drawWindow(w);

    return 0;
}