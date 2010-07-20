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
**    
*/

#include "precomp.h"
#include "building.h"
#include "world.h"

Building::Building(BuildingType buiding_type, World *world)
: GameObject(world)
{
	CL_GraphicContext gc = world->get_gc();

	switch(buiding_type)
	{
	case HELI_PAD:
		sprite = new CL_Sprite(gc, "helipad", &world->resources);
		collisionBuilding = new CL_CollisionOutline("Gfx/helipad.png");
		collisionBuilding->set_alignment(origin_center);
		break;
	}
	
	exploding = false;
}

Building::~Building()
{
	delete sprite;
	delete collisionBuilding;
}

void Building::setPos(int x, int y)
{
	posX = (float)x;
	posY = (float)y;
	collisionBuilding->set_translation(posX, posY);
}

void Building::setAngle(float newAngle)
{
	angle = newAngle;
	sprite->set_angle(CL_Angle(angle, cl_degrees));
	collisionBuilding->set_angle(CL_Angle(angle, cl_degrees));
}

void Building::draw()
{
	
	CL_GraphicContext gc = world->get_gc();
	sprite->draw(gc, posX, posY);
}

bool Building::update(int timeElapsed_ms)
{
	sprite->update(timeElapsed_ms);
	return true;
}

bool Building::hitCheck(CL_CollisionOutline *outline, GameObject *other)
{
	return collisionBuilding->collide(*outline);
}
