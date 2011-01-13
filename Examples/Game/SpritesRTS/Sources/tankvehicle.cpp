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
**    
*/

#include "precomp.h"

#include "tankvehicle.h"
#include "world.h"
#include "missile.h"

#ifndef WIN32
// For rand()
#include <stdlib.h>
#endif

TankVehicle::TankVehicle(TankType type, World *world)
: GameObject(world)
{
	CL_GraphicContext gc = world->get_gc();

	switch(type)
	{
	case SPACE_SHOOT:
		spriteBodyStill = new CL_Sprite(gc, "SpaceShootBodyStill", &world->resources);
		spriteBodyMoving = new CL_Sprite(gc, "SpaceShootBodyMoving", &world->resources);
		spriteTurretStill = new CL_Sprite(gc, "SpaceShootTurretStill", &world->resources);
		spriteTurretShooting = new CL_Sprite(gc, "SpaceShootTurretShooting", &world->resources);
		spriteTurretReloading = new CL_Sprite(gc, "SpaceShootTurretReloading", &world->resources);
		spriteTurretGunFlash = new CL_Sprite(gc, "SpaceShootTurretGunFlash", &world->resources);
		spriteSelected = new CL_Sprite(gc, "SpaceShootSelected", &world->resources);
		spriteRedGlow = new CL_Sprite(gc, "RedGlow", &world->resources);

		collisionBody = new CL_CollisionOutline("Gfx/spaceshoot_body_still1.png");
		collisionBody->set_alignment(origin_center);

		soundTurret = new CL_SoundBuffer("SpaceShootTurret", &world->resources);
		soundTurret->set_volume(0.3f);
		soundTurret->prepare();
		
		bodyTurnSpeed = 80.0f;
		turretTurnSpeed = 150.0f;
		moveSpeed = 100.0f;
		
		break;
	}
	
	bodyAngle = turretAngle = destBodyAngle = deltaBodyAngle = destTurretAngle = deltaTurretAngle = 0.0f;
	
	selected = false;
	isFiring = false;
	reverse = false;
	
	spriteBody = spriteBodyStill;
	spriteTurret = spriteTurretStill;
}

bool TankVehicle::isSelected() const
{
	return selected;
}

void TankVehicle::select()
{
	selected = true;
}

void TankVehicle::deselect()
{
	selected = false;
}

void TankVehicle::setPos(int x, int y)
{
	posX = destPosX = (float)x;
	posY = destPosY = (float)y;

	collisionBody->set_translation(posX, posY);
}

void TankVehicle::setTurretTargetPos(int x, int y)
{
	CL_Vec2f vector(posX - x, posY - y);
	CL_Vec2f up(0.0f, 1.0f);

	// Calculate angle from current sprite position to mouse position
	float angle = up.angle(vector).to_degrees();
	if(x < posX)
		angle = 360.0f - angle;

	setDestTurretAngle(angle);
}

void TankVehicle::setTargetPos(int x, int y)
{
	destPosX = (float)x;
	destPosY = (float)y;

	CL_Vec2f vector(posX - destPosX, posY - destPosY);
	CL_Vec2f up(0.0f, 1.0f);

	// Calculate angle from current sprite position to mouse position
	float angle = up.angle(vector).to_degrees();
	if(x < posX)
		angle = 360.0f - angle;

	setDestAngle(angle);
	
	// Calculate delta position (to be used per frame-update)
	vector.normalize();
	deltaPosX = -vector.x;
	deltaPosY = -vector.y;

	spriteBody = spriteBodyMoving;
}

void TankVehicle::setAngle(float angle)
{
	bodyAngle = angle;
	collisionBody->set_angle(CL_Angle(angle, cl_degrees));
}

void TankVehicle::setTurretAngle(float angle)
{
	turretAngle = angle;
}

void TankVehicle::setDestAngle(float destAngle)
{
	destBodyAngle = destAngle;
	deltaBodyAngle = destBodyAngle - bodyAngle;
	
	if(deltaBodyAngle > 180.0f)
	{
		deltaBodyAngle -= 360.0f;
		bodyAngle += 360.0f;
	}
	if(deltaBodyAngle < -180.0f)
	{
		deltaBodyAngle += 360.0f;
		bodyAngle -= 360.0f;
	}
}

void TankVehicle::setDestTurretAngle(float destAngle)
{
	destTurretAngle = destAngle;
	deltaTurretAngle = destTurretAngle - turretAngle;
	
	if(deltaTurretAngle > 180.0f)
	{
		deltaTurretAngle -= 360.0f;
		turretAngle += 360.0f;
	}
	if(deltaTurretAngle < -180.0f)
	{
		deltaTurretAngle += 360.0f;
		turretAngle -= 360.0f;
	}
}

bool TankVehicle::hitCheck(CL_CollisionOutline *outline, GameObject *other)
{
	return collisionBody->collide(*outline);
}

bool TankVehicle::hitCheck(const CL_Rect &rect)
{
	int width = spriteBody->get_width();
	int height = spriteBody->get_height();
	
	return (rect.left <= posX + width / 2 && rect.top <= posY + height / 2 && rect.right >= posX - width / 2 && rect.bottom >= posY - height / 2);
}

bool TankVehicle::hitCheck(int x, int y)
{
	int width = spriteBody->get_width();
	int height = spriteBody->get_height();
	
	return (x >= posX - width / 2 && x <= posX + width / 2 && y >= posY - height / 2 && y <= posY + height / 2);
}

void TankVehicle::fire(bool nuke)
{
	if(isFiring == false)
	{
 		spriteTurret = spriteTurretShooting;
		spriteTurretShooting->restart();
	
		soundTurret->play();
		
		isFiring = true;
		
		int delta;
		if(nuke)
			delta = 18;
		else
			delta = 360;
			
		for(int i=0; i<360; i += delta)
		{
			Missile *missile = new Missile(world, this);
			missile->setPos((int)posX, (int)posY);
			missile->setSpeed(1000.0f);
			missile->setAngle(turretAngle + i);
			missile->move(43);
			world->addObject(missile);
		}
	}
}

bool TankVehicle::update(int timeElapsed_ms)
{
	spriteBody->update(timeElapsed_ms);
	spriteTurret->update(timeElapsed_ms);
	float timeElapsed = (float) timeElapsed_ms / 1000.0f;

	if(spriteTurretShooting->is_finished() && spriteTurret == spriteTurretShooting)
	{
		spriteTurret = spriteTurretReloading;
		spriteTurretReloading->restart();
	}
	else if(spriteTurretReloading->is_finished() && spriteTurret == spriteTurretReloading)
	{
		spriteTurret = spriteTurretStill;
		spriteTurretStill->restart();

		isFiring = false;
	}

	if(deltaBodyAngle)
	{
		if(deltaBodyAngle > 0.0f)
		{
			bodyAngle += bodyTurnSpeed * timeElapsed;
			if(bodyAngle > destBodyAngle)
			{
				bodyAngle = destBodyAngle;
				deltaBodyAngle = 0.0f;
			}
		}
		else
		{
			bodyAngle -= bodyTurnSpeed * timeElapsed;
			if(bodyAngle < destBodyAngle)
			{
				bodyAngle = destBodyAngle;
				deltaBodyAngle = 0.0f;
			}
		}
	}
	
	if(deltaTurretAngle)
	{
		if(deltaTurretAngle > 0.0f)
		{
			turretAngle += turretTurnSpeed * timeElapsed;
			if(turretAngle > destTurretAngle)
			{
				turretAngle = destTurretAngle;
				deltaTurretAngle = 0.0f;
			}
		}
		else
		{
			turretAngle -= turretTurnSpeed * timeElapsed;
			if(turretAngle < destTurretAngle)
			{
				turretAngle = destTurretAngle;
				deltaTurretAngle = 0.0f;
			}
		}
	}
	
	if(deltaBodyAngle == 0.0f)
	{
		if(destPosX != posX || destPosY != posY)
		{
			posX += deltaPosX * moveSpeed * timeElapsed;
			posY += deltaPosY * moveSpeed * timeElapsed;
			collisionBody->set_translation(posX, posY);
			
			if((deltaPosX > 0 && posX > destPosX) || (deltaPosX < 0 && posX < destPosX))
				posX = destPosX;
			if((deltaPosY > 0 && posY > destPosY) || (deltaPosY < 0 && posY < destPosY))
				posY = destPosY;

			if(posX == destPosX && posY == destPosY)
			{
				spriteBody = spriteBodyStill;
				reverse = false;
			}
		}
	}
	
	spriteBody->set_angle(CL_Angle(bodyAngle, cl_degrees));
	spriteSelected->set_angle(CL_Angle(bodyAngle, cl_degrees));
	spriteTurret->set_angle(CL_Angle(turretAngle, cl_degrees));
	spriteTurretGunFlash->set_angle(CL_Angle(turretAngle, cl_degrees));
	
	collisionBody->set_angle(CL_Angle(bodyAngle, cl_degrees));
	
	if( !reverse )
	{
		if( world->hitCheck(collisionBody, this) )
		{
			setTargetPos(
				int(posX+cos((CL_PI/180.0f)*(bodyAngle+90+int(rand()%60)-30))*30),
				int(posY+sin((CL_PI/180.0f)*(bodyAngle+90+int(rand()%60)-30))*30));
			
			reverse = true;
		}
	}
	
	return true;
}

void TankVehicle::draw()
{
	CL_GraphicContext gc = world->get_gc();

	// Draw selection	
	if(selected)
		spriteSelected->draw(gc, posX, posY);

	// Draw tankbody shadow
	CL_BlendMode blend;
	blend.set_blend_function(cl_blend_zero, cl_blend_one_minus_src_alpha, cl_blend_zero, cl_blend_one_minus_src_alpha);
	gc.set_blend_mode(blend);
	spriteBody->set_alpha(0.5f);
	spriteBody->draw(gc, posX + 5, posY + 5);

	// Draw tankbody
	blend.set_blend_function(cl_blend_src_alpha, cl_blend_one_minus_src_alpha, cl_blend_src_alpha, cl_blend_one_minus_src_alpha);
	gc.set_blend_mode(blend);
	spriteBody->set_alpha(1.0f);
	spriteBody->draw(gc, posX, posY);
	
	// Draw tankturret shadow
	blend.set_blend_function(cl_blend_zero, cl_blend_one_minus_src_alpha, cl_blend_zero, cl_blend_one_minus_src_alpha);
	gc.set_blend_mode(blend);
	spriteTurret->set_alpha(0.5f);
	spriteTurret->draw(gc, posX + 5, posY + 5);

	// Draw tankturret
	blend.set_blend_function(cl_blend_src_alpha, cl_blend_one_minus_src_alpha, cl_blend_src_alpha, cl_blend_one_minus_src_alpha);
	gc.set_blend_mode(blend);
	spriteTurret->set_alpha(1.0f);
	spriteTurret->draw(gc, posX, posY);

	CL_BlendMode default_blend_mode;
	gc.set_blend_mode(default_blend_mode);

	// Draw gunflash
	if(spriteTurret == spriteTurretShooting)
		spriteTurretGunFlash->draw(gc, posX, posY);
	
	// Draw glow
	spriteRedGlow->draw(gc, posX, posY);
}
