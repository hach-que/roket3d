#ifndef HEADER_RMESH
#define HEADER_RMESH
namespace scene
{
	class RMesh : public RObject
	{
		irr::scene::IAnimatedMesh* base_self;
		//virtual irr::scene::IMesh *&self() { return base_self; };
		
		public:
			RMesh(lua_State* L, bool instantedByUser);
			void setIrrlichtMesh(irr::scene::IAnimatedMesh* mesh);
			irr::scene::IAnimatedMesh* getIrrlichtMesh();
			
			static const char *className;
			static const Luna < RMesh >::FunctionType Functions[];
			static const Luna < RMesh >::PropertyType Properties[];
	};
}
#endif
