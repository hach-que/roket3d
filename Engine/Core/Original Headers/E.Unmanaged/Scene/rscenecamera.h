#ifndef HEADER_RSCENECAMERA
#define HEADER_RSCENECAMERA
namespace scene
{
	class RSceneCamera : public RSceneNode
	{
		irr::scene::ICameraSceneNode * extended_self;
		irr::scene::ISceneNode *&self() { return (irr::scene::ISceneNode*&)extended_self; };
		
		public:
			RSceneCamera(lua_State* L, bool instantedByUser);
			int setSourcePosition(lua_State* L);
			int getSourcePosition(lua_State* L);
			int setDestinationPosition(lua_State* L);
			int getDestinationPosition(lua_State* L);
			int setUpVectorPosition(lua_State* L);
			
			virtual void setNode(irr::scene::ICameraSceneNode* meshnode, effectHandler* xe);
			virtual irr::scene::ICameraSceneNode* getNode();
			
			static const char *className; 
			static const Luna < RSceneCamera >::FunctionType Functions[];
			static const Luna < RSceneCamera >::PropertyType Properties[];
	};
}
#endif
