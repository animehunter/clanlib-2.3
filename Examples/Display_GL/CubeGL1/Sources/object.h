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
**    Mark Page
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl1.h>

class ObjectPolygon;

class ObjectVertex
{
public:
	ObjectVertex();

	void CalcNormal();

	CL_Vec3f m_OriginalPoint;
	CL_Vec3f m_Point;
	CL_Vec3f m_TexturePoint;

	ObjectPolygon *m_pPoly;	// Polygon that this point belongs to
};

class ObjectPolygon
{
public:
	ObjectPolygon();
	~ObjectPolygon();

	void Init(int num_vertex);
	void Draw(CL_GraphicContext &gc, CL_Texture &texture_image);
	void CalcNormal();

	void Destroy();

	int m_NumVertex;
	ObjectVertex *m_pVertex;
	CL_Vec3f m_Normal;
};

class Object
{
public:
	Object();
	~Object();

	void CreateCube(float size, int num_segments);
	void Draw(CL_GraphicContext_GL1 &gc, CL_Texture &texture_image);
	void CalcPolygonNormal();

public:

	int m_NumPolygon;
	ObjectPolygon *m_pPolygon;
private:
	void Destroy();
	void Init(int num_polygon);

};

