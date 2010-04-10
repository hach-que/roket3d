#ifndef HEADER_EVENTRECIEVER
#define HEADER_EVENTRECIEVER

#include "rguielement.h"
#include "rguiwindow.h"
#include "rscenenode.h"

class RoketEventReceiver : public irr::IEventReceiver
{
	irr::core::stringc luaGUIEventCapturer;
	irr::core::stringc luaMouseEventCapturer;
	irr::core::stringc luaKeyboardEventCapturer;
	irr::core::stringc luaPhysicsEventCapturer;
	lua_State* eventLocation; // used to call functions in the correct state (for threads)
	bool allIsSame;
	
	public:
		// Variables from RObject because we already have a base class
		bool isExisting;
		bool isPrecious;
		
		virtual bool OnEvent(const irr::SEvent& event)
		{
			if (event.EventType == irr::EET_GUI_EVENT)
			{
				if (event.GUIEvent.EventType==irr::gui::EGET_ELEMENT_CLOSED)
				{
					// window is closing
					if (event.GUIEvent.Element != NULL)
					{
						if (strcmp(event.GUIEvent.Element->getDebugName(),"CGUIWindow")==0)
						{
							irr::gui::IGUIWindow * wind = reinterpret_cast<irr::gui::IGUIWindow*>(event.GUIEvent.Element);
							gui::RGUIWindow * rwind = reinterpret_cast<gui::RGUIWindow*>(wind->RoketGetUserData());
							rwind->windowSetClosed();
						}
					}
				}
				if (luaGUIEventCapturer=="") return false;
				
				bool useID = false;
				irr::core::stringc elementname = event.GUIEvent.Caller->RoketGetName();
				irr::s32 id;
				if (elementname=="")
				{
					id = event.GUIEvent.Caller->getID();
					useID = true;
				}
				irr::core::stringc myeventcall;
				lua_getglobal(eventLocation, luaGUIEventCapturer.c_str());
				if (lua_type(eventLocation, -1)==LUA_TFUNCTION) {
					irr::core::stringc eventtype;
					eventtype=""; 
					switch (event.GUIEvent.EventType)
					{
						case irr::gui::EGET_ELEMENT_FOCUS_LOST: eventtype="FocusLost"; break;
						case irr::gui::EGET_ELEMENT_FOCUSED: eventtype="FocusGained"; break;
						case irr::gui::EGET_ELEMENT_HOVERED: eventtype="MouseOver"; break;
						case irr::gui::EGET_ELEMENT_LEFT: eventtype="MouseOut"; break;
						case irr::gui::EGET_ELEMENT_CLOSED: eventtype="Closing"; break;
						case irr::gui::EGET_BUTTON_CLICKED: eventtype="Clicked"; break;
						case irr::gui::EGET_SCROLL_BAR_CHANGED: eventtype="ValueChanged"; break;
						case irr::gui::EGET_CHECKBOX_CHANGED: eventtype="CheckedChanged"; break;
						case irr::gui::EGET_LISTBOX_CHANGED: eventtype="ItemSelected"; break;
						case irr::gui::EGET_LISTBOX_SELECTED_AGAIN: eventtype="ItemDoubleSelected"; break;
						case irr::gui::EGET_FILE_SELECTED: eventtype="FileSelected"; break;
						case irr::gui::EGET_FILE_CHOOSE_DIALOG_CANCELLED: eventtype="FileCancelled"; break;
						case irr::gui::EGET_MESSAGEBOX_YES: eventtype="MessageboxYes"; break;
						case irr::gui::EGET_MESSAGEBOX_NO: eventtype="MessageboxNo"; break;
						case irr::gui::EGET_MESSAGEBOX_OK: eventtype="MessageboxOK"; break;
						case irr::gui::EGET_MESSAGEBOX_CANCEL: eventtype="MessageboxCancel"; break;
						case irr::gui::EGET_EDITBOX_ENTER: eventtype="TextChanged"; break;
						case irr::gui::EGET_TAB_CHANGED: eventtype="TabChanged"; break;
						case irr::gui::EGET_MENU_ITEM_SELECTED: eventtype="ItemSelected"; break;
						case irr::gui::EGET_COMBO_BOX_CHANGED: eventtype="ItemSelected"; break;
						case irr::gui::EGET_SPINBOX_CHANGED: eventtype="ValueChanged"; break;
					}
					lua_getfield(RoketLUA, LUA_GLOBALSINDEX, "__internal_luaErrorHandler_errOnly");
					int posOnStack = 0;
					
					// build the function call
					lua_getglobal(eventLocation, luaGUIEventCapturer.c_str()); posOnStack++;
					if (allIsSame)
					{
						lua_pushnumber(eventLocation, 0); posOnStack++;
					}
					lua_pushstring(eventLocation, eventtype.c_str()); posOnStack++;
					if (useID)
					{
						lua_pushnumber(eventLocation, id); posOnStack++;
					}
					else
					{
						lua_pushstring(eventLocation, elementname.c_str()); posOnStack++;
					}
					
					// call the function
					event.GUIEvent.Caller->grab(); // grab it so we don't lose it
					lua_pcall(eventLocation,posOnStack-1,0,-posOnStack);
					nonlua_debug("Called GUI event handler " << eventtype.c_str() << ".",LVL_INFO);
					event.GUIEvent.Caller->drop(); // drop the reference count by one
				} else {
					nonlua_debug(luaGUIEventCapturer.c_str() << " does not exist.",LVL_WARN);
				}
				lua_pop(eventLocation, 1);
				return false;
			}
			if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			{
				if (luaMouseEventCapturer=="") return false;
				
				double mx = event.MouseInput.X;
				double my = event.MouseInput.Y;
				double mw = event.MouseInput.Wheel;
				
				irr::core::stringc eventtype;
				eventtype="";
				switch (event.MouseInput.Event)
				{
					case irr::EMIE_LMOUSE_PRESSED_DOWN:
						eventtype="LeftDown";
						break;
					case irr::EMIE_RMOUSE_PRESSED_DOWN:
						eventtype="RightDown";
						break;
					case irr::EMIE_MMOUSE_PRESSED_DOWN:
						eventtype="MiddleDown";
						break;
					case irr::EMIE_LMOUSE_LEFT_UP:
						eventtype="LeftUp";
						break;
					case irr::EMIE_RMOUSE_LEFT_UP:
						eventtype="RightUp";
						break;
					case irr::EMIE_MMOUSE_LEFT_UP:
						eventtype="MiddleUp";
						break;
					case irr::EMIE_MOUSE_MOVED:
						eventtype="Move";
						break;
					case irr::EMIE_MOUSE_WHEEL:
						eventtype="WheelScroll";
						break;
				}
				
				irr::core::stringc myeventcall;
				lua_getglobal(eventLocation, luaMouseEventCapturer.c_str());
				if (lua_type(eventLocation, -1)==LUA_TFUNCTION) {
					lua_pop(eventLocation,1);
					
					lua_getfield(RoketLUA, LUA_GLOBALSINDEX, "__internal_luaErrorHandler_errOnly");
					int posOnStack = 0;
					// build the function call
					lua_getglobal(eventLocation, luaMouseEventCapturer.c_str()); posOnStack++;
					if (allIsSame)
					{
						lua_pushnumber(eventLocation, 2); posOnStack++;
					}
					lua_pushstring(eventLocation, eventtype.c_str()); posOnStack++;
					lua_pushnumber(eventLocation, mx); posOnStack++;
					lua_pushnumber(eventLocation, my); posOnStack++;
					lua_pushnumber(eventLocation, mw); posOnStack++;
					
					// call the function
					lua_pcall(eventLocation,posOnStack-1,0,-posOnStack);
					nonlua_debug("Called mouse event handler.",LVL_INFO);
				} else {
					nonlua_debug(luaMouseEventCapturer.c_str() << " does not exist.",LVL_WARN);
				}
				lua_pop(eventLocation, 1);
				return false;
			}
			if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			{
				if (luaKeyboardEventCapturer=="") return false;
				
				wchar_t chr = event.KeyInput.Char;
				bool Ctrl = event.KeyInput.Control;
				bool PressedDown = event.KeyInput.PressedDown;
				bool Shift = event.KeyInput.Shift;
				
				irr::core::stringc eventtype;
				eventtype="";
				switch (event.KeyInput.Key)
				{
					// really long list of keycodes
					case irr::KEY_LBUTTON: eventtype="LButton"; break;
					case irr::KEY_RBUTTON: eventtype="RButton"; break;
					case irr::KEY_CANCEL: eventtype="Cancel"; break;
					case irr::KEY_MBUTTON: eventtype="MButton"; break;
					case irr::KEY_XBUTTON1: eventtype="XButton1"; break;
					case irr::KEY_XBUTTON2: eventtype="XButton2"; break;
					case irr::KEY_BACK: eventtype="Back"; break;
					case irr::KEY_TAB: eventtype="Tab"; break;
					case irr::KEY_CLEAR: eventtype="Clear"; break;
					case irr::KEY_RETURN: eventtype="Return"; break;
					case irr::KEY_SHIFT: eventtype="Shift"; break;
					case irr::KEY_CONTROL: eventtype="Control"; break;
					case irr::KEY_MENU: eventtype="Menu"; break;
					case irr::KEY_PAUSE: eventtype="Pause"; break;
					case irr::KEY_CAPITAL: eventtype="Capital"; break;
					case irr::KEY_KANA: eventtype="Kana"; break;
					//case irr::KEY_HANGUEL: eventtype="Hanguel"; break;
					//case irr::KEY_HANGUL: eventtype="Hangul"; break;
					case irr::KEY_JUNJA: eventtype="Junja"; break;
					case irr::KEY_FINAL: eventtype="Final"; break;
					case irr::KEY_HANJA: eventtype="Hanja"; break;
					//case irr::KEY_KANJI: eventtype="Kanji"; break;
					case irr::KEY_ESCAPE: eventtype="Escape"; break;
					case irr::KEY_CONVERT: eventtype="Convert"; break;
					case irr::KEY_NONCONVERT: eventtype="Non-Convert"; break;
					case irr::KEY_ACCEPT: eventtype="Accept"; break;
					case irr::KEY_MODECHANGE: eventtype="Modechange"; break;
					case irr::KEY_SPACE: eventtype="Space"; break;
					case irr::KEY_PRIOR: eventtype="Prior"; break;
					case irr::KEY_NEXT: eventtype="Next"; break;
					case irr::KEY_END: eventtype="End"; break;
					case irr::KEY_HOME: eventtype="Home"; break;
					case irr::KEY_LEFT: eventtype="Left"; break;
					case irr::KEY_UP: eventtype="Up"; break;
					case irr::KEY_RIGHT: eventtype="Right"; break;
					case irr::KEY_DOWN: eventtype="Down"; break;
					case irr::KEY_SELECT: eventtype="Select"; break;
					case irr::KEY_PRINT: eventtype="Print"; break;
					case irr::KEY_EXECUT: eventtype="Execut"; break;
					case irr::KEY_SNAPSHOT: eventtype="Snapshot"; break;
					case irr::KEY_INSERT: eventtype="Insert"; break;
					case irr::KEY_DELETE: eventtype="Delete"; break;
					case irr::KEY_HELP: eventtype="Help"; break;
					case irr::KEY_KEY_0: eventtype="0"; break;
					case irr::KEY_KEY_1: eventtype="1"; break;
					case irr::KEY_KEY_2: eventtype="2"; break;
					case irr::KEY_KEY_3: eventtype="3"; break;
					case irr::KEY_KEY_4: eventtype="4"; break;
					case irr::KEY_KEY_5: eventtype="5"; break;
					case irr::KEY_KEY_6: eventtype="6"; break;
					case irr::KEY_KEY_7: eventtype="7"; break;
					case irr::KEY_KEY_8: eventtype="8"; break;
					case irr::KEY_KEY_9: eventtype="9"; break;
					case irr::KEY_KEY_A: eventtype="A"; break;
					case irr::KEY_KEY_B: eventtype="B"; break;
					case irr::KEY_KEY_C: eventtype="C"; break;
					case irr::KEY_KEY_D: eventtype="D"; break;
					case irr::KEY_KEY_E: eventtype="E"; break;
					case irr::KEY_KEY_F: eventtype="F"; break;
					case irr::KEY_KEY_G: eventtype="G"; break;
					case irr::KEY_KEY_H: eventtype="H"; break;
					case irr::KEY_KEY_I: eventtype="I"; break;
					case irr::KEY_KEY_J: eventtype="J"; break;
					case irr::KEY_KEY_K: eventtype="K"; break;
					case irr::KEY_KEY_L: eventtype="L"; break;
					case irr::KEY_KEY_M: eventtype="M"; break;
					case irr::KEY_KEY_N: eventtype="N"; break;
					case irr::KEY_KEY_O: eventtype="O"; break;
					case irr::KEY_KEY_P: eventtype="P"; break;
					case irr::KEY_KEY_Q: eventtype="Q"; break;
					case irr::KEY_KEY_R: eventtype="R"; break;
					case irr::KEY_KEY_S: eventtype="S"; break;
					case irr::KEY_KEY_T: eventtype="T"; break;
					case irr::KEY_KEY_U: eventtype="U"; break;
					case irr::KEY_KEY_V: eventtype="V"; break;
					case irr::KEY_KEY_W: eventtype="W"; break;
					case irr::KEY_KEY_X: eventtype="X"; break;
					case irr::KEY_KEY_Y: eventtype="Y"; break;
					case irr::KEY_KEY_Z: eventtype="Z"; break;
					case irr::KEY_LWIN: eventtype="LeftSuper"; break;
					case irr::KEY_RWIN: eventtype="RightSuper"; break;
					case irr::KEY_APPS: eventtype="Apps"; break;
					case irr::KEY_SLEEP: eventtype="Sleep"; break;
					case irr::KEY_NUMPAD0: eventtype="Numpad0"; break;
					case irr::KEY_NUMPAD1: eventtype="Numpad1"; break;
					case irr::KEY_NUMPAD2: eventtype="Numpad2"; break;
					case irr::KEY_NUMPAD3: eventtype="Numpad3"; break;
					case irr::KEY_NUMPAD4: eventtype="Numpad4"; break;
					case irr::KEY_NUMPAD5: eventtype="Numpad5"; break;
					case irr::KEY_NUMPAD6: eventtype="Numpad6"; break;
					case irr::KEY_NUMPAD7: eventtype="Numpad7"; break;
					case irr::KEY_NUMPAD8: eventtype="Numpad8"; break;
					case irr::KEY_NUMPAD9: eventtype="Numpad9"; break;
					case irr::KEY_MULTIPLY: eventtype="Multiply"; break;
					case irr::KEY_ADD: eventtype="Add"; break;
					case irr::KEY_SEPARATOR: eventtype="Seperator"; break;
					case irr::KEY_SUBTRACT: eventtype="Subtract"; break;
					case irr::KEY_DECIMAL: eventtype="Decimal"; break;
					case irr::KEY_DIVIDE: eventtype="Divide"; break;
					case irr::KEY_F1: eventtype="F1"; break;
					case irr::KEY_F2: eventtype="F2"; break;
					case irr::KEY_F3: eventtype="F3"; break;
					case irr::KEY_F4: eventtype="F4"; break;
					case irr::KEY_F5: eventtype="F5"; break;
					case irr::KEY_F6: eventtype="F6"; break;
					case irr::KEY_F7: eventtype="F7"; break;
					case irr::KEY_F8: eventtype="F8"; break;
					case irr::KEY_F9: eventtype="F9"; break;
					case irr::KEY_F10: eventtype="F10"; break;
					case irr::KEY_F11: eventtype="F11"; break;
					case irr::KEY_F12: eventtype="F12"; break;
					case irr::KEY_F13: eventtype="F13"; break;
					case irr::KEY_F14: eventtype="F14"; break;
					case irr::KEY_F15: eventtype="F15"; break;
					case irr::KEY_F16: eventtype="F16"; break;
					case irr::KEY_F17: eventtype="F17"; break;
					case irr::KEY_F18: eventtype="F18"; break;
					case irr::KEY_F19: eventtype="F19"; break;
					case irr::KEY_F20: eventtype="F20"; break;
					case irr::KEY_F21: eventtype="F21"; break;
					case irr::KEY_F22: eventtype="F22"; break;
					case irr::KEY_F23: eventtype="F23"; break;
					case irr::KEY_F24: eventtype="F24"; break;
					case irr::KEY_NUMLOCK: eventtype="NumLock"; break;
					case irr::KEY_SCROLL: eventtype="ScrollLock"; break;
					case irr::KEY_LSHIFT: eventtype="LeftShift"; break;
					case irr::KEY_RSHIFT: eventtype="RightShift"; break;
					case irr::KEY_LCONTROL: eventtype="LeftControl"; break;
					case irr::KEY_RCONTROL: eventtype="RightControl"; break;
					case irr::KEY_LMENU: eventtype="LeftMenu"; break;
					case irr::KEY_RMENU: eventtype="RightMenu"; break;
					case irr::KEY_PLUS: eventtype="Plus"; break;
					case irr::KEY_COMMA: eventtype="Comma"; break;
					case irr::KEY_MINUS: eventtype="Minus"; break;
					case irr::KEY_PERIOD: eventtype="Period"; break;
					case irr::KEY_ATTN: eventtype="Attn"; break;
					case irr::KEY_CRSEL: eventtype="Crsel"; break;
					case irr::KEY_EXSEL: eventtype="Exsel"; break;
					case irr::KEY_EREOF: eventtype="Ereof"; break;
					case irr::KEY_PLAY: eventtype="Play"; break;
					case irr::KEY_ZOOM: eventtype="Zoom"; break;
					case irr::KEY_PA1: eventtype="PA1"; break;
					case irr::KEY_OEM_CLEAR: eventtype="OEMCLEAR"; break;
				}
				
				irr::core::stringc myeventcall;
				lua_getglobal(eventLocation, luaKeyboardEventCapturer.c_str());
				if (lua_type(eventLocation, -1)==LUA_TFUNCTION) {
					lua_pop(eventLocation,1);
					
					lua_getfield(RoketLUA, LUA_GLOBALSINDEX, "__internal_luaErrorHandler_errOnly");
					// build the function call
					int top = lua_gettop(eventLocation); 
					lua_getglobal(eventLocation, luaKeyboardEventCapturer.c_str());
					if (allIsSame)
						lua_pushnumber(eventLocation, 1);
					lua_pushstring(eventLocation, eventtype.c_str());
					lua_pushnumber(eventLocation, chr);
					lua_pushboolean(eventLocation, PressedDown);
					lua_pushboolean(eventLocation, Ctrl);
					lua_pushboolean(eventLocation, Shift);
					int bottom = lua_gettop(eventLocation); 
					
					// call the function
					lua_pcall(eventLocation,bottom - top - 1 ,0, top);
					nonlua_debug("Called keyboard event handler.",LVL_INFO);
				} else {
					nonlua_debug(luaKeyboardEventCapturer.c_str() << " does not exist.",LVL_WARN);
				}
				lua_pop(eventLocation, 1);
				return false;
			}
			if (event.EventType == irr::EET_ROKET_EVENT)
			{
				if (event.Roket3DEvent.subType == "Physics")
				{
					if (luaPhysicsEventCapturer=="") return false;

					if (event.Roket3DEvent.additionalInfo == "Transform")
					{
						irr::core::stringc myeventcall;
						scene::RSceneNode* eventnode = reinterpret_cast<scene::RSceneNode*>(event.Roket3DEvent.userData);
						lua_getglobal(eventLocation, luaMouseEventCapturer.c_str());
						if (lua_type(eventLocation, -1)==LUA_TFUNCTION) {
							lua_pop(eventLocation,1);
							
							lua_getfield(RoketLUA, LUA_GLOBALSINDEX, "__internal_luaErrorHandler_errOnly");
							int posOnStack = 0;
							// build the function call
							lua_getglobal(eventLocation, luaMouseEventCapturer.c_str()); posOnStack++;
							if (allIsSame)
							{
								lua_pushnumber(eventLocation, 3); posOnStack++;
							}
							lua_pushstring(eventLocation, "Transform"); posOnStack++;
							lua_pushstring(eventLocation, eventnode->base_self->RoketGetName().c_str()); posOnStack++;
							
							// call the function
							lua_pcall(eventLocation,posOnStack-1,0,-posOnStack);
							nonlua_debug("Called physics event handler.",LVL_INFO);
						} else {
							nonlua_debug(luaPhysicsEventCapturer.c_str() << " does not exist.",LVL_WARN);
						}
						lua_pop(eventLocation, 1);
					}
					else
					{
						nonlua_debug("Unknown physics event triggered.",LVL_WARN);
					}
				}
				else
				{
					nonlua_debug("Unknown event triggered.",LVL_WARN);
				}
				return false;
			}
			return false;
		}
		
		RoketEventReceiver(lua_State* L, bool instantedByUser);
		int setGUIEventCapturer(lua_State* L);
		int setMouseEventCapturer(lua_State* L);
		int setKeyboardEventCapturer(lua_State* L);
		int setPhysicsEventCapturer(lua_State* L);
		int setEventCapturer(lua_State* L);
		
		static const char *className;
		static const Luna < RoketEventReceiver >::FunctionType Functions[];
		static const Luna < RoketEventReceiver >::PropertyType Properties[];
};

#endif
