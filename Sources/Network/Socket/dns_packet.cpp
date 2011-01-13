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
#include "API/Network/Socket/dns_packet.h"
#include "API/Network/Socket/dns_resource_record.h"
#include "dns_packet_impl.h"
#ifndef WIN32
#include <netinet/in.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_DNSPacket Construction:

CL_DNSPacket::CL_DNSPacket()
: impl(new CL_DNSPacket_Impl)
{
}

CL_DNSPacket::CL_DNSPacket(const CL_DataBuffer &data)
: impl(new CL_DNSPacket_Impl)
{
	set_data(data);
}

CL_DNSPacket::CL_DNSPacket(
	int query_id,
	int opcode,
	bool recursion_desired,
	const CL_String &_question_name,
	int question_type,
	int question_class)
: impl(new CL_DNSPacket_Impl)
{
	CL_String question_name = _question_name;

	unsigned short command = (opcode << 11);
	if (recursion_desired)
		command |= (1 << 8);
	int question_count = 1;

	// Create dns packet with one question:

	int header_size = 12;
	int question_size = question_name.length() + 6;
	impl->data.set_size(header_size + question_size);
	unsigned char *d = (unsigned char *) impl->data.get_data();
	memset(d, 0, impl->data.get_size());
	*(unsigned short *) (d + 0) = htons(query_id);
	*(unsigned short *) (d + 2) = htons(command);
	*(unsigned short *) (d + 4) = htons(question_count);
	memcpy(d + header_size + 1, question_name.data(), question_name.length());
	CL_String8::size_type pos = 0;
	while (pos < question_name.length())
	{
		CL_String8::size_type end_pos = question_name.find('.', pos);
		if (end_pos == CL_String8::npos)
			end_pos = question_name.length();
		d[header_size + pos] = end_pos - pos;
		pos = end_pos+1;
	}
	*(unsigned short *) (d + header_size + pos + 1) = htons(question_type);
	*(unsigned short *) (d + header_size + pos + 3) = htons(question_class);
}

CL_DNSPacket::~CL_DNSPacket()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DNSPacket Attributes:

const CL_DataBuffer &CL_DNSPacket::get_data() const
{
	return impl->data;
}

unsigned short CL_DNSPacket::get_query_id() const
{
	if (impl->data.get_size() < 2)
		return 0;
	unsigned short id = ntohs(*(unsigned short *) (impl->data.get_data() + 0));
	return id;
}

bool CL_DNSPacket::is_query() const
{
	if (impl->data.get_size() < 4)
		return 0;
	unsigned short command = ntohs(*(unsigned short *) (impl->data.get_data() + 2));
	return (command & 0x8000) != 0;
}

bool CL_DNSPacket::is_response() const
{
	return !is_query();
}

int CL_DNSPacket::get_opcode() const
{
	if (impl->data.get_size() < 4)
		return 0;
	unsigned short command = ntohs(*(unsigned short *) (impl->data.get_data() + 2));
	return (command & 0x7800) >> 11;
}

bool CL_DNSPacket::is_authoriative_answer() const
{
	if (impl->data.get_size() < 4)
		return 0;
	unsigned short command = ntohs(*(unsigned short *) (impl->data.get_data() + 2));
	return (command & 0x0400) != 0;
}

bool CL_DNSPacket::is_truncated() const
{
	if (impl->data.get_size() < 4)
		return 0;
	unsigned short command = ntohs(*(unsigned short *) (impl->data.get_data() + 2));
	return (command & 0x0200) != 0;
}

bool CL_DNSPacket::is_recursion_desired() const
{
	if (impl->data.get_size() < 4)
		return 0;
	unsigned short command = ntohs(*(unsigned short *) (impl->data.get_data() + 2));
	return (command & 0x0100) != 0;
}

bool CL_DNSPacket::is_recursion_available() const
{
	if (impl->data.get_size() < 4)
		return 0;
	unsigned short command = ntohs(*(unsigned short *) (impl->data.get_data() + 2));
	return (command & 0x0080) != 0;
}

int CL_DNSPacket::get_response_code() const
{
	if (impl->data.get_size() < 4)
		return 0;
	unsigned short command = ntohs(*(unsigned short *) (impl->data.get_data() + 2));
	return (command & 0x000f);
}

int CL_DNSPacket::get_question_count() const
{
	if (impl->data.get_size() < 6)
		return 0;
	unsigned short question_count = ntohs(*(unsigned short *) (impl->data.get_data() + 4));
	return question_count;
}

int CL_DNSPacket::get_answer_count() const
{
	if (impl->data.get_size() < 8)
		return 0;
	unsigned short answer_count = ntohs(*(unsigned short *) (impl->data.get_data() + 6));
	return answer_count;
}

int CL_DNSPacket::get_nameserver_count() const
{
	if (impl->data.get_size() < 10)
		return 0;
	unsigned short ns_count = ntohs(*(unsigned short *) (impl->data.get_data() + 8));
	return ns_count;
}

int CL_DNSPacket::get_additional_count() const
{
	if (impl->data.get_size() < 12)
		return 0;
	unsigned short additional_count = ntohs(*(unsigned short *) (impl->data.get_data() + 10));
	return additional_count;
}

CL_String CL_DNSPacket::get_question_name(int index) const
{
	int header_size = 12;
	int questions_start = header_size;
	int pos = questions_start;
	for (int i = 0; i < index-1; i++)
	{
		// question section format is a qname plus 2 words for qtype and qclass
		pos = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos) + 4;
	}

	return CL_DNSResourceRecord::read_domain_name(impl->data, pos);
}

int CL_DNSPacket::get_question_type(int index) const
{
	int header_size = 12;
	int questions_start = header_size;
	int pos = questions_start;
	for (int i = 0; i < index-1; i++)
	{
		// question section format is a qname plus 2 words for qtype and qclass
		pos = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos) + 4;
	}
	int end_pos = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos);
	if (impl->data.get_size() < end_pos + 2)
		throw CL_Exception("Premature end of DNS packet");
	unsigned short qtype = ntohs(*(unsigned short*) (impl->data.get_data() + end_pos));
	return qtype;
}

int CL_DNSPacket::get_question_class(int index) const
{
	int header_size = 12;
	int questions_start = header_size;
	int pos = questions_start;
	for (int i = 0; i < index-1; i++)
	{
		// question section format is a qname plus 2 words for qtype and qclass
		pos = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos) + 4;
	}
	int end_pos = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos);
	if (impl->data.get_size() < end_pos + 4)
		throw CL_Exception("Premature end of DNS packet");
	unsigned short qclass = ntohs(*(unsigned short*) (impl->data.get_data() + end_pos + 2));
	return qclass;
}

CL_DNSResourceRecord CL_DNSPacket::get_answer(int index) const
{
	int i, pos;
	int question_count = get_question_count();

	// Skip questions part:
	int header_size = 12;
	int questions_start = header_size;
	pos = questions_start;
	for (i = 0; i < question_count; i++)
	{
		// question section format is a qname plus 2 words for qtype and qclass
		pos = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos) + 4;
	}
	int questions_end = pos;
	if (impl->data.get_size() < questions_end)
		throw CL_Exception("Premature end of DNS packet");

	// Find the requested resource record:
	pos = questions_end;
	for (i = 0; i < index; i++)
	{
		// resource record format:
		// qname + [word] qtype + [word] qclass + [dword] ttl + [word] rdlength + [rdlength] rdata
		int qname_end = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos);
		if (impl->data.get_size() < qname_end + 8)
			throw CL_Exception("Premature end of DNS packet");
		unsigned short rdlength = ntohs(*(unsigned short*) (impl->data.get_data() + qname_end + 8));
		int rr_end = qname_end + 10 + rdlength;
		if (impl->data.get_size() < rr_end)
			throw CL_Exception("Premature end of DNS packet");
		pos = rr_end;
	}

	CL_DNSResourceRecord record;
	record.set_record(*this, pos);
	return record;
}

CL_DNSResourceRecord CL_DNSPacket::get_nameserver(int index) const
{
	int i, pos;
	int question_count = get_question_count();
	int answer_count = get_answer_count();

	// Skip questions part:
	int header_size = 12;
	int questions_start = header_size;
	pos = questions_start;
	for (i = 0; i < question_count; i++)
	{
		// question section format is a qname plus 2 words for qtype and qclass
		pos = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos) + 4;
	}
	int questions_end = pos;
	if (impl->data.get_size() < questions_end)
		throw CL_Exception("Premature end of DNS packet");

	// Find the requested resource record:
	pos = questions_end;
	for (i = 0; i < answer_count + index; i++)
	{
		// resource record format:
		// qname + [word] qtype + [word] qclass + [dword] ttl + [word] rdlength + [rdlength] rdata
		int qname_end = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos);
		if (impl->data.get_size() < qname_end + 8)
			throw CL_Exception("Premature end of DNS packet");
		unsigned short rdlength = ntohs(*(unsigned short*) (impl->data.get_data() + qname_end + 8));
		int rr_end = qname_end + 10 + rdlength;
		if (impl->data.get_size() < rr_end)
			throw CL_Exception("Premature end of DNS packet");
		pos = rr_end;
	}

	CL_DNSResourceRecord record;
	record.set_record(*this, pos);
	return record;
}

CL_DNSResourceRecord CL_DNSPacket::get_additional(int index) const
{
	int i, pos;
	int question_count = get_question_count();
	int answer_count = get_answer_count();
	int nameserver_count = get_nameserver_count();

	// Skip questions part:
	int header_size = 12;
	int questions_start = header_size;
	pos = questions_start;
	for (i = 0; i < question_count; i++)
	{
		// question section format is a qname plus 2 words for qtype and qclass
		pos = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos) + 4;
	}
	int questions_end = pos;
	if (impl->data.get_size() < questions_end)
		throw CL_Exception("Premature end of DNS packet");

	// Find the requested resource record:
	pos = questions_end;
	for (i = 0; i < answer_count + nameserver_count + index; i++)
	{
		// resource record format:
		// qname + [word] qtype + [word] qclass + [dword] ttl + [word] rdlength + [rdlength] rdata
		int qname_end = CL_DNSResourceRecord::find_domain_name_end(impl->data, pos);
		if (impl->data.get_size() < qname_end + 8)
			throw CL_Exception("Premature end of DNS packet");
		unsigned short rdlength = ntohs(*(unsigned short*) (impl->data.get_data() + qname_end + 8));
		int rr_end = qname_end + 10 + rdlength;
		if (impl->data.get_size() < rr_end)
			throw CL_Exception("Premature end of DNS packet");
		pos = rr_end;
	}

	CL_DNSResourceRecord record;
	record.set_record(*this, pos);
	return record;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DNSPacket Operations:

void CL_DNSPacket::set_data(const CL_DataBuffer &data)
{
	impl->data = data;
}

void CL_DNSPacket::set_query_id(unsigned short query_id)
{
	if (impl->data.get_size() < 2)
		return;
	*(unsigned short *) (impl->data.get_data() + 0) = htons(query_id);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DNSPacket Implementation:
