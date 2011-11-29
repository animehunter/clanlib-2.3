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
**    Harry Storbacka
*/

#pragma once


#include "API/Display/api_display.h"
#include "API/Core/System/event.h"
#include "API/Core/System/thread_local_storage.h"
#include <vector>
#include <WinNT.h>

class CL_Win32Window;

class CL_API_DISPLAY CL_DisplayMessageQueue_Win32
{
/// \name Construction
/// \{
public:
	CL_DisplayMessageQueue_Win32();
	~CL_DisplayMessageQueue_Win32();

	static CL_DisplayMessageQueue_Win32 message_queue;

/// \}
/// \name Operations
/// \{
public:
	int wait(const std::vector<CL_Event> &events, int timeout);

	void add_client(CL_Win32Window *window);
	void remove_client(CL_Win32Window *window);

	static bool should_apply_vista_x64_workaround() { return ptrSetProcessUserModeExceptionPolicy == 0 || ptrGetProcessUserModeExceptionPolicy == 0; }

 /// \}
 /// \name Implementation
 /// \{

private:
	void process_message();

	class ThreadData : public CL_ThreadLocalStorageData
	{
	public:
		ThreadData() : already_flagged(false) { }

		std::vector<CL_Win32Window *> windows;
		bool already_flagged;
	};

	CL_SharedPtr<ThreadData> get_thread_data();

	#define WIN32_PROCESS_CALLBACK_FILTER_ENABLED 0x1
	typedef BOOL (WINAPI FuncSetProcessUserModeExceptionPolicy)(DWORD dwFlags);
	typedef BOOL (WINAPI FuncGetProcessUserModeExceptionPolicy)(LPDWORD lpFlags);
	static HMODULE moduleKernel32;
	static FuncSetProcessUserModeExceptionPolicy *ptrSetProcessUserModeExceptionPolicy;
	static FuncGetProcessUserModeExceptionPolicy *ptrGetProcessUserModeExceptionPolicy;
 /// \}
};

typedef struct _EXCEPTION_REGISTRATION_RECORD
{
	struct _EXCEPTION_REGISTRATION_RECORD *Next;
	PEXCEPTION_ROUTINE Handler;
} EXCEPTION_REGISTRATION_RECORD, *PEXCEPTION_REGISTRATION_RECORD;

// Workaround for the KB976038 bug on unpatched systems.
class CL_SEHCatchAllWorkaround
{
public:
	CL_SEHCatchAllWorkaround()
	: was_patched(CL_DisplayMessageQueue_Win32::should_apply_vista_x64_workaround()), old_exception_handler(0)
	{
		if (was_patched)
		{
			// remove all exception handler with exception of the default handler
			NT_TIB *tib = get_tib();
			old_exception_handler = tib->ExceptionList;
			while (tib->ExceptionList->Next != (_EXCEPTION_REGISTRATION_RECORD*)-1)
				tib->ExceptionList = tib->ExceptionList->Next;
		}
	}

	void unpatch()
	{
		if (was_patched)
		{
			// restore old exception handler
			NT_TIB *tib = get_tib();
			tib->ExceptionList = old_exception_handler;
		}
	}

private:
	// get thread information block
	NT_TIB *get_tib()
	{
		NT_TIB * pTib = (NT_TIB *)NtCurrentTeb();
		return pTib;
		//#ifdef _MSC_VER
		//return (NT_TIB *)__readfsdword(0x18);
		//#else
		//NT_TIB *tib = 0;
		//__asm
		//{
		//	mov EAX, FS:[18h]
		//	mov [tib], EAX
		//}
		//return tib;
		//#endif
	}

	bool was_patched;
	_EXCEPTION_REGISTRATION_RECORD* old_exception_handler;
};

