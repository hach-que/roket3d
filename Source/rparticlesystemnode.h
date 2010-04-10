#ifndef HEADER_RPARTICLESYSTEMNODE
#define HEADER_RPARTICLESYSTEMNODE
namespace scene
{
	class RParticleSystemSceneNode : public RSceneNode
	{
		irr::scene::IParticleSystemSceneNode * extended_self;
		irr::scene::ISceneNode *&self() { return (irr::scene::ISceneNode*&)extended_self; };
		
		public:
			RParticleSystemSceneNode(lua_State* L, bool instantedByUser);
			//int drawAll(lua_State* L);
			virtual void setNode(irr::scene::IParticleSystemSceneNode* meshnode, effectHandler* xe);
			virtual irr::scene::IParticleSystemSceneNode* getNode();
			int createBoxEmitter(lua_State* L);
			int createSphereEmitter(lua_State* L);
			int createRingEmitter(lua_State* L);
			int createPointEmitter(lua_State* L);
			
			static const char *className;
			static const Luna < RParticleSystemSceneNode >::FunctionType Functions[];
			static const Luna < RParticleSystemSceneNode >::PropertyType Properties[];
	};
}
#endif
