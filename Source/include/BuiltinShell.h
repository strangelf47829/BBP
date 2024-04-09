#ifndef BBP_STD_BUILTINSHELL_H
#define BBP_STD_BUILTINSHELL_H

#include "License.h"

namespace BBP
{
	namespace system
	{

		// Defines the entry type for a shell builtin
		using shell_builtin_t = BBP::std::errno_t(std::size_t argc, std::c_string *argv);

		// This structure defines a 
		struct BuiltinCommand
		{
			// Name of builtin command
			std::string name;

			// Pre-computed hash
			std::hash_t hash;

			// License information
			system::appInfo info;

			// The help information
			std::conststring helpInformation;

			// Pointer to the function
			shell_builtin_t *entryPoint;

			BuiltinCommand(std::conststring nam, shell_builtin_t *func, std::conststring help)
				: name(std::String(nam)), hash(std::strhsh(nam)), info(), helpInformation(help), entryPoint(func)
			{}

			BuiltinCommand(std::conststring nam, shell_builtin_t *func)
				: BuiltinCommand(nam, func, nullptr)
			{}

		};

		// Same as Unix (except for hash)
		extern BuiltinCommand builtin_command;
		extern BuiltinCommand cd_command;
		extern BuiltinCommand exit_command;
		extern BuiltinCommand hash_command;
		extern BuiltinCommand help_command;
		extern BuiltinCommand kill_command;
		extern BuiltinCommand logout_command;
		extern BuiltinCommand type_command;
		
		// "Binaries"
		extern BuiltinCommand cp_command;
		extern BuiltinCommand echo_command;
		extern BuiltinCommand mkdir_command;
		extern BuiltinCommand ls_command;
		extern BuiltinCommand rm_command;
		extern BuiltinCommand sudo_command;

		// ELSA related stuff
		extern BuiltinCommand elsa_command;
		extern BuiltinCommand sallie_command;

		// ELF related stuff
		extern BuiltinCommand readelf_command;
		extern BuiltinCommand dumpelf_command;

		// Driver related stuff
		extern BuiltinCommand driver_command;

		// Hypervisor related stuff
		extern BuiltinCommand context_command;
		extern BuiltinCommand mount_command;
		extern BuiltinCommand joblist_command;
		extern BuiltinCommand jobset_command;
		extern BuiltinCommand hyperv_command;

		// Interrupt stuff
		extern BuiltinCommand interrupt_command;

		// RAE stuff
		extern BuiltinCommand trap_command;
		extern BuiltinCommand status_command;
		extern BuiltinCommand rae_command;

		// Daemon
		extern BuiltinCommand daemon_command;

		// Misc stuff
		extern BuiltinCommand neofetch_command;

		// Builtin command page
		extern std::PAGE<BuiltinCommand*> builtinCommands;

		// Same as Unix (except for hash)
		shell_builtin_t builtin_builtin;
		shell_builtin_t cd_builtin;
		shell_builtin_t exit_builtin;
		shell_builtin_t hash_builtin;
		shell_builtin_t help_builtin;
		shell_builtin_t kill_builtin;
		shell_builtin_t logout_builtin;
		shell_builtin_t type_builtin;

		// "Binaries"
		shell_builtin_t cp_builtin;
		shell_builtin_t echo_builtin;
		shell_builtin_t mkdir_builtin;
		shell_builtin_t ls_builtin;
		shell_builtin_t rm_builtin;
		shell_builtin_t sudo_builtin;

		// ELSA related stuff
		shell_builtin_t elsa_builtin;
		shell_builtin_t sallie_builtin;

		// ELF related stuff
		shell_builtin_t readelf_builtin;
		shell_builtin_t dumpelf_builtin;

		// Driver related stuff
		shell_builtin_t driver_builtin;

		// Hypervisor related stuff
		shell_builtin_t context_builtin;
		shell_builtin_t mount_builtin;
		shell_builtin_t joblist_builtin;
		shell_builtin_t jobset_builtin;
		shell_builtin_t hyperv_builtin;

		// Interrupt stuff
		shell_builtin_t interrupt_builtin;

		// RAE stuff
		shell_builtin_t trap_builtin;
		shell_builtin_t status_builtin;
		shell_builtin_t rae_builtin;

		// Daemon
		shell_builtin_t daemon_builtin;

		// Misc stuff
		shell_builtin_t neofetch_builtin;
	}
}

#endif