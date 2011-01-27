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

#include "Network/precomp.h"
#include "unix_socket.h"
#include "API/Core/Text/string_format.h"
#include "API/Network/Socket/socket_name.h"
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>

#ifdef __APPLE__
# define SOL_TCP IPPROTO_TCP    // Fix for BSD systems. --NDT
#endif

CL_UnixSocket::CL_UnixSocket()
: handle(-1), close_handle_flag(true)
{
}

CL_UnixSocket::CL_UnixSocket(int handle)
: handle(handle), close_handle_flag(true)
{
	set_nonblocking();
}

CL_UnixSocket::~CL_UnixSocket()
{
	disconnect_abortive();
}

void CL_UnixSocket::set_handle(SOCKET new_handle, bool new_close_handle)
{
	close_handle();
	handle = new_handle;
	close_handle_flag = new_close_handle;
}

void CL_UnixSocket::create_tcp()
{
	create_socket_handle(SOCK_STREAM);
	set_nonblocking();
}

void CL_UnixSocket::create_udp()
{
	create_socket_handle(SOCK_DGRAM);
	set_nonblocking();
}

void CL_UnixSocket::close_handle()
{
	if (handle != -1 && close_handle_flag)
		close(handle);
	handle = -1;
}

void CL_UnixSocket::disconnect_graceful(int timeout)
{
	shutdown(handle, SHUT_RDWR);

	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(handle, &rfds);
	timeval tv;
	tv.tv_sec = timeout/1000;
	tv.tv_usec = (timeout%1000)*1000;
	select(handle+1, &rfds, 0, 0, &tv);

	close_handle();
}

void CL_UnixSocket::disconnect_abortive()
{
	if (handle != -1)
	{
		// Disable any lingering to cause a hard closure
		linger linger_value;
		linger_value.l_onoff = 1;
		linger_value.l_linger = 0;
		setsockopt(handle, SOL_SOCKET, SO_LINGER, (const char *) &linger_value, sizeof(linger));
	}
	close_handle();
}

void CL_UnixSocket::create_socket_handle(int type)
{
	handle = socket(AF_INET, type, 0);
	if (handle == -1)
		throw CL_Exception(L"Unable to create socket handle");
}

void CL_UnixSocket::set_nonblocking()
{
	int nonblocking = 1;
	ioctl(handle, FIONBIO, &nonblocking);

#ifdef SO_NOSIGPIPE
	int value = 1;
	setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, (const char *) &value, sizeof(int));
#endif
}

void CL_UnixSocket::set_nodelay(bool enable)
{
	int value = enable ? 1 : 0;
	int result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &value, sizeof(int));
	throw_if_failed(result);
}

void CL_UnixSocket::set_keep_alive(bool enable, int timeout, int interval)
{
	int value = enable ? 1 : 0;
	int result = setsockopt(handle, SOL_SOCKET, SO_KEEPALIVE, (const char *) &value, sizeof(int));
	throw_if_failed(result);

	if (enable && timeout != 0 && interval != 0)
	{
		setsockopt(handle, SOL_TCP, TCP_KEEPIDLE, (const char *) &timeout, sizeof(int));
		setsockopt(handle, SOL_TCP, TCP_KEEPINTVL, (const char *) &interval, sizeof(int));
	}
}

void CL_UnixSocket::bind(const CL_SocketName &socketname, bool reuse_address)
{
	if (reuse_address)
	{
		int value = 1;
		int result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const char *) &value, sizeof(int));
		throw_if_failed(result);
	}

	sockaddr_in addr;
	socketname.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	int result = ::bind(handle, (const sockaddr *) &addr, sizeof(sockaddr_in));
	throw_if_failed(result);
}

void CL_UnixSocket::listen(int backlog)
{
	int result = ::listen(handle, backlog);
	throw_if_failed(result);
}

int CL_UnixSocket::accept(CL_SocketName &out_socketname)
{
	sockaddr_in new_addr;
	memset(&new_addr, 0, sizeof(sockaddr_in));
	socklen_t size = sizeof(sockaddr_in);
	int accepted_socket = ::accept(handle, (sockaddr *) &new_addr, &size);
	throw_if_invalid(accepted_socket);
	out_socketname.from_sockaddr(AF_INET, (sockaddr *) &new_addr, size);
	return accepted_socket;
}

void CL_UnixSocket::connect(const CL_SocketName &socketname)
{
	sockaddr_in addr;
	socketname.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	int result = ::connect(handle, (const sockaddr *) &addr, sizeof(sockaddr_in));
	if (result == -1)
	{
		int errorcode = errno;
		if ( ! ( (errorcode == EWOULDBLOCK) || (errorcode == EINPROGRESS) ) )
			throw CL_Exception(error_to_string(errorcode));
	}
}

CL_SocketName CL_UnixSocket::get_local_name() const
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	socklen_t size = sizeof(sockaddr_in);
	int result = getsockname(handle, (sockaddr *) &addr, &size);
	throw_if_failed(result);

	CL_SocketName name;
	name.from_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	return name;
}

CL_SocketName CL_UnixSocket::get_remote_name() const
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	socklen_t size = sizeof(sockaddr_in);
	int result = getpeername(handle, (sockaddr *) &addr, &size);
	throw_if_failed(result);

	CL_SocketName name;
	name.from_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	return name;
}

int CL_UnixSocket::receive(void *data, int size)
{
	int result = ::recv(handle, (char *) data, size, 0);
	throw_if_failed(result);
	return result;
}

int CL_UnixSocket::peek(void *data, int size)
{
	int result = ::recv(handle, (char *) data, size, MSG_PEEK);
	throw_if_failed(result);
	return result;
}

int CL_UnixSocket::receive_from(void *data, int size, CL_SocketName &out_socketname)
{
	sockaddr_in new_addr;
	memset(&new_addr, 0, sizeof(sockaddr_in));
	socklen_t addr_size = sizeof(sockaddr_in);
	int result = ::recvfrom(handle, (char *) data, size, 0, (sockaddr *) &new_addr, &addr_size);
	throw_if_failed(result);
	out_socketname.from_sockaddr(AF_INET, (sockaddr *) &new_addr, size);
	return result;
}

int CL_UnixSocket::peek_from(void *data, int size, CL_SocketName &out_socketname)
{
	sockaddr_in new_addr;
	memset(&new_addr, 0, sizeof(sockaddr_in));
	socklen_t addr_size = sizeof(sockaddr_in);
	int result = ::recvfrom(handle, (char *) data, size, MSG_PEEK, (sockaddr *) &new_addr, &addr_size);
	throw_if_failed(result);
	out_socketname.from_sockaddr(AF_INET, (sockaddr *) &new_addr, size);
	return result;
}

int CL_UnixSocket::send(const void *data, int size)
{
	int result = ::send(handle, (const char *) data, size, 0);
	if (result == -1)
	{
		int errorcode = errno;
		if (errorcode == EWOULDBLOCK)
		{
			return 0;
		}
		else
		{
			throw CL_Exception(error_to_string(errorcode));
		}
	}
	else
	{
		return result;
	}
}

int CL_UnixSocket::send_to(const void *data, int size, const CL_SocketName &socketname)
{
	sockaddr_in addr;
	socketname.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	int result = ::sendto(handle, (const char *) data, size, 0, (const sockaddr *) &addr, sizeof(sockaddr_in));
	if (result == -1)
	{
		int errorcode = errno;
		if (errorcode == EWOULDBLOCK)
		{
			return 0;
		}
		else
		{
			throw CL_Exception(error_to_string(errorcode));
		}
	}
	else
	{
		return result;
	}
}

void CL_UnixSocket::close_send()
{
	shutdown(handle, SHUT_WR);
}

void CL_UnixSocket::throw_if_invalid(int result) const
{
	if (result == -1)
		throw CL_Exception(error_to_string(errno));
}

void CL_UnixSocket::throw_if_failed(int result) const
{
	if (result == -1)
		throw CL_Exception(error_to_string(errno));
}

void CL_UnixSocket::throw_if_socket_failed(int result) const
{
	if (result == -1)
	{
		int error = 0;
		socklen_t length = sizeof(int);
		getsockopt(handle, SOL_SOCKET, SO_ERROR, (char *) &error, &length);
		throw CL_Exception(error_to_string(error));
	}
}

CL_String CL_UnixSocket::error_to_string(int err)
{
	char buffer[256];
	memset(buffer, 0, 256);
	strerror_r(err, buffer, 256);
	return CL_String( buffer );
}
