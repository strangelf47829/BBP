#include "../include/Taskpool.h"

// Create a static page for this scheduler
BBP::std::STATIC_PAGE<BBP::std::TaskFlowInterface *, 32> taskInterfaces;
BBP::std::TaskPool OS::Scheduler::schedulerPool(taskInterfaces);