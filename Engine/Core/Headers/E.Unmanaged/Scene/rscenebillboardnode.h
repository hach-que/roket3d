#ifndef HEADER_RSCENEBILLBOARDNODE
#define HEADER_RSCENEBILLBOARDNODE
namespace scene
{
	class RBillboardSceneNode : public RSceneNode
	{
		irr::scene::IBillboardSceneNode * extended_self;
		irr::scene::ISceneNode *&self() { return (irr::scene::ISceneNode*&)extended_self; };
		
		public:
			RBillboardSceneNode(lua_State* L, bool instantedByUser);
			//int drawAll(lua_State* L);
			virtual void setNode(irr::scene::IBillboardSceneNode* node, effectHandler* xe);
			virtual irr::scene::IBillboardSceneNode* getNode();

			int setSize(lua_State* L);
			int getSize(lua_State* L);
			
			static const char *className;
			static const Luna < RBillboardSceneNode >::FunctionType Functions[];
			static const Luna < RBillboardSceneNode >::PropertyType Properties[];
	};
}
#endif
