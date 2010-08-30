#ifndef HEADER_RRECTANGLE
#define HEADER_RRECTANGLE
namespace core
{
	class RRectangle : public RObject
	{
		
		// This is the 2D rectangle class.
		
		irr::core::rect<cIrrNumber> irr_rect;
		bool objectExists;
		
		// More accurate precision for points.
		irr::core::vector2d<double> topLeft;
		irr::core::vector2d<double> bottomRight;

		public:
			RRectangle(lua_State* L, bool instantedByUser);
			int addInternalPoint(lua_State* L);
			int clipAgainst(lua_State* L);
			int constrainTo(lua_State* L);
			int getArea(lua_State* L);
			int getCenter(lua_State* L);
			int getHeight(lua_State* L);
			int getWidth(lua_State* L);
			int getSize(lua_State* L);
			int isPointInside(lua_State* L);
			int isRectCollided(lua_State* L);
			int isValid(lua_State* L);
			int repair(lua_State* L);
			
			int setUpperLeft(lua_State* L);
			int setLowerRight(lua_State* L);
			int getUpperLeft(lua_State* L);
			int getLowerRight(lua_State* L);

			int setObject(irr::core::rect<cIrrNumber> object);
			irr::core::rect<cIrrNumber> getObject();
			irr::core::rect<irr::s32> getObjectS32();

			static const char *className;
			static const Luna < RRectangle >::FunctionType Functions[];
			static const Luna < RRectangle >::PropertyType Properties[];
	};
}
#endif
