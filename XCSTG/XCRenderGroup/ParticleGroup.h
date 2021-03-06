#pragma once
#ifndef _PARTICLEGROUP_H_
#define _PARTICLEGROUP_H_
#include "../XCSpecialEffect/XCParticle.h"
#include "../util/GameTimer.h"
#include <vector>
class ParticleGroup {
protected:
	float render_abs_width = 1.0f, render_abs_height = 1.0f;
	xc_se::XCParticle *StorageParticleGroup=nullptr;
	int groupRenderType= DISPERSE_COORD,particle_count=0;
	XCGameTimer particleGroupTimer;
public:
	/*!Particle group coord type.*/
	enum GroupType {ALL_ONE_COORD, ROTATE_COORD, DISPERSE_COORD};
	/*!Single particle render type.*/
	enum ParticleType { CIRCLE_PARTICLE, MOVING_PARTICLE};
	void SetGroupRenderType(int groupType);
	virtual void GroupInit(int type,int count,float rendertime);
	/*Return whether particle render finish.*/
	virtual bool GroupRender(float x,float y,float z,float nowFrame);
	virtual void GroupRelease();
	void SetAbsWidthAndHeight(float absW, float absH);
};
#endif