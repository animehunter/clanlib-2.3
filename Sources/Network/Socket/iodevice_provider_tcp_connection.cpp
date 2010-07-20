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

#include "Network/precomp.h"
#include "iodevice_provider_tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/logger.h"

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_TCPConnection Construction:

CL_IODeviceProvider_TCPConnection::CL_IODeviceProvider_TCPConnection()
: timeout(15000)
{
	socket.create_tcp();
	create_events();
}
	
CL_IODeviceProvider_TCPConnection::CL_IODeviceProvider_TCPConnection(const CL_SocketName &remote)
: timeout(15000)
{
	socket.create_tcp();
	create_events();
	connect(remote);
}
	
CL_IODeviceProvider_TCPConnection::CL_IODeviceProvider_TCPConnection(const CL_SocketName &remote, const CL_SocketName &local)
: timeout(15000)
{
	socket.create_tcp();
	create_events();
	connect(remote, local);
}
	
CL_IODeviceProvider_TCPConnection::CL_IODeviceProvider_TCPConnection(int handle, bool close_socket)
: timeout(15000)
{
	socket.set_handle(handle, close_socket);
	create_events();
}

CL_IODeviceProvider_TCPConnection::~CL_IODeviceProvider_TCPConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_TCPConnection Attributes:

CL_SocketName CL_IODeviceProvider_TCPConnection::get_local_name() const
{
	return socket.get_local_name();
}

CL_SocketName CL_IODeviceProvider_TCPConnection::get_remote_name() const
{
	return socket.get_remote_name();
}

CL_Event CL_IODeviceProvider_TCPConnection::get_read_event()
{
	return read_event;
}

CL_Event CL_IODeviceProvider_TCPConnection::get_write_event()
{
	return write_event;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_TCPConnection Operations:

void CL_IODeviceProvider_TCPConnection::connect(const CL_SocketName &remote)
{
	socket.connect(remote);
	int wakeup_reason = CL_Event::wait(write_event, except_event, timeout);
	switch (wakeup_reason)
	{
	case 0: return;
	case 1: receive(0, 0, false); throw CL_Exception("Connection attempt failed");
	case -1: throw CL_Exception("Connection attempt timed out");
	}
}

void CL_IODeviceProvider_TCPConnection::connect(
	const CL_SocketName &remote,
	const CL_SocketName &local)
{
	socket.bind(local, false);
	connect(remote);
}
	
void CL_IODeviceProvider_TCPConnection::set_handle(int handle, bool close_socket)
{
	socket.set_handle(handle, close_socket);
}

void CL_IODeviceProvider_TCPConnection::disconnect_abortive()
{
	socket.disconnect_abortive();
}

void CL_IODeviceProvider_TCPConnection::disconnect_graceful()
{
	socket.disconnect_graceful(timeout);
}

void CL_IODeviceProvider_TCPConnection::set_nodelay(bool enable)
{
	socket.set_nodelay(enable);
}

void CL_IODeviceProvider_TCPConnection::set_keep_alive(bool enable, int timeout, int interval)
{
	socket.set_keep_alive(enable, timeout, interval);
}

int CL_IODeviceProvider_TCPConnection::send(const void *data, int len, bool send_all)
{
	if (!send_all)
	{
		return socket.send(data, len);
	}
	else
	{
		const char *d = (const char *) data;
		int pos = 0;
		while (pos < len)
		{
			if (!write_event.wait(timeout))
				throw CL_Exception("Send timed out");
			pos += socket.send(d+pos, len-pos);
		}
		return pos;
	}
}

int CL_IODeviceProvider_TCPConnection::receive(void *data, int len, bool receive_all)
{
	if (!receive_all)
	{
		return socket.receive(data, len);
	}
	else
	{
		char *d = (char *) data;
		int pos = 0;
		while (pos < len)
		{
			if (!read_event.wait(timeout))
				throw CL_Exception("Receive timed out");
			int received = socket.receive(d+pos, len-pos);
			if (received == 0)
				throw CL_Exception("Unable to receive all data: connection closed by peer");
			pos += received;
		}
		return pos;
	}
}

int CL_IODeviceProvider_TCPConnection::peek(void *data, int len)
{
	return socket.peek(data, len);
}

CL_IODeviceProvider *CL_IODeviceProvider_TCPConnection::duplicate()
{
	throw CL_Exception("CL_IODeviceProvider_TCPConnection::duplicate() - duplicate not supported for TCP connections.");
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_TCPConnection Implementation:

void CL_IODeviceProvider_TCPConnection::create_events()
{
#ifdef WIN32
	read_event = CL_Event(new CL_EventProvider_Win32Socket(&socket, CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(new CL_EventProvider_Win32Socket(&socket, CL_EventProvider_Win32Socket::socket_event_write));
	except_event = CL_Event(new CL_EventProvider_Win32Socket(&socket, CL_EventProvider_Win32Socket::socket_event_exception));
#else
	read_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_read));
	write_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_write));
	except_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_exception));
#endif
}
