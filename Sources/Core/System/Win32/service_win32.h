/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#pragma once


#include "../service_impl.h"
#include "API/Core/System/event.h"

class CL_Service_Win32 : public CL_Service_Impl
{
/// \name Construction
/// \{

public:
	CL_Service_Win32(CL_Service *service, const CL_String &service_name);
	virtual ~CL_Service_Win32();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	int main(int argc, char **argv);

	int run_debug(std::vector<CL_String> args);
	int run_service();
	int run_install();
	int run_uninstall();
	void print_help();

/// \}
/// \name Implementation
/// \{

private:
	static void WINAPI service_thread_main(DWORD argc, LPTSTR *argv);
	static BOOL WINAPI control_handler(DWORD ctrl_type);
	static VOID WINAPI service_ctrl(DWORD ctrl_code);

	BOOL report_status(
		DWORD current_state,
		DWORD win32_exitcode,
		DWORD wait_hint);

	CL_Event stop_event;
	CL_Event reload_event;
	bool debug_mode;
	int check_point;
	SERVICE_STATUS_HANDLE handle_service_status;
	SERVICE_STATUS service_status;
/// \}
};


