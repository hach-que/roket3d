//////////////////////////////////////////////////////////
// CAUTION! This code has been known to cause heads to  //
//          explode.  Read at your own risk.            //
//////////////////////////////////////////////////////////

#include "roket3d.h"


// MAKES BINDINGS (Also has the worst indentation ever) 
template < class T > class Luna {
    typedef struct {
	T              *pT;
    } userdataType;
    
  public:

    enum {
	NUMBER,
	STRING
    };

    struct PropertyType {
	const char     *name;
	int             (T::*getter) (lua_State *);
	int             (T::*setter) (lua_State *);
    };

    struct FunctionType {
	const char     *name;
	int             (T::*function) (lua_State *);
    };

    // FOR OBJECT DESTRUCTORS 
    // lua_State *lua_state; 

    // GET C++ OBJECT FROM LUA
    // get userdata from Lua stack and return pointer to T object
    static T       *check(lua_State * L, int narg) {
	// Check to see whether we are a table
	    if (lua_istable(L,narg+1))
	    {
		    lua_gettablevalue(L,narg+1,0);
		    userdataType   *ud =
				    static_cast <userdataType * >(luaL_testudata(L, -1, T::className));
		    if (!ud)
			    luaL_typerroradvanced(L, narg+1, T::className);
		    lua_pop(L,1);
		    return ud->pT;		// pointer to T object
	    }
	    else
	    {
		    luaL_typerroradvanced(L, narg+1, T::className);
			return 0;
	    } 
    }

	static T       *lightcheck(lua_State * L, int narg) {
	// Check to see whether we are a table
	    if (lua_istable(L,narg+1))
	    {
		    lua_gettablevalue(L,narg+1,0);
		    userdataType   *ud =
				    static_cast <userdataType * >(luaL_testudata(L, -1, T::className));
		    if (!ud)
			    return NULL; // lightcheck returns NULL if not found.
		    lua_pop(L,1);
		    return ud->pT;		// pointer to T object
	    }
	    else
	    {
		    return NULL;
	    } 
    }

    // REGISTER CLASS AS A GLOBAL TABLE 
    static void     Register(lua_State * L, const char *namespac) {

		// More stack space
		lua_checkstack(L,5);

	// ADD CONSTRUCTOR AS A GLOBAL FUNCTION (in namespace if
	// nessicary) 
	if (strcmp(namespac, "") != 0) {
	    lua_getglobal(L, namespac);
	    lua_pushcfunction(L, &Luna < T >::constructor);
	    lua_setfield(L, -2, T::className);
	    lua_pop(L, 1);
	    //cout << "Initalizing " << T::
		//className << " into the " << namespac << " namespace." <<
		//endl;
	} else {
	    lua_pushcfunction(L, &Luna < T >::constructor);
	    lua_setglobal(L, T::className);
	}

	// ADD METETABLE WITH CLASS NAME 
	// THIS IS USED FOR THE USERDATA ONLY 
	// STORE IN LUA_REGISTRYINDEX 
	// OVERLOAD LUA TABLE FUNCTIONS 
	luaL_newmetatable(L, T::className);
	int             metatable = lua_gettop(L);

	// Set the reverse
	lua_pushvalue(L, -1);
	lua_pushstring(L, T::className);
	lua_rawset(L, LUA_REGISTRYINDEX);

	// GC CALL BACK 
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, &Luna < T >::gc_obj);
	lua_settable(L, metatable);

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, &Luna < T >::property_getter);
	lua_settable(L, metatable);

	lua_pushstring(L, "__setindex");
	lua_pushcfunction(L, &Luna < T >::property_setter);
	lua_settable(L, metatable);

    }

    static int      constructor(lua_State * L) {

	// MAKE A NEW TABLE TO RETURN 
	// AKA OUR NEW LUA OBJECT 
	lua_newtable(L);

	// GET ADDRESS OF newtable ON STACK 
	int             newtable = lua_gettop(L);

	// PUSH INDEX OF USER DATA 
	lua_pushnumber(L, 0);

	// MAKE USER DATA ON THE STACK AND POINT IT TO LUNA OBJECT 
	T             **a = (T **) lua_newuserdata(L, sizeof(T *));
	T              *obj = new T(L, true);
	*a = obj;

	// const PropertyType *p; 
	// p = T::Properties; 
	// p = (*a)->T::Properties; 

	int             userdata = lua_gettop(L);

	// GET CLASS META TABLE FROM GLOBAL LUA_REGISTRYINDEX 
	luaL_getmetatable(L, T::className);

	// SET THE METATABLE ON USER DATA 
	lua_setmetatable(L, userdata);

	// SET table[ 0 ] = USERDATA; 
	lua_settable(L, newtable);

	// GET CLASS META TABLE FROM GLOBAL LUA_REGISTRYINDEX 
	luaL_getmetatable(L, T::className);
	// SET THE METATABLE ON newtable 
	lua_setmetatable(L, newtable);


	// REGISTER PROPERTIES on METATABLE 
	luaL_getmetatable(L, T::className);

	// ADDS PROPERTY NAMES AND DEFUALT VALUES TO THE TABLE 
	for (int i = 0; T::Properties[i].name; i++) {
	    // ADD NAME KEY 
	    lua_pushstring(L, T::Properties[i].name);
	    lua_pushnumber(L, i);
	    lua_settable(L, -3);
	}

	// POP OFF META 
	lua_pop(L, 1);


	// REGISTER FUNCTIONS 
	// THIS MAKES A WRAPPER FUNCTION THAT WHEN CALLED TRIGGERS
	// function_dispatch() 
	// THE WRAPPER HOLDS A INDEX TO THE ARRAY OF FUNCTIONS TO CALL 

	// MAKES A CLOSER WITH THE NAME? AND 1 VALUE OF i? 
	// A CLOUSER IS A FUNCTION WITH BOUND VARS 
	// WHEN CALLED THE FUNCTION CAN ACCESS THESE VARS 

	for (int i = 0; T::Functions[i].name; i++) {
	    lua_pushstring(L, T::Functions[i].name);
	    lua_pushnumber(L, i);
	    lua_pushcclosure(L, &Luna < T >::function_dispatch, 1);
	    lua_settable(L, newtable);
	}

	return 1;
    }

    static T       *createNew(lua_State * L) {

	// MAKE A NEW TABLE TO RETURN 
	// AKA OUR NEW LUA OBJECT 
	lua_newtable(L);

	// GET ADDRESS OF newtable ON STACK 
	int             newtable = lua_gettop(L);

	// PUSH INDEX OF USER DATA 
	lua_pushnumber(L, 0);

	// MAKE USER DATA ON THE STACK AND POINT IT TO LUNA OBJECT 
	T             **a = (T **) lua_newuserdata(L, sizeof(T *));
	T              *obj = new T(L, false);
	obj->isExisting = false;
	*a = obj;

	// const PropertyType *p; 
	// p = T::Properties; 
	// p = (*a)->T::Properties; 

	int             userdata = lua_gettop(L);

	// GET CLASS META TABLE FROM GLOBAL LUA_REGISTRYINDEX 
	luaL_getmetatable(L, T::className);

	// SET THE METATABLE ON USER DATA 
	lua_setmetatable(L, userdata);

	// SET table[ 0 ] = USERDATA; 
	lua_settable(L, newtable);


	// GET CLASS META TABLE FROM GLOBAL LUA_REGISTRYINDEX 
	luaL_getmetatable(L, T::className);
	// SET THE METATABLE ON newtable 
	lua_setmetatable(L, newtable);


	// REGISTER PROPERTIES on METATABLE 
	luaL_getmetatable(L, T::className);

	// ADDS PROPERTY NAMES AND DEFUALT VALUES TO THE TABLE 
	for (int i = 0; T::Properties[i].name; i++) {
	    // ADD NAME KEY 
	    lua_pushstring(L, T::Properties[i].name);
	    lua_pushnumber(L, i);
	    lua_settable(L, -3);
	}

	// POP OFF META 
	lua_pop(L, 1);


	// REGISTER FUNCTIONS 
	// THIS MAKES A WRAPPER FUNCTION THAT WHEN CALLED TRIGGERS
	// function_dispatch() 
	// THE WRAPPER HOLDS A INDEX TO THE ARRAY OF FUNCTIONS TO CALL 

	// MAKES A CLOSER WITH THE NAME? AND 1 VALUE OF i? 
	// A CLOUSER IS A FUNCTION WITH BOUND VARS 
	// WHEN CALLED THE FUNCTION CAN ACCESS THESE VARS 

	for (int i = 0; T::Functions[i].name; i++) {
	    lua_pushstring(L, T::Functions[i].name);
	    lua_pushnumber(L, i);
	    lua_pushcclosure(L, &Luna < T >::function_dispatch, 1);
	    lua_settable(L, newtable);
	}

	return obj;
    }

    static T       *createFromExisting(lua_State * L, T * existingobj) {

	// MAKE A NEW TABLE TO RETURN 
	// AKA OUR NEW LUA OBJECT 
	lua_newtable(L);

	// GET ADDRESS OF newtable ON STACK 
	int             newtable = lua_gettop(L);

	// PUSH INDEX OF USER DATA 
	lua_pushnumber(L, 0);

	// MAKE USER DATA ON THE STACK AND POINT IT TO LUNA OBJECT 
	T             **a = (T **) lua_newuserdata(L, sizeof(T *));
	T              *obj = existingobj;
	obj->isExisting = true;
	*a = obj;

	// const PropertyType *p; 
	// p = T::Properties; 
	// p = (*a)->T::Properties; 

	int             userdata = lua_gettop(L);


	// GET CLASS META TABLE FROM GLOBAL LUA_REGISTRYINDEX 
	luaL_getmetatable(L, T::className);

	// SET THE METATABLE ON USER DATA 
	lua_setmetatable(L, userdata);

	// SET table[ 0 ] = USERDATA; 
	lua_settable(L, newtable);

	// GET CLASS META TABLE FROM GLOBAL LUA_REGISTRYINDEX 
	luaL_getmetatable(L, T::className);
	// SET THE METATABLE ON newtable 
	lua_setmetatable(L, newtable);


	// REGISTER PROPERTIES on METATABLE 
	luaL_getmetatable(L, T::className);

	// ADDS PROPERTY NAMES AND DEFUALT VALUES TO THE TABLE 
	for (int i = 0; T::Properties[i].name; i++) {
	    // ADD NAME KEY 
	    lua_pushstring(L, T::Properties[i].name);
	    lua_pushnumber(L, i);
	    lua_settable(L, -3);
	}

	// POP OFF META 
	lua_pop(L, 1);


	// REGISTER FUNCTIONS 
	// THIS MAKES A WRAPPER FUNCTION THAT WHEN CALLED TRIGGERS
	// function_dispatch() 
	// THE WRAPPER HOLDS A INDEX TO THE ARRAY OF FUNCTIONS TO CALL 

	// MAKES A CLOSER WITH THE NAME? AND 1 VALUE OF i? 
	// A CLOUSER IS A FUNCTION WITH BOUND VARS 
	// WHEN CALLED THE FUNCTION CAN ACCESS THESE VARS 

	for (int i = 0; T::Functions[i].name; i++) {
	    lua_pushstring(L, T::Functions[i].name);
	    lua_pushnumber(L, i);
	    lua_pushcclosure(L, &Luna < T >::function_dispatch, 1);
	    lua_settable(L, newtable);
	}

	return obj;
    }

    // PROPERTY DISPATCH 
    static int      property_getter(lua_State * L) {

	// GET VAR NAME OFF STACK 
	//string          _name = lua_tostring(L, 2);	// UNCOMMENT THIS
	// IS YOU NEED TO
	// SEE THE NAME IN 
	// THE DEBUGGER 
	lua_pushvalue(L, 2);

	// DO A NORMAL GET 
	//lua_rawget(L, 1);

	//if (lua_isnil(L, -1)) {
	    // POP NIL VALUE FROM NORMAL GET 
	    // lua_remove( L, -1 ); 
	//    lua_pop(L, 1);
	//} else {
	    // RETURN NORMAL GET SUCCEEDED 
	//    return 1;
//	}

	// string _name = lua_tostring( L, 2 ); 

	// GET METABLE 
	lua_getmetatable(L, 1);

	// LOOK UP KEY IN META TABLE 
	lua_pushvalue(L, 2);
	// lua_gettable( L, -2 ); 
	lua_rawget(L, -2);

	if (lua_isnumber(L, -1)) {

	    // GET ARRAY INDEX OF PROPERTY STORED IN METABLE 
	    int             _index = lua_tonumber(L, -1);

	    // GET table[ 0 ] 
	    lua_pushnumber(L, 0);
	    // lua_gettable( L, 1 ); 
	    lua_rawget(L, 1);

	    // GETS THE USER DATA FROM table[ 0 ] 
	    T             **obj =
		static_cast < T ** >(lua_touserdata(L, -1));

	    // POP OFF STACK 
	    // lua_pop( L, 1 ); 
	    // lua_remove( L, -1 ); 

	    // PUSH VALUE TO TOP OF STACK 
	    // FOR FUNCTION CALL 
	    lua_pushvalue(L, 3);

	    // CACHE ARRAY 
	    const PropertyType *_properties = (*obj)->T::Properties;

	    // CALL FUNCTION 
		int result = ((*obj)->*(T::Properties[_index].getter)) (L);

		/*
		for (int i=1;i<lua_gettop(L);i+=1)
		{
			if (lua_istable(L,i))
			{
				cout << i << ": table" << endl;
			}
			else if (lua_isnumber(L,i))
			{
				cout << i << ": number : " << lua_tonumber(L,i) << endl;
			}
			else if (lua_isstring(L,i))
			{
				cout << i << ": string : " << lua_tostring(L,i)  << endl;
			}
		}
		*/
	    return result;

	}
	// PUSH NIL 
	lua_pushnil(L);

	return 1;

    }


    // PROPERTY DISPATCH 
    static int      property_setter(lua_State * L) {

	// if (lua_isstring(L,2))
	// cout << "SET PROPERTY NAME: " << lua_tostring(L,2) << " - " <<
	// lua_gettop(L) << endl;
	// if (!lua_isnoneornil(L,3))// && 3<=lua_gettop(L)-3)
	// cout << "SET PROPERTY VALUE: " << lua_tonumber(L,3) << endl;

	// GET VAR NAME OFF STACK 
	// string _name = lua_tostring( L, 2 ); 
	// int _name_index = lua_gettop( L ); 
	// 
	// if (_name_index > 2)
	// {
	// // GET VAR VALUE OFF STACK 
	// string _value = lua_tostring( L, 3 ); 
	// //cout << "Property set " << _name << " to " << _value << endl;
	// }

	// GET METABLE 
	lua_getmetatable(L, 1);

	// LOOK UP KEY IN META TABLE 
	lua_pushvalue(L, 2);
	// lua_gettable( L, -2 ); 
	lua_rawget(L, -2);

	if (lua_isnil(L, -1)) {

	    // POP KEY 
	    // POP METATABLE 
	    lua_pop(L, 2);

	    // PROPERTY NOT FOUND 
	    // DO A NORMAL SET 
	    lua_rawset(L, 1);

	    return 0;
	} else {

	    // GET ARRAY INDEX OF PROPERTY STORED IN METABLE 
	    int             _index = lua_tonumber(L, -1);
	    // lua_pop( L, 1 ); 

	    // POP OFF METATABLE 
	    // lua_pop( L, 1 ); 

	    // GET table[ 0 ] 
	    lua_pushnumber(L, 0);
	    // lua_gettable( L, 1 ); 
	    lua_rawget(L, 1);

	    // GETS THE USER DATA FROM table[ 0 ] 
	    T             **obj =
		static_cast < T ** >(lua_touserdata(L, -1));

	    // POP OFF USER DATA 
	    // THIS SHOULD LEAVE THE VALUE AT THE TOP OF THE STACK FOR THE 
	    // 
	    // FUNCTION CALL 
	    // lua_pop( L, 1 ); 

	    // PUSH VALUE TO TOP OF STACK 
	    // FOR FUNCTION CALL 
	    lua_pushvalue(L, 3);

	    // cout << "PROPERTY TOP: " << lua_gettop(L);
	    // if (!lua_isnoneornil(L,lua_gettop(L)))// &&
	    // 3<=lua_gettop(L)-3)
	    // cout << "SET PROPERTY VALUE 2: " <<
	    // lua_tonumber(L,lua_gettop(L)) << endl;

	    // CACHE ARRAY 
	    const PropertyType *_properties = (*obj)->T::Properties;

	    return ((*obj)->*(T::Properties[_index].setter)) (L);

	}

    }

    // FUNCTION DISPATCH 
    static int      function_dispatch(lua_State * L) {

	if (lua_gettop(L) == 0 || !lua_istable(L,1))
	{
		debug("Do not use the . operator on class functions.  Use the : operator to make function calls.",LVL_ERR,L);
		return 0;
	}

	// GET FUNCTION INDEX FROM CLOSURE 
	int             i = (int) lua_tonumber(L, lua_upvalueindex(1));
	
	// GET table[ 0 ] 
	lua_pushnumber(L, 0);
	// lua_gettable( L, 1 ); 
	lua_rawget(L, 1);

	// GETS THE USER DATA FROM? / FOR? table[ 0 ] 
	T             **obj = static_cast < T ** >(lua_touserdata(L, -1));

	// POP OFF STACK 
	// lua_remove(L, -1 ); 
	lua_pop(L, 1);

	// CALL FUNCTION 
	return ((*obj)->*(T::Functions[i].function)) (L);
    }

    // GC FOR USER DATA ONLY 
    static int      gc_obj(lua_State * L) {

	T             **obj =
	    static_cast < T ** >(luaL_checkudata(L, -1, T::className));

	// if (T::irrlichtCleanupRequired)
	// T::cleanUp();

	if (!(*obj)->isExisting && !(*obj)->isPrecious)
	{
		cout << "Cleaning up a " << T::className << "." << endl;
		delete(*obj);
	}

	return 0;
    }

};
