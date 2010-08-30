#include "roket3d.h"
#include "E.Unmanaged/GUI/rguienvironment.h"

namespace gui
{
	const char *RGUIEnvironment::className = TYPE_RGUIENVIRONMENT;
	const Luna < RGUIEnvironment >::FunctionType RGUIEnvironment::Functions[] = {
		{"drawAll", &RGUIEnvironment::drawAll},
  {"addButton", &RGUIEnvironment::addButton},
  {"addCheckBox", &RGUIEnvironment::addCheckBox},
  {"addEditBox", &RGUIEnvironment::addEditBox},
  {"addComboBox", &RGUIEnvironment::addComboBox},
  {"addListBox", &RGUIEnvironment::addListBox},
  {"addStaticText", &RGUIEnvironment::addStaticText},
  {"addFileOpenDialog", &RGUIEnvironment::addFileOpenDialog},
  {"addWindow", &RGUIEnvironment::addWindow},
  {"addImage", &RGUIEnvironment::addImage},
  {"getFont", &RGUIEnvironment::getFont},
  {"getSkin", &RGUIEnvironment::getSkin},
		{0}
	};
	const Luna < RGUIEnvironment >::PropertyType RGUIEnvironment::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	int RGUIEnvironment::setEnv(irr::gui::IGUIEnvironment* enviro)
	{
		env=enviro;
		isEnvSet=true;
		return 0;
	}
	
	int RGUIEnvironment::drawAll(lua_State* L)
	{
		if (isEnvSet)
			env->drawAll();
		return 0;
	}
	
	int RGUIEnvironment::getSkin(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		gui::RGUISkin* skn = Luna < gui::RGUISkin >::createNew(L);
		skn->setSkin(env->getSkin());
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::getFont(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		const char* filename = getmandatoryargument_constchar(L,1);
		if (!fileExists(filename))
		{
			debug(FILE_NOT_FOUND("load font",filename).c_str(),LVL_ERR,L);
			return 0;
		}
		
		gui::RGUIFont* font = Luna < gui::RGUIFont >::createNew(L);
		font->setFont(env->getFont(filename));
		return 1;
		
		endifisvalid();
	}
	
	irr::gui::IGUIElement* RGUIEnvironment::getParent(lua_State* L, int pos)
	{
		gui::RGUIElement* elem = 0;
		elem = Luna<gui::RGUIElement>::lightcheck(L, pos);
		elem = Luna<gui::RGUIFileOpenDialog>::lightcheck(L, pos);
		elem = Luna<gui::RGUICombobox>::lightcheck(L, pos);
		elem = Luna<gui::RGUIListbox>::lightcheck(L, pos);
		elem = Luna<gui::RGUICheckbox>::lightcheck(L, pos);
		elem = Luna<gui::RGUIImage>::lightcheck(L, pos);
		elem = Luna<gui::RGUIWindow>::lightcheck(L, pos);
		elem = Luna<gui::RGUIMeshViewer>::lightcheck(L, pos);
		elem = Luna<gui::RGUIScrollBar>::lightcheck(L, pos);
		elem = Luna<gui::RGUIWindow>::lightcheck(L, pos);
		if (elem != NULL)
		{
			return elem->getElement();
		}
		else
		{
			return NULL;
		}
	}
	
	int RGUIEnvironment::addButton(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);
		int id = getoptionalargument(L,2,-1);
		const char* name = getoptionalargument(L,3,"");
		irr::gui::IGUIElement* parent = getParent(L,4);

		gui::RGUIButton* elem = Luna < gui::RGUIButton >::createNew(L);
		irr::gui::IGUIButton* ielem = env->addButton(rect->getObjectS32(),parent,id);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::addCheckBox(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);
		int id = getoptionalargument(L,2,-1);
		const char* name = getoptionalargument(L,3,"");
		irr::gui::IGUIElement* parent = getParent(L,4);
		
		gui::RGUICheckbox* elem = Luna < gui::RGUICheckbox >::createNew(L);
		irr::gui::IGUICheckBox* ielem = env->addCheckBox(false,rect->getObjectS32(),parent,id);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::addEditBox(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);
		int id = getoptionalargument(L,2,-1);
		const char* name = getoptionalargument(L,3,"");
		irr::gui::IGUIElement* parent = getParent(L,4);
		
		gui::RGUIEditBox* elem = Luna < gui::RGUIEditBox >::createNew(L);
		irr::gui::IGUIEditBox* ielem = env->addEditBox(ConvertToWideChar(""),rect->getObjectS32(),true,parent,id);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::addComboBox(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);
		int id = getoptionalargument(L,2,-1);
		const char* name = getoptionalargument(L,3,"");
		irr::gui::IGUIElement* parent = getParent(L,4);
		
		gui::RGUICombobox* elem = Luna < gui::RGUICombobox >::createNew(L);
		irr::gui::IGUIComboBox* ielem = env->addComboBox(rect->getObjectS32(),parent,id);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::addListBox(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);
		int id = getoptionalargument(L,2,-1);
		const char* name = getoptionalargument(L,3,"");
		irr::gui::IGUIElement* parent = getParent(L,4);
		
		gui::RGUIListbox* elem = Luna < gui::RGUIListbox >::createNew(L);
		irr::gui::IGUIListBox* ielem = env->addListBox(rect->getObjectS32(),parent,id);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::addStaticText(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);
		int id = getoptionalargument(L,2,-1);
		const char* name = getoptionalargument(L,3,"");
		irr::gui::IGUIElement* parent = getParent(L,4);
		
		gui::RGUIStaticText* elem = Luna < gui::RGUIStaticText >::createNew(L);
		irr::gui::IGUIStaticText* ielem = env->addStaticText(ConvertToWideChar(""),rect->getObjectS32(),true,false,parent,id,true);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::addImage(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);
		int id = getoptionalargument(L,2,-1);
		const char* name = getoptionalargument(L,3,"");
		irr::gui::IGUIElement* parent = getParent(L,4);
		
		gui::RGUIImage* elem = Luna < gui::RGUIImage >::createNew(L);
		irr::gui::IGUIImage* ielem = env->addImage(rect->getObjectS32(),parent,id);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::addFileOpenDialog(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		const char* title = getoptionalargument(L,1,"Select File");
		bool modal = getoptionalargument(L,2,true);
		int id = getoptionalargument(L,3,-1);
		const char* name = getoptionalargument(L,4,"");
		irr::gui::IGUIElement* parent = getParent(L,5);
		
		gui::RGUIFileOpenDialog* elem = Luna < gui::RGUIFileOpenDialog >::createNew(L);
		irr::gui::IGUIFileOpenDialog* ielem = env->addFileOpenDialog(ConvertToWideChar(title),modal,parent,id);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	int RGUIEnvironment::addWindow(lua_State* L)
	{
		ifistrue(isEnvSet);
		
		core::RRectangle* rect = Luna<core::RRectangle>::check(L,1);
		const char* title = getmandatoryargument_constchar(L,2);
		bool modal = getoptionalargument(L,3,true);
		int id = getoptionalargument(L,4,-1);
		const char* name = getoptionalargument(L,5,"");
		irr::gui::IGUIElement* parent = getParent(L,6);
		
		gui::RGUIWindow* elem = Luna < gui::RGUIWindow >::createNew(L);
		irr::gui::IGUIWindow* ielem = env->addWindow(rect->getObjectS32(),modal,ConvertToWideChar(title),parent,id);
		ielem->setID(id);
		ielem->RoketSetName(name);
		elem->setElement(ielem);
		return 1;
		
		endifisvalid();
	}
	
	irr::gui::IGUIEnvironment* RGUIEnvironment::getEnvC()
	{
		if (isEnvSet)
			return env;
		else
			return NULL;
	}
	
	RGUIEnvironment::RGUIEnvironment(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RGUIEnvironments.  You must collect one from an RDevice.",LVL_ERR,L);
	}
}
