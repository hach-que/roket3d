#ifndef HEADER_RPHYSICSJOINT
#define HEADER_RPHYSICSJOINT

#include "roket_physicsjointball.h"
#include "roket_physicsjointhinge.h"
#include "roket_physicsjointslider.h"
#include "roket_physicsjointcorkscrew.h"
#include "roket_physicsjointuniversal.h"

namespace physics
{
	class RPhysicsJoint : public RObject
	{
		Roket_PhysicsJointBall      * obj_ball;
		Roket_PhysicsJointHinge     * obj_hinge;
		Roket_PhysicsJointSlider    * obj_slider;
		Roket_PhysicsJointCorkscrew * obj_corkscrew;
		Roket_PhysicsJointUniversal * obj_universal;
		// add custom physics joint here later
		bool objectExists;

		ePhysicsJointType jointType;

		public:
			RPhysicsJoint(lua_State* L, bool instantedByUser);
			// Returns the type of joint.
			//  _JOINT_BALL_AND_SOCKET
			//  _JOINT_BALL (same as _JOINT_BALL_AND_SOCKET)
			//  _JOINT_HINGE
			//  _JOINT_SLIDER
			//  _JOINT_CORKSCREW
			//  _JOINT_UNIVERSAL
			int getType(lua_State* L);
			int setStiffness(lua_State* L);
			int getStiffness(lua_State* L);
			int destroy(lua_State* L);

			// Ball functions
			int setLimits(lua_State* L);
			int getOmega(lua_State* L);
			int getForce(lua_State* L);
			int getAngle(lua_State* L);
			int getPosition(lua_State* L);
			int getVelocity(lua_State* L);

			// C++ version
			ePhysicsJointType getType();

			int setObjectBallAndSocket		(Roket_PhysicsJointBall* object);
			int setObjectHinge				(Roket_PhysicsJointHinge* object);
			int setObjectSlider				(Roket_PhysicsJointSlider* object);
			int setObjectCorkscrew			(Roket_PhysicsJointCorkscrew* object);
			int setObjectUniversal			(Roket_PhysicsJointUniversal* object);
			Roket_PhysicsJointBall*			getObjectBallAndSocket();
			Roket_PhysicsJointHinge*		getObjectHinge();
			Roket_PhysicsJointSlider*		getObjectSlider();
			Roket_PhysicsJointCorkscrew*	getObjectCorkscrew();
			Roket_PhysicsJointUniversal*	getObjectUniversal();

			void setInvalid();

			static const char *className;
			static const Luna < RPhysicsJoint >::FunctionType Functions[];
			static const Luna < RPhysicsJoint >::PropertyType Properties[];
	};
}
#endif
