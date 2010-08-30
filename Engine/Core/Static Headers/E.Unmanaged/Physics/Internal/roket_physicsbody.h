#ifndef HEADER_ROKETPHYSICSBODY
#define HEADER_ROKETPHYSICSBODY
#include "E.Unmanaged/Scene/rscenenode.h"

namespace physics
{
	class Roket_PhysicsBody
	{
		public:
			NewtonBody * body;

		private:
			NewtonWorld * world;
			scene::RSceneNode* node;
			bool objectExists;
	
			bool affectedByGravity;
			irr::core::array<irr::core::vector3df> forcesToApply;
			irr::core::array<irr::core::vector3df> torqueToApply;
		
		public:
			Roket_PhysicsBody(NewtonCollision* collision, NewtonWorld* physworld, ePhysicsType type, int materialid);
			~Roket_PhysicsBody();
			cIrrVector3D getPhysicsPosition();
			cIrrVector3D getPhysicsRotation();
			//cIrrVector3D getPhysicsScale();
			
			void setPhysicsSettings(dFloat mass, irr::core::vector3df inertia);
			void setPosition(irr::core::vector3df vec);
			void setRotation(irr::core::vector3df vec);
			void applyForce(irr::core::vector3df force);
			void applyTorque(irr::core::vector3df torque);
			dFloat getPhysicsMass();
			irr::core::vector3d<dFloat> getPhysicsInertia();
			void attachNode(scene::RSceneNode* nn);

			void deletePhysics();

			/* UPDATE BODY FUNCTIONS */
			void updateSelf(const dFloat* matrix);
			void updateForce();
			void updateTorque();
	};
}
#endif
