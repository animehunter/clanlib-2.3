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

#include "Core/precomp.h"
#include "API/Core/XML/dom_attr.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "dom_node_generic.h"
#include "dom_tree_node.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomAttr construction:

CL_DomAttr::CL_DomAttr()
{
}

CL_DomAttr::CL_DomAttr(CL_DomDocument doc, const CL_DomString &name, const CL_DomString &namespace_uri)
: CL_DomNode(doc, ATTRIBUTE_NODE)
{
	CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
	impl->get_tree_node()->set_node_name(doc_impl, name);
	impl->get_tree_node()->set_namespace_uri(doc_impl, namespace_uri);
}

CL_DomAttr::CL_DomAttr(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}

CL_DomAttr::~CL_DomAttr()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomAttr attributes:

CL_DomString CL_DomAttr::get_name() const
{
	if (impl)
		return impl->get_tree_node()->get_node_name();
	return CL_DomString();
}
	
bool CL_DomAttr::get_specified() const
{
	return false;
}
	
CL_DomString CL_DomAttr::get_value() const
{
	if (impl)
		return impl->get_tree_node()->get_node_value();
	return CL_DomString();
}
	
void CL_DomAttr::set_value(const CL_DomString &value)
{
	if (impl)
	{
		CL_DomDocument_Generic *doc_impl = (CL_DomDocument_Generic *) impl->owner_document.get();
		impl->get_tree_node()->set_node_value(doc_impl, value);
	}
}

CL_DomElement CL_DomAttr::get_owner_element() const
{
	if (impl)
		return get_parent_node().to_element();
	return CL_DomElement();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomAttr operations:

/////////////////////////////////////////////////////////////////////////////
// CL_DomAttr implementation:
