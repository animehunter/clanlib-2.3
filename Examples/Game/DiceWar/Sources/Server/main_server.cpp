
#include "precomp.h"
#include "server.h"

class DiceWarService : public CL_Service
{
public:
	DiceWarService() : CL_Service("Dice War Server")
	{
	}

	void service_start(std::vector<CL_String> &args)
	{
		cl_log_event("system", "Starting service..");
		thread.start(this, &DiceWarService::game_thread_main);
		cl_log_event("system", "Service started.");
	}

	void service_reload()
	{
	}

	void service_stop()
	{
		cl_log_event("system", "Stopping service..");
		stop_event.set();
		thread.join();
		cl_log_event("system", "Service stopped.");
	}

private:
	void game_thread_main()
	{
		Server server;
		server.exec(stop_event);
	}

	CL_ConsoleLogger logger;
	CL_Thread thread;
	CL_Event stop_event;
};

int main(int argc, char **argv)
{
	try
	{
		CL_SetupCore setup_core;
		CL_SetupNetwork setup_network;

		DiceWarService service;
		return service.main(argc, argv);
	}
	catch (CL_Exception &e)
	{
		CL_Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
	}
}
