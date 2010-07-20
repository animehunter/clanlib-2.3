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

#pragma once

#include "gameobject.h"

class CL_Sprite;
class CL_SoundBuffer;
class World;

class TankVehicle : public GameObject
{
// Enums
public:
	enum TankType
	{
		SPACE_SHOOT
	};
	
// Construction
public:
	TankVehicle(TankType type, World *world);

// Attributes:
public:
	bool isSelected() const;
	
// Operations:
public:
	void setAngle(float angle);
	void setDestAngle(float angle);

	void setTurretAngle(float angle);
	void setDestTurretAngle(float angle);
	void setTurretTargetPos(int x, int y);
	
	void getPos(int &x, int &y);
	void setPos(int x, int y);
	void setTargetPos(int x, int y);
	
	void select();
	void deselect();
	
	void fire(bool nuke);
	
	bool hitCheck(CL_CollisionOutline *outline, GameObject *other);
	bool hitCheck(const CL_Rect &rect);
	bool hitCheck(int x, int y);
	
	virtual void draw();
	virtual bool update(int timeElapsed_ms);

// Implementation:
private:
	CL_Sprite *spriteBody;
	CL_Sprite *spriteTurret;
	
	CL_Sprite *spriteBodyStill;
	CL_Sprite *spriteBodyMoving;
	CL_Sprite *spriteTurretStill;
	CL_Sprite *spriteTurretShooting;
	CL_Sprite *spriteTurretReloading;
	CL_Sprite *spriteSelected;
	CL_Sprite *spriteTurretGunFlash;
	CL_Sprite *spriteRedGlow;
	
	CL_SoundBuffer *soundTurret;

	CL_CollisionOutline *collisionBody;
	
	float bodyAngle;
	float destBodyAngle, deltaBodyAngle;

	float turretAngle;
	float destTurretAngle, deltaTurretAngle;

	float posX, posY;
	float destPosX, destPosY;
	float deltaPosX, deltaPosY;
	
	float bodyTurnSpeed;
	float turretTurnSpeed;
	float moveSpeed;
	
	bool selected;
	
	bool isFiring;
	bool reverse;
};

