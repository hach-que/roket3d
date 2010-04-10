#include "roket3d.h"
#include "eventreciever.h"

const char *RoketEventReceiver::className = TYPE_ROKETEVENTRECEIVER;
// const char *RoketEventReceiver::className = "RoketEventReceiver";
const Luna < RoketEventReceiver >::FunctionType RoketEventReceiver::Functions[] = {
	{"setGUIEventCapturer", &RoketEventReceiver::setGUIEventCapturer},
	{"setMouseEventCapturer", &RoketEventReceiver::setMouseEventCapturer},
	{"setKeyboardEventCapturer", &RoketEventReceiver::setKeyboardEventCapturer},
	{"setPhysicsEventCapturer", &RoketEventReceiver::setPhysicsEventCapturer},
	{"setEventCapturer", &RoketEventReceiver::setEventCapturer},
	{0}
};
const Luna < RoketEventReceiver >::PropertyType RoketEventReceiver::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
	{ 0 }
};

RoketEventReceiver::RoketEventReceiver(lua_State* L, bool instantedByUser)
{
	allIsSame=false;
	isPrecious = true;
	eventLocation = L;
}

int RoketEventReceiver::setGUIEventCapturer(lua_State* L)
{
	// TODO: Check the argument check is correct.
	const char* eventc = getmandatoryargument_constchar(L, 1);
	luaGUIEventCapturer = eventc;
	allIsSame=false;
	return 0;
}

int RoketEventReceiver::setMouseEventCapturer(lua_State* L)
{
	// TODO: Check the argument check is correct.
	const char* eventc = getmandatoryargument_constchar(L, 1);
	luaMouseEventCapturer = eventc;
	allIsSame=false;
	return 0;
}

int RoketEventReceiver::setKeyboardEventCapturer(lua_State* L)
{
	// TODO: Check the argument check is correct.
	const char* eventc = getmandatoryargument_constchar(L, 1);
	luaKeyboardEventCapturer = eventc;
	allIsSame=false;
	return 0;
}

int RoketEventReceiver::setPhysicsEventCapturer(lua_State* L)
{
	// TODO: Check the argument check is correct.
	const char* eventc = getmandatoryargument_constchar(L, 1);
	luaPhysicsEventCapturer = eventc;
	allIsSame=false;
	return 0;
}

int RoketEventReceiver::setEventCapturer(lua_State* L)
{
	// TODO: Check the argument check is correct.
	const char* eventc = getmandatoryargument_constchar(L, 1);
	luaGUIEventCapturer = eventc;
	luaMouseEventCapturer = eventc;
	luaKeyboardEventCapturer = eventc;
	luaPhysicsEventCapturer = eventc;
	allIsSame=true;
	return 0;
}
