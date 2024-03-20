#include "../include/stdio.h"
#include "../include/Signal.h"
#include "../include/Resources.h"
#include "../include/ELF.h"
#include "../include/FileSys.h"
#include "../include/Lex.h"
#include "../include/CPPApplications.h"
#include "../include/Executable.h"
#include "../include/Time.h"
#include "../include/Graphics.h"
#include "../include/Locale.h"

#include "../include/OS.h"

#include "../include/Kernel.h"


int BBP::boot_main(int argc, char **argv)
{   


    BBP::std::window *w = &BBP::system::kernelSS()->activeContext->display;

    std::R2D::fill(*w, 0x00FF00FF);
    std::R2D::setCursorPos(*w, 50, 50);
    w->fontSize = 6;

    std::R2D::print(*w,"Hello!\n");

    std::R2D::fill(*w, 0xFF00FFFF);
    std::R2D::print(*w, "Pretty nice, huh?\n");

    return 0;
}