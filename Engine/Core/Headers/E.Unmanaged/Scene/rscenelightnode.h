#ifndef HEADER_RSCENELIGHTNODE
#define HEADER_RSCENELIGHTNODE

#include "../RoketGraphics/XEffects/Source/effectWrapper.h"
#include "../RoketGraphics/XEffects/Source/effectWrapperCB.h"

namespace scene
{
	class RLightSceneNode : public RSceneNode
	{
		irr::scene::ILightSceneNode * extended_self;
		irr::scene::ISceneNode *&self() { return (irr::scene::ISceneNode*&)extended_self; };
		irr::u32 shadowLightID;
		irr::video::SColor lightColor;
		effectHandler* xeffect;
		irr::core::vector3df targetPosition;
		core::RDevice* roketdevice;
		bool isRelativeTarget_setting;
		
		public:
			RLightSceneNode(lua_State* L, bool instantedByUser);
			//int drawAll(lua_State* L);
			virtual void setNode(irr::scene::ILightSceneNode* lightnode, effectHandler* xe);
			virtual irr::scene::ILightSceneNode* getNode();
			
			virtual void setRoketDevice(core::RDevice* rd);
			virtual void setupShadows(irr::core::vector3df start, core::RDevice* rd, effectHandler* xe, irr::video::SColor lc, float nearValue, float farValue);
			int setPosition(lua_State* L);

			int setTargetPosition(lua_State* L);
			int getTargetPosition(lua_State* L);
			int isRelativeTarget(lua_State* L);
			int setRelativeTarget(lua_State* L);

			static const char *className;
			static const Luna < RLightSceneNode >::FunctionType Functions[];
			static const Luna < RLightSceneNode >::PropertyType Properties[];
	};
}
#endif
