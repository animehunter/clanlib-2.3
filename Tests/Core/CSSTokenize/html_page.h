
#pragma once

class HTMLPage
{
public:
	HTMLPage(const CL_String &page_url, const CL_String &refererer_url = CL_String());

	CL_String webpage;
	CL_String pagecss;

private:
	CL_String load_css(const CL_String &csstext, const CL_String &base_url);
	static CL_String download_url(const CL_String &page_url, const CL_String &refererer_url);
};
