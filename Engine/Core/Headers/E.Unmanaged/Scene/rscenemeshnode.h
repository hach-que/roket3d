#ifndef HEADER_RSCENEMESHNODE
#define HEADER_RSCENEMESHNODE
namespace scene
{
	class RMeshSceneNode : public RSceneNode
	{
		irr::scene::IAnimatedMeshSceneNode * extended_self;
		irr::scene::ISceneNode *&self() { return (irr::scene::ISceneNode*&)extended_self; };
		
		public:
			RMeshSceneNode(lua_State* L, bool instantedByUser);
			//int drawAll(lua_State* L);
			virtual void setNode(irr::scene::IAnimatedMeshSceneNode* meshnode, effectHandler* xe);
			virtual irr::scene::IAnimatedMeshSceneNode* getNode();
			
			static const char *className;
			static const Luna < RMeshSceneNode >::FunctionType Functions[];
			static const Luna < RMeshSceneNode >::PropertyType Properties[];
	};
}
#endif
