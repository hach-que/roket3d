#ifndef HEADER_RTEXTURE
#define HEADER_RTEXTURE
namespace video
{
	class RTexture : public RObject
	{
		irr::video::ITexture* base_self;
		bool textureExists;
		
		public:
			RTexture(lua_State* L, bool instantedByUser);
			void setTexture(irr::video::ITexture* tex);
			irr::video::ITexture* getTexture();
			//int drawAll(lua_State* L);
			
			static const char *className;
			static const Luna < RTexture >::FunctionType Functions[];
			static const Luna < RTexture >::PropertyType Properties[];
	};
}
#endif
