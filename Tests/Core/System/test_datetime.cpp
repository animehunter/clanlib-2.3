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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_datetime()
{
	CL_Console::write_line(" Header: datetime.h");
	CL_Console::write_line("  Class: CL_DateTime");

	CL_Console::write_line("   Function: bool is_null()");
	CL_DateTime datetime;
	if (!datetime.is_null()) fail();
	datetime = CL_DateTime(2009, 2, 4, 17,45, 33, 123456789);
	if (datetime.is_null()) fail();

	CL_Console::write_line("   Function: get_year()");
	if (datetime.get_year() != 2009) fail();
	CL_Console::write_line("   Function: get_month()");
	if (datetime.get_month() != 2) fail();
	CL_Console::write_line("   Function: get_day()");
	if (datetime.get_day() != 4) fail();
	CL_Console::write_line("   Function: get_hour()");
	if (datetime.get_hour() != 17) fail();
	CL_Console::write_line("   Function: get_minutes()");
	if (datetime.get_minutes() != 45) fail();
	CL_Console::write_line("   Function: get_seconds()");
	if (datetime.get_seconds() != 33) fail();
	CL_Console::write_line("   Function: get_nanoseconds()");
	if (datetime.get_nanoseconds() != 123456789) fail();

	CL_Console::write_line("   Function: get_time_of_week()");

	for (int cnt=1; cnt<=31; cnt++)
	{
		datetime = CL_DateTime(2009, 1, cnt, 17,45, 33, 123456789);
		int day_of_week = datetime.get_day_of_week();
		if (day_of_week != ((cnt-1+4)%7)) fail();
	}
	for (int cnt=1; cnt<=28; cnt++)
	{
		datetime = CL_DateTime(2009, 2, cnt, 17,45, 33, 123456789);
		int day_of_week = datetime.get_day_of_week();
		if (day_of_week != ((cnt-1+0)%7)) fail();
	}
	for (int cnt=1; cnt<=31; cnt++)
	{
		datetime = CL_DateTime(2007, 10, cnt, 17,45, 33, 123456789);
		int day_of_week = datetime.get_day_of_week();
		if (day_of_week != ((cnt-1+1)%7)) fail();
	}

	CL_Console::write_line("   Function: set_null()");
	datetime.set_null();
	if (!datetime.is_null()) fail();

	CL_Console::write_line("   Function: set_year()");
	datetime.set_year(2009);
	if (datetime.get_year() != 2009) fail();
	CL_Console::write_line("   Function: set_month()");
	datetime.set_month(2);
	if (datetime.get_month() != 2) fail();
	CL_Console::write_line("   Function: set_day()");
	datetime.set_day(4);
	if (datetime.get_day() != 4) fail();
	CL_Console::write_line("   Function: set_hour()");
	datetime.set_hour(17);
	if (datetime.get_hour() != 17) fail();
	CL_Console::write_line("   Function: set_minutes()");
	datetime.set_minutes(45);
	if (datetime.get_minutes() != 45) fail();
	CL_Console::write_line("   Function: set_seconds()");
	datetime.set_seconds(33);
	if (datetime.get_seconds() != 33) fail();
	CL_Console::write_line("   Function: set_nanoseconds()");
	datetime.set_nanoseconds(123456789);
	if (datetime.get_nanoseconds() != 123456789) fail();

	CL_Console::write_line("   Function: to_ticks()");
	datetime = CL_DateTime(2009, 2, 4, 17,45, 33, 123456789);
	cl_int64 ticks_b = datetime.to_ticks();

	cl_int64 ticks;
	
	ticks = datetime.get_year() - 1601;
	ticks *= 365;	// Days in a year

	// Count number of leap years
	int current_year = datetime.get_year();
	int num_leaps = 0;
	for (int cnt=1601; cnt < current_year; cnt++)
	{
		if ( ( ((cnt % 4) == 0) && ((cnt % 100 !=0)) ) || ((cnt % 400)==0) )
		{
			num_leaps++;
		}
	}
	ticks += num_leaps;	// Extra days

	switch (datetime.get_month())
	{
		case (12):
			ticks += 30;	// Nov
		case (11):
			ticks += 31;	// Oct
		case (10):
			ticks += 30;	// Sep
		case (9):
			ticks += 31;	// Aug
		case (8):
			ticks += 31;	// Jul
		case (7):
			ticks += 30;	// Jun
		case (6):
			ticks += 31;	// May
		case (5):
			ticks += 30;	// Apr
		case (4):
			ticks += 31;	// Mar
		case (3):
			ticks += 28;	// Feb
			if ( ( ((current_year % 4) == 0) && ((current_year % 100 !=0)) ) || ((current_year % 400)==0) )
			{
				ticks++;	// Leap year
			}

		case (2):
			ticks += 31;	// Jan
			break;
	}

	ticks += datetime.get_day() - 1;

	ticks *= 24;	// Hours in day
	ticks += datetime.get_hour();
	ticks *= 60;	// Minutes per hour
	ticks += datetime.get_minutes();
	ticks *= 60;	// Seconds per minute
	ticks += datetime.get_seconds();
	ticks *= 10000000;	// To ticks
	ticks += datetime.get_nanoseconds() / 100;

	if (ticks != ticks_b) fail();


	CL_Console::write_line("   Function: get_current_utc_time(), to_utc(), to_local()");

	CL_Console::write_line("--> Check current time. Press any key to continue");
	datetime = CL_DateTime::get_current_utc_time();
	CL_String datestring = convert_time(datetime);
	CL_Console::write_line(cl_format("[ UTC ] %1", datestring));
	datetime = datetime.to_utc();
	datestring = convert_time(datetime);
	CL_Console::write_line(cl_format("[ UTC ] %1", datestring));
	datetime = datetime.to_local();
	datestring = convert_time(datetime);
	CL_Console::write_line(cl_format("[LOCAL] %1", datestring));

	CL_Console::wait_for_key();

	CL_Console::write_line("   Function: get_current_local_time(), to_utc(), to_local()");

	CL_Console::write_line("--> Check current time. Press any key to continue");
	datetime = CL_DateTime::get_current_local_time();
	datestring = convert_time(datetime);
	CL_Console::write_line(cl_format("[LOCAL] %1", datestring));
	datetime = datetime.to_utc();
	datestring = convert_time(datetime);
	CL_Console::write_line(cl_format("[ UTC ] %1", datestring));
	datetime = datetime.to_local();
	datestring = convert_time(datetime);
	CL_Console::write_line(cl_format("[LOCAL] %1", datestring));

	CL_Console::wait_for_key();

}


CL_String TestApp::convert_time(CL_DateTime &datetime)
{

	CL_StringFormat format("Year:%1 Month: %2 Day: %3 Hour:%4 Mins:%5 Secs:%6 Nano=%7");
	format.set_arg(1, datetime.get_year());
	format.set_arg(2, datetime.get_month());
	format.set_arg(3, datetime.get_day());
	format.set_arg(4, datetime.get_hour());
	format.set_arg(5, datetime.get_minutes());
	format.set_arg(6, datetime.get_seconds());
	format.set_arg(7, datetime.get_nanoseconds());
	return format.get_result();	
}

