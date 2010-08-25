#ifndef HEADER_RSCENETERRAINNODE
#define HEADER_RSCENETERRAINNODE

namespace scene
{
	class RTerrainSceneNode : public RSceneNode
	{
		irr::scene::ITerrainSceneNode * extended_self;
		irr::scene::ISceneNode *&self() { return (irr::scene::ISceneNode*&)extended_self; };

		//irr::scene::IShadowVolumeSceneNode * shadow_volume;
		
		public:
			RTerrainSceneNode(lua_State* L, bool instantedByUser);
			int getHeight(lua_State* L);
			virtual void setNode(irr::scene::ITerrainSceneNode* meshnode, irr::scene::ISceneManager* mngr, effectHandler* xe);
			virtual irr::scene::ITerrainSceneNode* getNode();
			
			static const char *className;
			static const Luna < RTerrainSceneNode >::FunctionType Functions[];
			static const Luna < RTerrainSceneNode >::PropertyType Properties[];
	};
}
#endif
