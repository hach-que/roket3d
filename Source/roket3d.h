#ifndef HEADER_ROKET3D
#define HEADER_ROKET3D

#define ENABLE_AUDIO 0

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
using namespace std;
#include <lua.hpp>
/*#if defined(WIN32)
#include "../Newton/win32/Newton.h"
#elif defined(WIN64)
#include "../Newton/win64/Newton.h"
#elif defined(__MACOSX__)
#include "../Newton/mac/Newton.h"
#elif defined(_LP64) && defined(__GNUC__)
#include "../Newton/linux64/Newton.h"
#elif !defined(_LP64) && defined(__GNUC__)
#include "../Newton/linux32/Newton.h"
#else
#error "Unknown platform.  Cannot include Newton headers.  I will now quit."
#endif*/

#if defined(WIN32) || defined(WIN64)
	#include <Newton.h>
#else
	#include "/windows/C/Roket3D/Newton/linux32/sdk/Newton.h"
	#define _cdecl __attribute__((_cdecl))
#endif
#if ENABLE_AUDIO
	#include "IAudioManager.h"
	#include "IAudio.h"
#endif
#if defined(WIN32) || defined(WIN64)
	#include <SDL.h>
	#include <SDL_thread.h>
	#include <SDL_net.h>
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_thread.h>
	#include <SDL/SDL_net.h>
	#include <boost/bind.hpp>
#endif

#include "../RoketGraphics/include/irrlicht.h"
#include "../RoketGraphics/XEffects/Source/effectWrapper.h"
#include "../RoketGraphics/XEffects/Source/effectWrapperCB.h"

extern lua_State *RoketLUA;
extern int debuglevel;
extern irr::core::stringc luaFile;
struct safeVP
{
	private:
		irr::core::stringc Ptype;
		void * PvoidPointer;
	
	public:
		irr::core::stringc type()
		{
			return Ptype;
		}
		
		void type(irr::core::stringc newtype)
		{
			if (newtype!="")
				Ptype=newtype;
		}
		
		void * voidPointer()
		{
			return PvoidPointer;
		}
		
		void voidPointer(void * newVoidPointer)
		{
			PvoidPointer = newVoidPointer;
		}
};

enum ePhysicsType
{
	E_TYPE_SPHERE = 0,
	E_TYPE_CUBE,
	E_TYPE_MESH,
	E_TYPE_TERRAIN,
	E_TYPE_IGNORE,
	E_TYPE_STATIC,
	E_TYPE_NONE,
	E_TYPE_UNKNOWN
};

enum ePhysicsJointType
{
	E_JOINT_BALL_AND_SOCKET = 0,
	E_JOINT_HINGE,
	E_JOINT_SLIDER,
	E_JOINT_CORKSCREW,
	E_JOINT_UNIVERSAL,
	E_JOINT_NONE
};

void *luaL_testudata (lua_State *L, int ud, const char *tname);
const char* luaL_getudatatype (lua_State *L, int ud);
int luaL_typerroradvanced (lua_State *L, int narg, const char *tname);
// convert char to wide char
wchar_t *ConvertToWideChar (const char *p);
wchar_t *ConvertToWideChar (char *p);
// convert wide char to char
char *ConvertToChar (wchar_t * p);
char *ConvertToChar (const wchar_t * p);
void lua_gettablevalue (lua_State * luaVM, int tableindex, int valueindex);
void parseArguments(int argc, char *argv[]);
int includeFile(lua_State* L);

/*********************************************
 * CORE DEFINITIONS                          *
 *********************************************/

#define ROKET3D_VERSION "0.5 ALPHA"
#define ROKET3D_NAME "Roket3D"
#define ROKET3D_TYPE "no"
#define ROKET3D_DEVELOPERS "James Rhodes"
#define ROKET3D_ASSISTANTS "None"
#define OPENGL
// On windows, use DirectX
#if ((defined(WIN32) || defined(WIN64)) && (!defined(__GNUC__)))
#undef OPENGL
#define WINDOWS_COMPILE
#endif
#define LVL_ERR 1
#define LVL_WARN 2
#define LVL_INFO 3
#define LVL_FTL 0

#define PI 3.1415926535897932384626433832795029L /* pi */

// types
#define TYPE_COLOR							"Color"
#define TYPE_BOOLEAN						"Boolean"
#define TYPE_NUMBER							"Number"
#define TYPE_TABLE							"Table"
#define TYPE_2DRAY							"Ray2D"
#define TYPE_3DRAY							"Ray3D"
#define TYPE_STRING							"String"
#define TYPE_RECTANGLE						"Area2D"
#define TYPE_CUBE							"Area3D"
#define TYPE_2DVECTOR						"Vector2D"
#define TYPE_3DVECTOR						"Vector3D"
#define TYPE_3DTRIANGLE						"Triangle3D"
#define TYPE_3DPLANE						"Plane"
#define TYPE_QUATERNION						"Quaternion"
#define TYPE_MATRIX							"Matrix"
			 
// class types
#define TYPE_RDEVICE							"RDevice"
#define TYPE_RCURSORCONTROL						"RCursorControl"
#define TYPE_ROKETEVENTRECEIVER					"RoketEventReceiver"
#define TYPE_RGUIBUTTON							"RGUIButton"
#define TYPE_RGUICHECKBOX						"RGUICheckBox"
#define TYPE_RGUICOMBOBOX						"RGUIComboBox"
#define TYPE_RGUIEDITBOX						"RGUIEditBox"
#define TYPE_RGUIELEMENT						"RGUIElement"
#define TYPE_RGUIENVIRONMENT					"RGUIEnvironment"
#define TYPE_RGUIFILEOPENDIALOG					"RGUIFileOpenDialog"
#define TYPE_RGUIFONT							"RGUIFont"
#define TYPE_RGUIIMAGE							"RGUIImage"
#define TYPE_RGUILISTBOX						"RGUIListBox"
#define TYPE_RGUIMESHVIEWER						"RGUIMeshViewer"
#define TYPE_RGUISCROLLBAR						"RGUIScrollBar"
#define TYPE_RGUISKIN							"RGUISkin"
#define TYPE_RGUISTATICTEXT						"RGUIStaticText"
#define TYPE_RGUIWINDOW							"RGUIWindow"
#define TYPE_RSCENECAMERA						"RSceneCamera"
#define TYPE_RSCENECOLLISIONMANAGER				"RSceneCollisionManager"
#define TYPE_RSCENESPECIALEFFECTS				"RSceneSpecialEffects"
#define TYPE_RSCENEMANAGER						"RSceneManager"
#define TYPE_RSCENEMESH							"RMesh"
#define TYPE_RSCENENODE							"RSceneNode"
#define TYPE_RSCENEMESHNODE						"RMeshSceneNode"
#define TYPE_RSCENELIGHTNODE					"RLightSceneNode"
#define TYPE_RSCENETERRAINNODE					"RTerrainSceneNode"
#define TYPE_RSCENEBILLBOARDNODE				"RBillboardSceneNode"
#define TYPE_RSCENEBILLBOARDTEXTNODE			"RBillboardTextSceneNode"
#define TYPE_RSCENEWATERNODE					"RWaterSceneNode"
#define TYPE_RSCENEOCTTREENODE					"ROctTreeSceneNode"
#define TYPE_RPARTICLESYSTEMSCENENODE			"RParticleSystemSceneNode"
#define TYPE_RVIDEOTEXTURE						"RTexture"
#define TYPE_RVIDEODRIVER						"RVideoDriver"
#define TYPE_RVIDEOMATERIAL						"RMaterial"
#define TYPE_REXTTHREAD							"RThread"
#define TYPE_RNETSOCKET							"RNetSocket"
#define TYPE_RAUDIOCONTROLLER					"RAudioController"
#define TYPE_RAUDIOSOUND						"RAudioSound"


// define an infinity number
#define MATERIAL_INFINITY	-65536

// Physics engine constants
#define PHYS_SOLVER_EXACT	0
#define PHYS_SOLVER_ADAPTIVE	1
#define PHYS_SOLVER_GAME	2
#define PHYS_FRICTION_EXACT	0
#define PHYS_FRICTION_ADAPTIVE	1
#define PHYS_HARDWARE_DEFAULT	0
#define PHYS_HARDWARE_MEDIUM	1
#define PHYS_HARDWARE_BEST	2
#define NEWTON_SCALE 1

#define cNumber		 double
#define cIrrNumber   irr::f32
#define cIrrVector3D irr::core::vector3df
#define cIrrVector2D irr::core::vector2df

#define retarg(msg)								\
			irr::core::stringc ret;					\
			ret = msg;						\
			return ret;

irr::core::stringc INVALID_ARGUMENT(const char* expected, int argpos);
irr::core::stringc FILE_NOT_FOUND(const char* action, const char* file);
irr::core::stringc INTERNAL_ERROR(const char* action);

#define debug(m,l,LUA)								\
			if (debuglevel>=l)					\
			{ 							\
				if (l==LVL_ERR)					\
				{						\
					cout<<"ERR : ";				\
				}						\
				if (l==LVL_WARN)				\
				{						\
					cout<<"WARN: ";				\
				}						\
				if (l==LVL_INFO)				\
				{						\
					cout<<"INFO: ";				\
				}						\
			 	if (l==LVL_FTL)					\
				{						\
			 		cout<<"FATL: ";				\
				}						\
				lua_Debug info;					\
				lua_getstack(LUA, 1, &info);			\
				lua_getinfo(LUA,"S",&info);			\
				lua_getinfo(LUA,"l",&info);			\
				cout << info.short_src << ":" << info.currentline << ": " << m << endl; \
			}

#define nonlua_debug(m,l)							\
			if (debuglevel>=l)					\
			{ 							\
				if (l==LVL_ERR)					\
				{						\
					cout<<"ERR : ";				\
				}						\
				if (l==LVL_WARN)				\
				{						\
					cout<<"WARN: ";				\
				}						\
				if (l==LVL_INFO)				\
				{						\
					cout<<"INFO: ";				\
				}						\
			 	if (l==LVL_FTL)					\
				{						\
			 		cout<<"FATL: ";				\
				}						\
				cout << m << endl; \
			}
			 
#define ifisvalid(ptr)								\
			if (isvalid(ptr))					\
			{

#define ifistrue(bool)  if(bool){
			 
#define endifisvalid()								\
			}							\
			else							\
			{							\
				debug("Sorry, this object is no longer valid.",LVL_ERR,L);\
				return 0;					\
			}

#define ifdebug(exp,m,l,LUA)						\
			if (exp)					\
			{						\
				debug(m,l,LUA);				\
				if (l==LVL_ERR)				\
				{					\
					return 0;			\
				}					\
			}	

#define ifcdebug(exp,m,l,LUA)						\
			if (exp)					\
			{						\
				debug(m,l,LUA);				\
				return;				\
			}

#define showstack(lstack)	\
	for (int i=1;i<lua_gettop(lstack)+1;i+=1)\
	{\
		if (lua_isnumber(lstack,i))\
		{\
			cout << i << ": number : " << lua_tonumber(lstack,i) << endl;\
		}\
		else if (lua_isstring(lstack,i))\
		{\
			cout << i << ": string : " << lua_tostring(lstack,i) << endl;\
		}\
		else if (lua_istable(lstack,i))\
		{\
			cout << i << ": table" << endl;\
		}\
		else if (lua_iscfunction(lstack,i))\
		{\
			cout << i << ": cfunction" << endl;\
		}\
		else if (lua_isfunction(lstack,i))\
		{\
			cout << i << ": function" << endl;\
		}\
		else if (lua_isboolean(lstack,i))\
		{\
			if (lua_toboolean(lstack,i)==true)\
				cout << i << ": boolean : true" << endl;\
			else\
				cout << i << ": boolean : false" << endl;\
		}\
		else if (lua_isuserdata(lstack,i))\
		{\
			cout << i << ": userdata" << endl;\
		}\
		else if (lua_isnil(lstack,i))\
		{\
			cout << i << ": nil" << endl;\
		}\
		else if (lua_islightuserdata(lstack,i))\
		{\
			cout << i << ": light userdata" << endl;\
		}\
	}

inline void* lua_multicheck(lua_State* L, const char* type)
{
	void* test = luaL_testudata(L, -1, type);
	if (test!=NULL)
	{
		return test;
	}
	else
	{
		lua_pop(L,1);
		return NULL;
	}
}

// include luna template
#include "luna.h"

/*
#ifndef VARCHECKER
#define VARCHECKER
template < class T > class VarCheck {
	public:
	static T check(lua_State* L, const char* TYPE, int ARGUMENT)
	{
		if (lua_isnoneornil(L,ARGUMENT))
			luaL_typerror(L, ARGUMENT, TYPE);
		if (TYPE==TYPE_NUMBER && lua_isnumber(L,ARGUMENT))
			return reinterpret_cast<T>(lua_tonumber(L,ARGUMENT));
		else
			luaL_typerror(L, ARGUMENT, TYPE);
		if (TYPE==TYPE_STRING && lua_isstring(L,ARGUMENT))
			return reinterpret_cast<T>(lua_tostring(L,ARGUMENT));
		else
			luaL_typerror(L, ARGUMENT, TYPE);
		if (TYPE==TYPE_TABLE && lua_istable(L,ARGUMENT))
			return reinterpret_cast<T>(lua_totable(L,ARGUMENT));
		else
			luaL_typerror(L, ARGUMENT, TYPE);
	}
};
#endif
*/

bool isvalid(void* self);
const char* getoptionalargument(lua_State* luaVM,int n,const char* d);
//double getoptionalargument(lua_State* luaVM,int n,double d);
cNumber getoptionalargument(lua_State* luaVM,int n,cNumber d);
bool getoptionalargument(lua_State* luaVM,int n,bool d);
double getoptionalargument(lua_State* luaVM,int n,int d);

const char* getmandatoryargument_constchar(lua_State* luaVM,int n);
//double getmandatoryargument_double(lua_State* luaVM,int n);
cNumber getmandatoryargument_cNumber(lua_State* luaVM,int n);
bool getmandatoryargument_bool(lua_State* luaVM,int n);

bool fileExists(const irr::core::stringc& fileName);
void scanForFile(irr::core::stringc sfile);

int luaErrorHandler(lua_State* L);
int luaErrorHandler_errOnly(lua_State* L);
int luaErrorHandler_syntaxErr(const char* msg);

/*********************************************
 * END CORE DEFINITIONS                      *
 *********************************************/

// include all R3D classes
#include "robject.h"
#include "eventreciever.h"
#include "gui.h"
#include "core.h"
#include "scene.h"
#include "video.h"
#include "ext.h"
#include "physics.h"
#include "net.h"
#include "audio.h"

void initalizeRoket3D (lua_State * L, int argc, char* argv[]);
void registerConstant (lua_State * L, const char* name, const char* value, const char* namespac);
void registerConstant (lua_State * L, const char* name, const char* value);
void registerConstant (lua_State * L, const char* name, double value, const char* namespac);
void registerConstant (lua_State * L, const char* name, double value);
void registerConstant (lua_State * L, int index, const char* value, const char* namespac);
void registerConstant (lua_State * L, int index, double value, const char* namespac);

#endif
