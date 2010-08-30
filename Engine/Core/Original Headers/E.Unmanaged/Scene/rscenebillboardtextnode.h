#ifndef HEADER_RSCENEBILLBOARDTEXTNODE
#define HEADER_RSCENEBILLBOARDTEXTNODE
namespace scene
{
	class RBillboardTextSceneNode : public RSceneNode
	{
		irr::scene::IBillboardTextSceneNode * extended_self;
		irr::scene::ISceneNode *&self() { return (irr::scene::ISceneNode*&)extended_self; };
		
		public:
			RBillboardTextSceneNode(lua_State* L, bool instantedByUser);
			//int drawAll(lua_State* L);
			virtual void setNode(irr::scene::IBillboardTextSceneNode* node, effectHandler* xe);
			virtual irr::scene::IBillboardTextSceneNode* getNode();

			int setSize(lua_State* L);
			int getSize(lua_State* L);
			int setText(lua_State* L);
			int setTextColor(lua_State* L);

			static const char *className;
			static const Luna < RBillboardTextSceneNode >::FunctionType Functions[];
			static const Luna < RBillboardTextSceneNode >::PropertyType Properties[];
	};
}
#endif
