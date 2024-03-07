#include "../include/stdio.h"
#include "../include/Signal.h"
#include "../include/Resources.h"
#include "../include/ELF.h"
#include "../include/Kernel.h"
#include "../include/Lex.h"
#include "../include/CPPApplications.h"
#include "../include/Executable.h"
#include "../include/System.h"
#include "../include/Graphics.h"
#include "../include/OS.h"
#include "../include/System.h"

namespace BBP
{
       

    int shell(BBP::std::string &str)
    {
        // Get the first command line arg
        std::c_string command = std::read_a(&str, 0);
        
        // Then go until space
        for (std::index_t index = 0; index < str.dataSize; index++)
        {
            // Read char at index index
            std::string_element c = std::read(&str, index);

            // Check if 'c' is a space
            if (c != ' ')
                continue;
            
            str.data[index] = 0;
            break;

        }

        // If string is just empty, do nothing
        if (command[0] == 0)
            return 0;

        // Create paths
        std::PATH procPath("/proc/exec/");
        procPath.makeAbsolutePath(system::kernelSS()->activeContext->workingDirectory);
        std::PATH executable(command);
        executable.makeAbsolutePath(&procPath);

        

        try 
        {
            // Create file, if it exists, no error will be thrown. Otherwise, an error will be thrown.
            std::FILE executableFile(executable);

            // The file exists. Check if the page has the correct magic numbers.
            std::mem_t magic0 = std::read_r(executableFile.data()->fileData.page, BBP_EXEC_MAGIC0IDX);
            std::mem_t magic1 = std::read_r(executableFile.data()->fileData.page, BBP_EXEC_MAGIC1IDX);
            std::mem_t magic2 = std::read_r(executableFile.data()->fileData.page, BBP_EXEC_MAGIC2IDX);
            std::mem_t magic3 = std::read_r(executableFile.data()->fileData.page, BBP_EXEC_MAGIC3IDX);

            // If the magic matches up, this file is an executable.
            bool isMachineCodeExecutable = (magic0 == BBP_EXEC_MAGIC0) && (magic1 == BBP_EXEC_MAGIC1) && (magic2 == BBP_EXEC_MAGIC2) && (magic3 == BBP_EXEC_MAGIC3);
            
            if (isMachineCodeExecutable)
            {
                // This file is an executable in machine code format. Extract the function from the page, and execute.
                std::executable_main _entry = ((std::executable_main *)executableFile.data()->fileData.page->raw)[0];
                std::execute(_entry, 0, nullptr);
            }
            else
            {
                std::printf("%s: Exec format error.\n", command);
            }

            return system::kernelSS()->activeContext->__errno;

        }
        catch (std::except const &e)
        {
            switch (system::kernelSS()->activeContext->__errno)
            {
            case ENOENT:
                std::printf("%s: command not found.\n", command);
                system::kernelSS()->activeContext->__errno = 0;
                return -1;
                break;
            default:
                // Unkown error, rethrow
                throw;
            }
        }

    }

    int shell(const char *ss)
    {
        // Get string length
        std::size_t strL = std::strlen(ss);

        // Create new string of that size
        std::string str(strL + 1, (std::string_element *)BBP::system::kernelSS()->activeContext->activemem->calloc(strL + 1, sizeof(std::string_element)));

        // Copy string
        std::strcpy(&str, ss);

        // save shell return value
        int s = shell(str);

        // Free string mem
        BBP::system::kernelSS()->activeContext->activemem->free(str.data);

        // Return shell result
        return s;

    }

    BBP::std::static_string<509> linePage;
    BBP::std::Stack<BBP::std::string_element> line(&linePage, linePage.dataSize);

    int shell_main(int argc, char **argv)
    {
        // This surpresses the g++ unused parameters warnings
        argc = argc;
        argv = argv;

        int count = 0;
        bool doLoop = true;

        shell("elsa");
        return 0;

        while (true)
        {
            std::printf("\e[0;92m" HOSTNAME "\e[0;37m:\e[1;34m%s\e[0;37m$ ", system::kernelSS()->activeContext->workingDirectory->relName());

            while (doLoop)
            {
                std::getC();
                if (system::kernelSS()->activeContext->STDIN.atElement)
                {
                    char c = 0;
                    char del = 0x08;
                    system::kernelSS()->activeContext->STDIN >> c;

                    //std::printf("0x%02x ", c);

                    switch (c)
                    {
                    case 0x0a:
                    case 0x0d:
                        doLoop = false;
                        
                        break;

                    case 12:
                        // Screen clear
                        std::printf("\033[2J\033[1;1H");
                        std::printf("\e[0;92m" HOSTNAME "\e[0;37m:\e[1;34m%s\e[0;37m$ ", system::kernelSS()->activeContext->workingDirectory->relName());
                        break;

                    case 0x7f:
                        
                        if (line.atElement)
                        {
                            system::kernelSS()->activeContext->STDOUT <<= del;
                            system::kernelSS()->activeContext->STDOUT <<= ' ';
                            system::kernelSS()->activeContext->STDOUT <<= del;
                            line--;
                        }

                        break;
                    default:
                        system::kernelSS()->activeContext->STDOUT <<= c;
                        line << c;
                        break;
                    }
                }
            }
            line << '\0';
            std::printf("\n");
            shell(linePage);
            line.atElement = 0;
            count++;
            doLoop = true;
        }


        return 0;
    }
}