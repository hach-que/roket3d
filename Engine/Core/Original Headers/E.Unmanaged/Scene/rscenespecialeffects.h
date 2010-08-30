#ifndef HEADER_RSCENESPECIALEFFECTS
#define HEADER_RSCENESPECIALEFFECTS

namespace scene
{
	class RSceneSpecialEffects : public RObject
	{
		effectHandler* xeffect;
		core::RDevice* device;
		bool objectExists;
		
		public:
			RSceneSpecialEffects(lua_State* L, bool instantedByUser);
			RSceneSpecialEffects();
			~RSceneSpecialEffects();

			void setObject(effectHandler* xe, core::RDevice* dev);
			effectHandler* getObject();

			int addPostProcessor(lua_State* L);
			int removePostProcessor(lua_State* L);
			int setShaderPath(lua_State* L);
			int getShaderPath(lua_State* L);

			static const char *className;
			static const Luna < RSceneSpecialEffects >::FunctionType Functions[];
			static const Luna < RSceneSpecialEffects >::PropertyType Properties[];
	};
}
#endif
