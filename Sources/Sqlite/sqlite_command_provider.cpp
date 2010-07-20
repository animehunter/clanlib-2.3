/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "Sqlite/precomp.h"
#include "sqlite_command_provider.h"
#include "sqlite_connection_provider.h"
#include "sqlite_reader_provider.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"
#include "API/Database/db_command_provider.h"
#include "sqlite3.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteCommandProvider Construction:

CL_SqliteCommandProvider::CL_SqliteCommandProvider(CL_SqliteConnectionProvider *connection, const CL_StringRef &text)
: connection(connection), text(text), vm(0), last_insert_rowid(-1)
{
	CL_String t = text;
	if (t.empty() || t[t.length()-1] != ';')
		t += ";";
	const CL_String::char_type *tail = 0;
	int result;
	for (int i = 0; i < 1000; i++)
	{
		result = sqlite3_prepare(connection->db, t.data(), t.length()*sizeof(CL_String::char_type), &vm, (const char **) &tail);
		if (result != SQLITE_BUSY)
			break;
		CL_System::sleep(1);
	}
	throw_if_failed(result);
}

CL_SqliteCommandProvider::~CL_SqliteCommandProvider()
{
	if (connection->active_reader && connection->active_reader->vm == vm)
	{
		connection->active_reader->destroy_command = true;
	}
	else
	{
		int result = sqlite3_finalize(vm);
		if (result != SQLITE_OK)
		{
			const char *err = sqlite3_errmsg(connection->db);
			CL_StringRef8 error = (err == 0) ? "Unknown error" : err;
			throw CL_Exception(CL_StringHelp::local8_to_text(error));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteCommandProvider Attributes:

int CL_SqliteCommandProvider::get_input_parameter_column(const CL_StringRef &name) const
{
	return sqlite3_bind_parameter_index(vm, CL_StringHelp::text_to_utf8(name).c_str());
}

int CL_SqliteCommandProvider::get_output_last_insert_rowid() const
{
	return last_insert_rowid;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteCommandProvider Operations:

void CL_SqliteCommandProvider::set_input_parameter_string(int index, const CL_StringRef &value)
{
	int result = sqlite3_bind_text(vm, index, value.data(), value.length(), SQLITE_TRANSIENT);
	throw_if_failed(result);
}

void CL_SqliteCommandProvider::set_input_parameter_bool(int index, bool value)
{
	int result = sqlite3_bind_int(vm, index, value ? 1 : 0);
	throw_if_failed(result);
}

void CL_SqliteCommandProvider::set_input_parameter_int(int index, int value)
{
	int result = sqlite3_bind_int(vm, index, value);
	throw_if_failed(result);
}

void CL_SqliteCommandProvider::set_input_parameter_double(int index, double value)
{
	int result = sqlite3_bind_double(vm, index, value);
	throw_if_failed(result);
}

void CL_SqliteCommandProvider::set_input_parameter_datetime(int index, const CL_DateTime &value)
{
	set_input_parameter_string(index, CL_SqliteConnectionProvider::to_sql_datetime(value));
}

void CL_SqliteCommandProvider::set_input_parameter_binary(int index, const CL_DataBuffer &value)
{
	int result = sqlite3_bind_blob(vm, index, value.get_data(), value.get_size(), SQLITE_TRANSIENT);
	throw_if_failed(result);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SqliteCommandProvider Implementation:

void CL_SqliteCommandProvider::throw_if_failed(int result) const
{
	if (result != SQLITE_OK)
	{
		CL_String8 error = sqlite3_errmsg(connection->db);
		throw CL_Exception(CL_StringHelp::local8_to_text(error));
	}
}
