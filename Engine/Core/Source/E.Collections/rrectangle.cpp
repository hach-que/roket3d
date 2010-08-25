#include "roket3d.h"
#include "E.Collections/rrectangle.h"

namespace core
{
	const char *RRectangle::className = TYPE_RECTANGLE;
	const Luna < RRectangle >::FunctionType RRectangle::Functions[] = {
  {"addInternalPoint", &RRectangle::addInternalPoint},
  {"clipAgainst", &RRectangle::clipAgainst},
  {"constrainTo", &RRectangle::constrainTo},
  {"getArea", &RRectangle::getArea},
  {"getWidth", &RRectangle::getWidth},
  {"getHeight", &RRectangle::getHeight},
  {"getSize", &RRectangle::getSize},
  {"isPointInside", &RRectangle::isPointInside},
  {"isRectCollided", &RRectangle::isRectCollided},
  {"isValid", &RRectangle::isValid},
  {"repair", &RRectangle::repair},
  {0}
	};
	const Luna < RRectangle >::PropertyType RRectangle::Properties[] = {
		{"UpperLeft", &RRectangle::getUpperLeft, &RRectangle::setUpperLeft},
  {"LowerRight", &RRectangle::getLowerRight, &RRectangle::setLowerRight},
  {0}
	};
	
	
	RRectangle::RRectangle(lua_State* L, bool instantedByUser)
	{
		if (lua_isnil(L,1))
		{
			// Form 1
			irr_rect = irr::core::rect<cIrrNumber>(0,0,0,0);
			objectExists = true;
		}
		else
		{
			// Form 2
			
			// Get top left
			core::RVector2D *p1 = Luna<core::RVector2D>::check(L, 0);
			irr::core::vector2df p1i = p1->getObject();
			
			// Get bottom right
			core::RVector2D *p2 = Luna<core::RVector2D>::check(L, 1);
			irr::core::vector2df p2i = p2->getObject();
			
			topLeft = p1->getObjectDouble();
			bottomRight = p2->getObjectDouble();
			irr_rect = irr::core::rect<cIrrNumber>(p1i.X,p1i.Y,p2i.X,p2i.Y);
			objectExists = true;
		}
	}
	
	int RRectangle::setObject(irr::core::rect<cIrrNumber> object)
	{
		irr_rect = object;
		objectExists = true;
		return 0;
	}

	irr::core::rect<cIrrNumber> RRectangle::getObject()
	{
		if (objectExists)
			return irr_rect;
		else
			return irr::core::rect<cIrrNumber>(0,0,0,0);
	}

	irr::core::rect<irr::s32> RRectangle::getObjectS32()
	{
		if (objectExists)
		{
			irr::core::rect<irr::s32> newrect;
			irr::core::position2d<irr::s32> newul;
			irr::core::position2d<irr::s32> newlr;
			newul.X = irr_rect.UpperLeftCorner.X;
			newul.Y = irr_rect.UpperLeftCorner.Y;
			newlr.X = irr_rect.LowerRightCorner.X;
			newlr.Y = irr_rect.LowerRightCorner.Y;
			newrect.LowerRightCorner = newlr;
			newrect.UpperLeftCorner = newul;
			return newrect;
		}
		else
			return irr::core::rect<irr::s32>(0,0,0,0);
	}

	int RRectangle::addInternalPoint(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the line
		core::RVector2D *vector = Luna<core::RVector2D>::check(L, 1);
		irr_rect.addInternalPoint(vector->getObjectAsPosition());
		return 0;
		
		endifisvalid();
	}

	int RRectangle::clipAgainst(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other rectangle
		core::RRectangle *otherrect = Luna<core::RRectangle>::check(L, 1);
		irr_rect.clipAgainst(otherrect->getObject());
		return 0;
		
		endifisvalid();
	}

	int RRectangle::constrainTo(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other rectangle
		core::RRectangle *otherrect = Luna<core::RRectangle>::check(L, 1);
		lua_pushboolean(L,irr_rect.constrainTo(otherrect->getObject()));
		return 1;
		
		endifisvalid();
	}

	int RRectangle::getArea(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_rect.getArea());
		return 1;
		
		endifisvalid();
	}

	int RRectangle::getCenter(lua_State* L)
	{
		ifistrue(objectExists);
		
		// get middle point
		RVector2D* returnvector = Luna < RVector2D >::createNew(L);
		irr::core::position2df pos = irr_rect.getCenter();
		irr::core::vector2df newvec;
		newvec.X = pos.X;
		newvec.Y = pos.Y;
		returnvector->setObject(newvec);
		return 1;
		
		endifisvalid();
	}

	int RRectangle::getWidth(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_rect.getWidth());
		return 1;
		
		endifisvalid();
	}

	int RRectangle::getHeight(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushnumber(L,irr_rect.getHeight());
		return 1;
		
		endifisvalid();
	}

	int RRectangle::getSize(lua_State* L)
	{
		ifistrue(objectExists);
		
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		irr::core::vector2df newvec;
		irr::core::dimension2df pos = irr_rect.getSize();
		newvec.X = pos.Width;
		newvec.Y = pos.Height;
		vector->setObject(newvec);
		return 1;
		
		endifisvalid();
	}

	int RRectangle::isPointInside(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the position
		core::RVector2D *pos = Luna<core::RVector2D>::check(L, 1);
		lua_pushboolean(L,irr_rect.isPointInside(irr::core::position2df(pos->getObject().X,pos->getObject().Y)));
		return 1;
		
		endifisvalid();
	}

	int RRectangle::isRectCollided(lua_State* L)
	{
		ifistrue(objectExists);
		
		// Get the other rectangle
		core::RRectangle *otherrect = Luna<core::RRectangle>::check(L, 1);
		lua_pushboolean(L,irr_rect.isRectCollided(otherrect->getObject()));
		return 1;

		endifisvalid();
	}

	int RRectangle::isValid(lua_State* L)
	{
		ifistrue(objectExists);
		
		lua_pushboolean(L,irr_rect.isValid());
		return 1;

		endifisvalid();
	}

	int RRectangle::repair(lua_State* L)
	{
		ifistrue(objectExists);
		
		irr_rect.repair();
		return 1;

		endifisvalid();
	}

	// **************** BEGIN PROPERTY FUNCTIONS ***************************
	
	int RRectangle::getUpperLeft(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_rect.UpperLeftCorner.X,(irr::f32)topLeft.X) ||
			!irr::core::equals(irr_rect.UpperLeftCorner.Y,(irr::f32)topLeft.Y))
		{
			topLeft.X = irr_rect.UpperLeftCorner.X;
			topLeft.Y = irr_rect.UpperLeftCorner.Y;
		}
		// Get the upper left Vector
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		irr::core::vector2d<double> newvec = topLeft;
		vector->setObjectDouble(newvec);
		return 1;
		
		endifisvalid();
	}
	
	int RRectangle::getLowerRight(lua_State* L)
	{
		ifistrue(objectExists);
		
		if (!irr::core::equals(irr_rect.LowerRightCorner.X,(irr::f32)bottomRight.X) ||
			!irr::core::equals(irr_rect.LowerRightCorner.Y,(irr::f32)bottomRight.Y))
		{
			bottomRight.X = irr_rect.LowerRightCorner.X;
			bottomRight.Y = irr_rect.LowerRightCorner.Y;
		}
		// Get the bottom right Vector
		RVector2D* vector = Luna < RVector2D >::createNew(L);
		irr::core::vector2d<double> newvec = bottomRight;
		vector->setObjectDouble(newvec);
		return 1;
		
		endifisvalid();
	}
	
	int RRectangle::setUpperLeft(lua_State* L)
	{
		ifistrue(objectExists);

		// TODO: Check argument check is correct.
		core::RVector2D *p1 = Luna<core::RVector2D>::check(L, -1);
		
		topLeft = p1->getObjectDouble();
		irr_rect.UpperLeftCorner = p1->getObjectAsPosition();
		return 0;
		
		endifisvalid();
	}
	
	int RRectangle::setLowerRight(lua_State* L)
	{
		ifistrue(objectExists);
		
		// TODO: Check argument check is correct.
		core::RVector2D *p2 = Luna<core::RVector2D>::check(L, -1);
		
		bottomRight = p2->getObjectDouble();
		irr_rect.LowerRightCorner = p2->getObjectAsPosition();
		return 0;
		
		endifisvalid();
	}
}
