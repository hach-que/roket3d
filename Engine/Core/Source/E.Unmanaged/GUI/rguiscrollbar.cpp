#include "roket3d.h"
#include "E.Unmanaged/GUI/rguiscrollbar.h"

namespace gui
{
	const char *RGUIScrollBar::className = TYPE_RGUISCROLLBAR;
	const Luna < RGUIScrollBar >::FunctionType RGUIScrollBar::Functions[] = {
		//{"drawAll", &RGUIScrollBar::drawAll},
		{0}
	};
	const Luna < RGUIScrollBar >::PropertyType RGUIScrollBar::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RGUIScrollBar::RGUIScrollBar(lua_State* L, bool instantedByUser)
	{
	}
	
	int RGUIScrollBar::getLargeStep(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getLargeStep());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIScrollBar::getSmallStep(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getSmallStep());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIScrollBar::getPos(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getPos());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIScrollBar::getMax(lua_State* L)
	{
		ifistrue(isElementSet);
		
		lua_pushnumber(L,extended_self->getMax());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIScrollBar::setLargeStep(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setLargeStep(getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIScrollBar::setSmallStep(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setLargeStep(getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIScrollBar::setPos(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setLargeStep(getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	
	int RGUIScrollBar::setMax(lua_State* L)
	{
		ifistrue(isElementSet);
		
		extended_self->setLargeStep(getmandatoryargument_cNumber(L,1));
		return 0;
		
		endifisvalid();
	}
	
	void RGUIScrollBar::setElement(irr::gui::IGUIScrollBar* elem)
	{
		extended_self = elem;
		isElementSet = true;
	}
	
	irr::gui::IGUIScrollBar* RGUIScrollBar::getElement()
	{
		return extended_self;
	}
}
