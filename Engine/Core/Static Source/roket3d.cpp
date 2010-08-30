#include "roket3d.h"

lua_State *RoketLUA;
int debuglevel;
irr::core::stringc luaFile;

irr::core::stringc INVALID_ARGUMENT(const char* expected, int argpos)
{
	irr::core::stringc str;
	str = "Invalid argument for position ";
	str += argpos;
	str += ", expecting ";
	str += expected;
	str += ".";
	return str;
}

irr::core::stringc FILE_NOT_FOUND(const char* action, const char* file)
{
	irr::core::stringc str;
	str = "Unable to ";
	str += action;
	str += " because the file ";
	str += file;
	str += " was not found.";
	return str;
}

irr::core::stringc INTERNAL_ERROR(const char* action)
{
	irr::core::stringc str;
	str = "Internal error! ";
	str += action;
	str += ".";
	return str;
}

wchar_t *ConvertToWideChar (const char *p)
{
	wchar_t *r;
	r = new wchar_t[strlen (p) + 1];
	const char *tempsour = p;
	wchar_t *tempdest = r;
	while (*tempdest++ = *tempsour++);
	return r;
}

bool isvalid(void* self)
{
	if (self == 0)
	{
		return false;
	}
	else if (self == (void*)-1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

wchar_t *ConvertToWideChar (char *p)
{
	wchar_t *r;
	r = new wchar_t[strlen (p) + 1];
	char *tempsour = p;
	wchar_t *tempdest = r;
	while (*tempdest++ = *tempsour++);
	return r;
}

// convert wide char to char
char *ConvertToChar (wchar_t * p)
{
	char *r;
	r = new char[wcslen (p) + 1];
	wchar_t *tempsour = p;
	char *tempdest = r;
	while (*tempdest++ = *tempsour++);
	return r;
}

char *ConvertToChar (const wchar_t * p)
{
	char *r;
	r = new char[wcslen (p) + 1];
	const wchar_t *tempsour = p;
	char *tempdest = r;
	while (*tempdest++ = *tempsour++);
	return r;
}

void lua_gettablevalue (lua_State * luaVM, int tableindex, int valueindex)
{
	lua_pushnumber (luaVM, valueindex);
	lua_gettable (luaVM, tableindex);
	// use lua_to<type>(-1); to get the value.
}

/* Optional Argument Functions */

const char* getoptionalargument(lua_State* luaVM,int n,const char* d){
	if (lua_isstring(luaVM,n+1))
	{
		return lua_tostring(luaVM,n+1);
	}
	else
	{
		return d;
	}
}

/*
double getoptionalargument(lua_State* luaVM,int n,double d)
{
	// TODO: Find out why this requires -3
	if (!lua_isnoneornil(luaVM,n) && n<=lua_gettop(luaVM)-3)
	{
		return (double)lua_tonumber(luaVM,n);
	}
	else
	{
		return d;
	}
}
*/

cNumber getoptionalargument(lua_State* luaVM,int n,cNumber d)
{
	// TODO: Find out why this requires -3
	if (!lua_isnoneornil(luaVM,n+1))
	{
		return (cNumber)lua_tonumber(luaVM,n+1);
	}
	else
	{
		return d;
	}
}

double getoptionalargument(lua_State* luaVM,int n,int d)
{
	// TODO: Find out why this requires -3
	if (!lua_isnoneornil(luaVM,n+1))
	{
		return (double)lua_tonumber(luaVM,n+1);
	}
	else
	{
		return (double)d;
	}
}

bool getoptionalargument(lua_State* luaVM,int n,bool d)
{
	// TODO: Find out why this requires -3
	if (!lua_isnoneornil(luaVM,n+1))
	{
		return lua_toboolean(luaVM,n+1);
	}
	else
	{
		return d;
	}
}

const char* getmandatoryargument_constchar(lua_State* luaVM,int n){
	if (lua_isstring(luaVM,n+1))
	{
		return lua_tostring(luaVM,n+1);
	}
	else
	{
		luaL_typerror(luaVM, n, TYPE_STRING);
	}
}

/*
double getmandatoryargument_double(lua_State* luaVM,int n)
{
	if (lua_isnumber(luaVM,n+1))
	{
		return (double)lua_tonumber(luaVM,n+1);
	}
	else
	{
		luaL_typerror(luaVM, n, TYPE_NUMBER);
	}
}
*/

cNumber getmandatoryargument_cNumber(lua_State* luaVM,int n)
{
	if (lua_isnumber(luaVM,n+1))
	{
		return (cNumber)lua_tonumber(luaVM,n+1);
	}
	else
	{
		luaL_typerror(luaVM, n, TYPE_NUMBER);
	}
}

bool getmandatoryargument_bool(lua_State* luaVM,int n)
{
	if (lua_isboolean(luaVM,n+1))
	{
		return lua_toboolean(luaVM,n+1);
	}
	else
	{
		luaL_typerror(luaVM, n, TYPE_BOOLEAN);
	}
}

int includeFile(lua_State* L)
{
	if (!lua_isstring(L,1)) luaL_typerror(L, 1, TYPE_STRING);
	if (fileExists(lua_tostring(L,1)))
	{
		lua_pushnumber(L,luaL_dofile(L,lua_tostring(L,1)));
		return 1;
	}
	else
	{
		debug("Unable to find " << lua_tostring(L,1) << ".",LVL_ERR,L);
		return 0;
	}
}

void registerConstant (lua_State * L, const char* name, const char* value)
{
	// Creates a constant
	lua_pushstring(L, value);
	lua_setglobal(L, name);
}

void registerConstant (lua_State * L, const char* name, const char* value, const char* namespac)
{
	// Creates a constant
	lua_getglobal(L, namespac);
	lua_pushstring(L, value);
	lua_setfield(L, -2, name);
	lua_pop(L, 1);
}

void registerConstant (lua_State * L, const char* name, double value)
{
	// Creates a constant
	lua_pushnumber(L, value);
	lua_setglobal(L, name);
}

void registerConstant (lua_State * L, const char* name, double value, const char* namespac)
{
	// Creates a constant
	lua_getglobal(L, namespac);
	lua_pushnumber(L, value);
	lua_setfield(L, -2, name);
	lua_pop(L, 1);
}

void registerConstant (lua_State * L, int index, const char* value, const char* namespac)
{
	// Creates a constant
	lua_getglobal(L, namespac);
	lua_pushnumber(L, index);
	lua_pushstring(L, value);
	lua_settable(L, -3);
	lua_pop(L, 1);
}

void registerConstant (lua_State * L, int index, double value, const char* namespac)
{
	// Creates a constant
	lua_getglobal(L, namespac);
	lua_pushnumber(L, index);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
	lua_pop(L, 1);
}

void initalizeRoket3D (lua_State * L, int argc, char* argv[])
{
	CoreInitalize		(L);
	GUIInitalize		(L);
	SceneInitalize		(L);
	VideoInitalize		(L);
	ExtInitalize		(L);
	PhysicsInitalize	(L);
	NetInitalize		(L);
	AudioInitalize		(L);

	// work out the location of Roket3D
	registerConstant(L, "_SELFEXE",argv[0]);
	
	// work out the working directory
    irr::core::stringc basedir(argv[0]);
    char chrr = '/';
    basedir = basedir.subString(0,basedir.findLast(chrr));
    registerConstant(L, "_SELFDIR",basedir.c_str());
    basedir = basedir + "/";
    registerConstant(L, "_SELFDIRWS",basedir.c_str());

	registerConstant(L, "_ARGC",argc);
	lua_newtable(L);
	lua_setglobal(L,"_ARGV");
	for (int i=0;i<argc;i++)
	{
		registerConstant(L, i,argv[i],"_ARGV");
	}

	// setup physics solver constants
	registerConstant(L, "_SOLVER_EXACT",0.f);
	registerConstant(L, "_SOLVER_ADAPTIVE",1.f);
	registerConstant(L, "_SOLVER_GAME",2.f);
	registerConstant(L, "_FRICTION_EXACT",0.f);
	registerConstant(L, "_FRICTION_ADAPTIVE",1.f);
	registerConstant(L, "_HARDWARE_DEFAULT",0.f);
	registerConstant(L, "_HARDWARE_MEDIUM",1.f);
	registerConstant(L, "_HARDWARE_BEST",2.f);

	registerConstant(L, "_JOINT_BALL_AND_SOCKET",0.f);
	registerConstant(L, "_JOINT_BALL",0.f);
	registerConstant(L, "_JOINT_HINGE",1.f);
	registerConstant(L, "_JOINT_SLIDER",2.f);
	registerConstant(L, "_JOINT_CORKSCREW",3.f);
	registerConstant(L, "_JOINT_UNIVERSAL",4.f);
	registerConstant(L, "_JOINT_NONE",5.f);

	registerConstant(L, "_EVENT_GUI",0.f);
	registerConstant(L, "_EVENT_KEYBOARD",1.f);
	registerConstant(L, "_EVENT_MOUSE",2.f);
	registerConstant(L, "_EVENT_PHYSICS",3.f);

	// Lua provides an in-built function like this.
	//lua_register    (L,"include",&includeFile);
}

bool fileExists(const irr::core::stringc& fileName)
{
	if (fileName.size() == 0)
		return false;
	std::fstream fin;
	fin.open(fileName.c_str(),std::ios::in);
	if( fin.is_open() )
	{
		fin.close();
		return true;
	}
	fin.close();
	return false;
}

void scanForFile(irr::core::stringc sfile)
{
	irr::core::stringc basename;
	if (luaFile=="")
		basename = "./";
	if (luaFile.subString(luaFile.size()-1,1)=="/" || luaFile.subString(luaFile.size()-1,1)=="\\")
		basename = luaFile;
	if (luaFile=="" || luaFile.subString(luaFile.size()-1,1)=="/" || luaFile.subString(luaFile.size()-1,1)=="\\")
	{
		irr::core::stringc fullfilename;
		fullfilename = basename + sfile;
		// TODO Try and repair this instruction.  This is just a quick bugfix.
#if ((defined(WIN32) || defined(WIN64)) && (!defined(__GNUC__)))
		printf("Searching for %s ... ",sfile);
#else
		std::cout << "Searching for " << sfile.c_str() << " ...";
#endif
		if (fileExists(fullfilename))
		{
			cout << "found." << endl;
			luaFile=fullfilename;
			return;
		}
		else
		{
			cout << "not found." << endl;
		}
	}
}

// Parse arguments function
void parseArguments(int argc, char *argv[])
{
	int i;
	luaFile="";
	bool displayHelp = false;
	for (i=1;i<argc;i++)
	{
		if (strcmp(argv[i],"--debug=info")==0) { debuglevel = LVL_INFO; continue; }
		if (strcmp(argv[i],"--debug=warn")==0) { debuglevel = LVL_WARN; continue; }
		if (strcmp(argv[i],"--debug=error")==0) { debuglevel = LVL_ERR; continue; }
		if (strcmp(argv[i],"--debug=fatal")==0) { debuglevel = LVL_FTL; continue; }
		if (strcmp(argv[i],"--help")==0) { displayHelp = true; continue; }
		if (strcmp(argv[i],"-h")==0) { displayHelp = true; continue; }
		if (strcmp(argv[i],"/?")==0) { displayHelp = true; continue; } // windows help toggle
		
		if (i==argc-1)
		{
			luaFile=argv[i];
		}
	}
	
	
	// scan for main.lua files
	scanForFile("main.rks");
	scanForFile("main.rcs");
	scanForFile("main.lua");
	scanForFile("scripts/main.rks");
	scanForFile("scripts/main.rcs");
	scanForFile("scripts/main.lua");
	scanForFile("code/main.rks");
	scanForFile("code/main.rcs");
	scanForFile("code/main.lua");
	scanForFile("src/main.rks");
	scanForFile("src/main.rcs");
	scanForFile("src/main.lua");
	scanForFile("examples/main.rks");
	scanForFile("examples/main.rcs");
	scanForFile("examples/main.lua");
	
	if (luaFile=="")
		displayHelp=true;
	
	if (displayHelp==true)
	{	
		cout << "Syntax: roket3d [options] [filename]" << endl
		<< endl
		<< "Filename is the name of a " << ROKET3D_NAME << " script." << endl
		<< "If filename is not given, Roket3D will search in common places." << endl
		<< "Options can be any of the following : " << endl
		<< "	--debug=[info|warn|error|fatal]		Sets the debugging level." << endl
		<< "	--help					Shows this help message." << endl
		<< endl
		<< ROKET3D_NAME << " is copyright Roket Productions 2006-2009." << endl
		<< "This version of " << ROKET3D_NAME << " is " << ROKET3D_VERSION << "." << endl
		<< "This version is from " << ROKET3D_TYPE << " IDE." << endl
		<< "This application uses Lua 5.1.2.  Visit http://www.lua.org for more information." << endl
		<< "This application uses a modified version of Irrlicht 1.4.  Visit" << endl
		<< "  http://irrlicht.sourceforge.net/ for more information." << endl;
	}
}

int luaErrorHandler(lua_State* L)
{
	//cout << "=====================================" << endl;
	//showstack(L);
	cout << "=====================================" << endl;
	cout << "FATL: " << lua_tostring(L, -1) << endl;
	cout << "============= STACK =================" << endl;

	if (!lua_isstring(L, 1))  /* 'message' not a string? */
	{
		cout << "============= MSG NOT STRING ========" << endl;
		return 1;  /* keep it intact */
	}
	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		cout << "============= NO DEBUG ==============" << endl;
		return 1;
	}
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 2);
		cout << "============= NO TRACE ==============" << endl;
		return 1;
	}
	lua_pushvalue(L, 1);  /* pass error message */
	lua_pushinteger(L, 2);  /* skip this function and traceback */
	lua_call(L, 2, 1);  /* call debug.traceback */
	cout << "============= END STACK =============" << endl;
	//showstack(L);
	//cout << "=====================================" << endl;
	return 1;

	/*
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
	lua_replace(L, -2);
	lua_pcall(L, 0, 0, 0);
	cout << "=====================================" << endl;
	return 0;
	*/
}

int luaErrorHandler_syntaxErr(const char* msg)
{
	cout << "=====================================" << endl;
	cout << "FATL: " << msg << endl;
	cout << "=====================================" << endl;
	return 1;
}

int luaErrorHandler_errOnly(lua_State* L)
{
	cout << "=====================================" << endl;
	cout << "ERR : " << lua_tostring(L, -1) << endl;
	cout << "=====================================" << endl;
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
	lua_replace(L, -2);
	lua_pcall(L, 0, 0, 0);
	cout << "=====================================" << endl;
	return 0;
}

void *luaL_testudata (lua_State *L, int ud, const char *tname) {
	void *p = lua_touserdata(L, ud);
	if (p != NULL) {  /* value is a userdata? */
		if (lua_getmetatable(L, ud)) {  /* does it have a metatable? */
			lua_getfield(L, LUA_REGISTRYINDEX, tname);  /* get correct metatable */
			if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
				lua_pop(L, 2);  /* remove both metatables */
				return p;
			}
		}
	}
	return NULL;  /* to avoid warnings */
}

const char* luaL_getudatatype (lua_State *L, int ud) {
	void *p = lua_touserdata(L, ud);
	if (p != NULL) {  /* value is a userdata? */
		lua_getmetatable(L, ud);
		lua_gettable(L, LUA_REGISTRYINDEX);
		if (lua_isstring(L, lua_gettop(L)))
			return "unidentified class";
		else
			return lua_tostring(L, -1);
	}
	else
        return luaL_typename(L, ud);
}

int luaL_typerroradvanced (lua_State *L, int narg, const char *tname) {
	const char *msg = lua_pushfstring(L, "%s expected, got %s",
									tname, luaL_getudatatype(L, narg));
	return luaL_argerror(L, narg, msg);
}