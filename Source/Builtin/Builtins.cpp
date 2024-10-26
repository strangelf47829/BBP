#include "../include/BuiltinShell.h"

BBP::system::BuiltinCommand BBP::system::builtin_command("builtin", BBP::system::builtin_builtin, "[name]");
BBP::system::BuiltinCommand BBP::system::cd_command("cd", BBP::system::cd_builtin, "[dir]");
BBP::system::BuiltinCommand BBP::system::exit_command("exit", BBP::system::exit_builtin, "[code]");
BBP::system::BuiltinCommand BBP::system::hash_command("hash", BBP::system::hash_builtin, "[-s|-S] [-l] ...");
BBP::system::BuiltinCommand BBP::system::help_command("help", BBP::system::help_builtin, "[command]");
BBP::system::BuiltinCommand BBP::system::kill_command("kill", BBP::system::kill_builtin, "[-S signal] pid");
BBP::system::BuiltinCommand BBP::system::logout_command("logout", BBP::system::logout_builtin, "");
BBP::system::BuiltinCommand BBP::system::type_command("type", BBP::system::type_builtin, "file");
BBP::system::BuiltinCommand BBP::system::cat_command("cat", BBP::system::cat_builtin, "file");

// "Binaries"
BBP::system::BuiltinCommand BBP::system::cp_command("cp", BBP::system::cp_builtin, "filename1 filename2");
BBP::system::BuiltinCommand BBP::system::echo_command("echo", BBP::system::echo_builtin, "[-e] ...");
BBP::system::BuiltinCommand BBP::system::mkdir_command("mkdir", BBP::system::mkdir_builtin, "dir");
BBP::system::BuiltinCommand BBP::system::ls_command("ls", BBP::system::ls_builtin, "[dir]");
BBP::system::BuiltinCommand BBP::system::rm_command("rm", BBP::system::rm_builtin, "file");
BBP::system::BuiltinCommand BBP::system::sudo_command("sudo", BBP::system::sudo_builtin, "");

// ELSA related stuff
BBP::system::BuiltinCommand BBP::system::elsa_command("elsa", BBP::system::elsa_builtin, "[-o output] [-c] [-g] source");
BBP::system::BuiltinCommand BBP::system::sallie_command("sallie", BBP::system::sallie_builtin, "");

// ELF related stuff
BBP::system::BuiltinCommand BBP::system::readelf_command("readelf", BBP::system::readelf_builtin, "file");
BBP::system::BuiltinCommand BBP::system::dumpelf_command("dumpelf", BBP::system::dumpelf_builtin, "file");

// Driver related stuff
BBP::system::BuiltinCommand BBP::system::driver_command("driver", BBP::system::driver_builtin, "");

// Hypervisor related stuff
BBP::system::BuiltinCommand BBP::system::context_command("context", BBP::system::context_builtin, "[-l|-s|-k] [id]");
BBP::system::BuiltinCommand BBP::system::mount_command("mount", BBP::system::mount_builtin, "[-h id] [-r id|-k] path");
BBP::system::BuiltinCommand BBP::system::joblist_command("joblist", BBP::system::joblist_builtin, "id");
BBP::system::BuiltinCommand BBP::system::jobset_command("jobset", BBP::system::jobset_builtin, "id");
BBP::system::BuiltinCommand BBP::system::hyperv_command("hyperv", BBP::system::hyperv_builtin, "[-l|-c] id");

// Interrupt stuff
BBP::system::BuiltinCommand BBP::system::interrupt_command("interrupt", BBP::system::interrupt_builtin, "[-l] [-t] [-a|-d] id");

// RAE stuff
BBP::system::BuiltinCommand BBP::system::trap_command("trap", BBP::system::trap_builtin, "signal id");
BBP::system::BuiltinCommand BBP::system::status_command("status", BBP::system::status_builtin, "[-l] id");
BBP::system::BuiltinCommand BBP::system::rae_command("rae", BBP::system::rae_builtin, "[-r file]");

// Daemon
BBP::system::BuiltinCommand BBP::system::daemon_command("daemon", BBP::system::daemon_builtin);

// Misc stuff
BBP::system::BuiltinCommand BBP::system::neofetch_command("neofetch", BBP::system::neofetch_builtin, "[-s]");

// Builtin command count
constexpr BBP::std::size_t builtinCommandCount = 31;

// Array of all builtins
BBP::system::BuiltinCommand *commands[builtinCommandCount] = {
	&BBP::system::builtin_command,
	&BBP::system::cd_command,
	&BBP::system::exit_command,
	&BBP::system::hash_command,
	&BBP::system::help_command,
	&BBP::system::kill_command,
	&BBP::system::logout_command,
	&BBP::system::type_command,
	&BBP::system::cat_command,
	&BBP::system::cp_command,
	&BBP::system::echo_command,
	&BBP::system::mkdir_command,
	&BBP::system::ls_command,
	&BBP::system::rm_command,
	&BBP::system::sudo_command,
	&BBP::system::elsa_command,
	&BBP::system::sallie_command,
	&BBP::system::readelf_command,
	&BBP::system::dumpelf_command,
	&BBP::system::driver_command,
	&BBP::system::context_command,
	&BBP::system::mount_command,
	&BBP::system::joblist_command,
	&BBP::system::jobset_command,
	&BBP::system::hyperv_command,
	&BBP::system::interrupt_command,
	&BBP::system::trap_command,
	&BBP::system::status_command,
	&BBP::system::rae_command,
	&BBP::system::daemon_command,
	&BBP::system::neofetch_command
};

// Page for builtins
BBP::std::PAGE<BBP::system::BuiltinCommand*> BBP::system::builtinCommands(builtinCommandCount, commands);
