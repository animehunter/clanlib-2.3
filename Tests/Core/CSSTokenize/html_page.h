
#pragma once

class HTMLPage
{
public:
	HTMLPage(const CL_String &page_url, const CL_String &refererer_url = CL_String());

	CL_String webpage;
	CL_String pagecss;

private:
	static CL_String download_url(const CL_String &page_url, const CL_String &refererer_url);
};
