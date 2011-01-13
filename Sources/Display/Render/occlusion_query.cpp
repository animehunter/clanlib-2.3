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

#include "Display/precomp.h"
#include "API/Display/Render/occlusion_query.h"
#include "API/Display/TargetProviders/occlusion_query_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OcclusionQuery_Impl Class:

class CL_OcclusionQuery_Impl
{
public:
	CL_OcclusionQuery_Impl() : provider(0)
	{
	}

	~CL_OcclusionQuery_Impl()
	{
		if( provider )
			provider->destroy();
	}

	CL_OcclusionQueryProvider *provider;
};
/////////////////////////////////////////////////////////////////////////////
// CL_OcclusionQuery Construction:

CL_OcclusionQuery::CL_OcclusionQuery(CL_GraphicContext &context)
: impl(new CL_OcclusionQuery_Impl)
{
	CL_GraphicContextProvider *gc_provider = context.get_provider();
	impl->provider = gc_provider->alloc_occlusion_query();
}

CL_OcclusionQuery::~CL_OcclusionQuery()
{
}

CL_OcclusionQuery::CL_OcclusionQuery()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OcclusionQuery Attributes:

void CL_OcclusionQuery::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_OcclusionQuery is null");
}

int CL_OcclusionQuery::get_result()
{
	return impl->provider->get_result();
}

bool CL_OcclusionQuery::is_result_ready()
{
	return impl->provider->is_result_ready();
}

CL_OcclusionQueryProvider *CL_OcclusionQuery::get_provider() const
{
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OcclusionQuery Operations:

void CL_OcclusionQuery::begin()
{
	impl->provider->begin();
}

void CL_OcclusionQuery::end()
{
	impl->provider->end();
}

/////////////////////////////////////////////////////////////////////////////
// CL_OcclusionQuery Implementation:
