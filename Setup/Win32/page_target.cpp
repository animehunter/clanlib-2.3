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

#include "precomp.h"
#include "page_target.h"
#include "resource.h"
#include <shlobj.h>

typedef HRESULT (WINAPI *FolderPathFunc)(HWND, LPTSTR, int, BOOL);

PageTarget::PageTarget()
{
	target_version = 900;
	include_unicode = false;
	include_sse2 = true;
	include_intrinsics = true;
	include_mtdll = false;
	include_dll = false;
	include_x64 = false;
	enable_debug_optimize = false;
	enable_whole_program_optimize = false;

	HKEY hKey = 0;
	LONG result = RegOpenKeyEx(
		HKEY_CURRENT_USER, TEXT("Software\\Clanlib.org\\ClanLib Configure\\InstallLocation"),
		0, KEY_READ, &hKey);
	if (result == ERROR_SUCCESS)
	{
		DWORD type;
		DWORD value;
		DWORD size;
		
		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("TargetVersion"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			target_version = value;
		}

		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("IncludeUnicode"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			include_unicode = (value != 0);
		}

		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("IncludeMTDLL"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			include_mtdll = (value != 0);
		}

		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("IncludeSSE2"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			include_sse2 = (value != 0);
		}

		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("DebugOptimize"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			enable_debug_optimize = (value != 0);
		}

		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("WholeProgramOptimize"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			enable_whole_program_optimize = (value != 0);
		}

		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("IncludeIntrinsics"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			include_intrinsics = (value != 0);
		}

		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("IncludeDLL"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			include_dll = (value != 0);
		}

		size = sizeof(DWORD);
		result = RegQueryValueEx(hKey, TEXT("IncludeX64"), 0, &type, (LPBYTE) &value, &size);
		if (result == ERROR_SUCCESS && type == REG_DWORD)
		{
			include_x64 = (value != 0);
		}

		RegCloseKey(hKey);
	}

	memset(&propsheetpage, 0, sizeof(PROPSHEETPAGE));
	propsheetpage.dwSize = sizeof(PROPSHEETPAGE);
	propsheetpage.dwFlags = PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	propsheetpage.pszTemplate = MAKEINTRESOURCE(IDD_TARGET_PAGE);
	propsheetpage.pfnDlgProc = (DLGPROC)&PageTarget::dialog_proc;
	propsheetpage.lParam = (LPARAM) this;
	propsheetpage.pszHeaderTitle = TEXT("Project Setup");
	propsheetpage.pszHeaderSubTitle = TEXT("Development environment");
	handle_propsheetpage = CreatePropertySheetPage(&propsheetpage);
}

INT_PTR CALLBACK PageTarget::dialog_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			HWND default_focus = (HWND) wParam;
			PROPSHEETPAGE *propsheetpage = (PROPSHEETPAGE *) lParam;
			PageTarget *self = (PageTarget *) propsheetpage->lParam;
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) self);

			if (self->target_version == 900)
				CheckRadioButton(hWnd, IDC_RADIO_VC90, IDC_RADIO_VC100, IDC_RADIO_VC90);
			else if (self->target_version == 1000)
				CheckRadioButton(hWnd, IDC_RADIO_VC90, IDC_RADIO_VC100, IDC_RADIO_VC100);

			SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_NONUNICODE), BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_UNICODE), BM_SETCHECK, self->include_unicode ? BST_CHECKED : BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_MTDLL), BM_SETCHECK, self->include_mtdll ? BST_CHECKED : BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_SSE2), BM_SETCHECK, self->include_sse2 ? BST_CHECKED : BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_INTRINSICS), BM_SETCHECK, self->include_intrinsics ? BST_CHECKED : BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_DEBUG_OPTIMIZE), BM_SETCHECK, self->enable_debug_optimize ? BST_CHECKED : BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_WHOLE_PROGRAM_OPTIMIZE), BM_SETCHECK, self->enable_whole_program_optimize ? BST_CHECKED : BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_DLL), BM_SETCHECK, self->include_dll ? BST_CHECKED : BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_X64), BM_SETCHECK, self->include_x64 ? BST_CHECKED : BST_UNCHECKED, 0);

			// return FALSE if we set the focus
			return TRUE;
		}
	case WM_NOTIFY:
		{
			NMHDR *header = (NMHDR *) lParam;
			PageTarget *self = (PageTarget *) GetWindowLongPtr(hWnd, GWL_USERDATA);
			return self->on_notify(hWnd, header);
		}
	case WM_DESTROY:
	default:
		return FALSE;
	}
}

INT_PTR PageTarget::on_notify(HWND hWnd, NMHDR *header)
{
	// Don't go to next page yet:
	// SetWindowLong(hwnd, DWL_MSGRESULT, -1);
	// return TRUE;

	// Go straight to the completion page:
	// SetWindowLong(hwnd, DWL_MSGRESULT, IDD_FINISH);
	// return TRUE;

	// Not finished yet:
	// SetWindowLong(hwnd, DWL_MSGRESULT, TRUE);
	// return TRUE;

	switch (header->code)
	{
	case PSN_SETACTIVE:
		PostMessage(header->hwndFrom, PSM_SETWIZBUTTONS, 0, PSWIZB_BACK|PSWIZB_NEXT);
		return TRUE;
	case PSN_WIZBACK:
	case PSN_WIZNEXT:
		if (IsDlgButtonChecked(hWnd, IDC_RADIO_VC90) == BST_CHECKED)
			target_version = 900;
		else if (IsDlgButtonChecked(hWnd, IDC_RADIO_VC100) == BST_CHECKED)
			target_version = 1000;
		include_unicode = (SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_UNICODE), BM_GETCHECK, 0, 0) == BST_CHECKED);
		include_x64 = (SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_X64), BM_GETCHECK, 0, 0) == BST_CHECKED);
		include_mtdll = (SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_MTDLL), BM_GETCHECK, 0, 0) == BST_CHECKED);
		include_sse2 = (SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_SSE2), BM_GETCHECK, 0, 0) == BST_CHECKED);
		include_intrinsics = (SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_INTRINSICS), BM_GETCHECK, 0, 0) == BST_CHECKED);
		enable_debug_optimize = (SendMessage(GetDlgItem(hWnd, IDC_CHECK_DEBUG_OPTIMIZE), BM_GETCHECK, 0, 0) == BST_CHECKED);
		enable_whole_program_optimize = (SendMessage(GetDlgItem(hWnd, IDC_CHECK_WHOLE_PROGRAM_OPTIMIZE), BM_GETCHECK, 0, 0) == BST_CHECKED);
		include_dll = (SendMessage(GetDlgItem(hWnd, IDC_CHECK_INCLUDE_DLL), BM_GETCHECK, 0, 0) == BST_CHECKED);
		return TRUE;
	case PSN_WIZFINISH:
	default:
		return FALSE;
	}
}
