#include "unit.h"
#include <algorithm>

#include <iostream>
using namespace std;

Unit::Unit(Field *pos)
: Element(pos), status(UNIT_IDLE)
{
	init();
	spawned = false;
	decomposed = false;
	
	speed.x = 0;
	speed.y	= 0;

	attackCooldown	= false;
	backswingTime 	= UNIT_BACKSWING_TIME;
	travelTime 		= UNIT_TRAVEL_TIME;
	
	attributes.hitpoints = UNIT_BASE_HITPOINTS;
	attributes.damage	 = UNIT_BASE_DAMAGE;
	attributes.defense	 = UNIT_BASE_DEFENSE;

	path.clear();
}

bool Unit::spawn()
{
	if(!mPosition->habitant && !spawned)
	{
		spawned = true;
		mPosition->habitant = this;
		return true;
	}
	return false;
}

void Unit::loadRectangle()
{
	mResource.width	= 112;
	mResource.height= 112;
	mResource.x		= 0;
	mResource.y		= 0;
}

void Unit::loadImage()
{
	image = Image::load("/opt/EmperorVsAliens/data/images/hiver.png",mResource.x,mResource.y,mResource.width,mResource.height);
}

void Unit::update()
{
	if(!spawned) 
		if(!spawn())
			return;

	frameCount = (frameCount + 1) % 8;
	mResource.x = mResource.width * frameCount;
	mResource.y = mResource.height * status;

	if((ARRIVAL_TIME <= Timer::get_currentFrameTick()) && status == UNIT_MOVING)	arrive();
	if((ATTACK_READY_TIME <= Timer::get_currentFrameTick()) && attackCooldown)		enableAttack();  
	
	if(frameCount == 7 && status == UNIT_DEAD)	 onDeath();	
}

void Unit::IA()
{
	switch(status)
	{
	case UNIT_DEAD:
	case UNIT_MOVING: break;
	case UNIT_IDLE:
		if(!mPosition->x){
			getTarget();
		}
		else if(!path.size()){
			createPath();
		}
		else{
			decision();
		}
	break;
	case UNIT_ATTACKING: 
		if(!attackCooldown) attack();
	break;
	default:
	break;
	}
}

void Unit::createPath()
{
	Field *next = mPosition;
	while(next)
	{ 
		next = next->path[WEST];
		path.push_front(next);
	}
}

void Unit::decision()
{
	Field *next = !path.empty() ? path.back() : 0;
	if(!next)
	{}
	else if(next->habitant && !next->locked){
    	startAttack((Unit *)next->habitant);
	}
	else if(!next->locked){
		next->locked = true;
    	move();
	}
	else{
	}
}

void Unit::move()
{
	ARRIVAL_TIME = Timer::get_currentFrameTick() + travelTime;
	mPosition->habitant = NULL;
	status = UNIT_MOVING;
	speed.x = -SPEED_X;
	speed.y = 0;
}

void Unit::arrive()
{
	status = UNIT_IDLE;				
	speed.x = 0;
	speed.y = 0;
	mPosition = path.back();	
	mPosition->habitant = this;
	mPosition->locked = false;
	path.pop_back();
}

void Unit::onDeath()
{	
	mPosition->locked = false;	
	decomposed = true;
}

void Unit::enableAttack()
{
	attackCooldown = false;
}

void Unit::receiveDamage(int damage)
{
	attributes.hitpoints -= (damage - attributes.defense);
	if(attributes.hitpoints <= 0)
	{
		status = UNIT_DEAD;
		mPosition->habitant = NULL;	
		mPosition->locked = false;	
	}
}

void Unit::attack()
{
	ATTACK_READY_TIME = Timer::get_currentFrameTick() + backswingTime;
	attackCooldown = true;
	target->receiveDamage(attributes.damage);
	if(target->status == UNIT_DEAD)
	{
		status = UNIT_IDLE;
		target = NULL;
	}
}

void Unit::startAttack(Unit* newTarget)
{
	status = UNIT_ATTACKING;
	target = newTarget;
}

void Unit::getTarget()
{
	//cout << "Attacking wall!" << endl;
	//startAttack(this);
}
