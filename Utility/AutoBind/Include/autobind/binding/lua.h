/*
 * This header file is part of the AutoBind
 * toolset.  It defines the Bindings<> template
 * for use within the LuaTranslator code.
 */

#include <string>
#include <vector>

template<class T>
{
	class Bindings
	{
		// TODO: Do we even use / need this?
		typedef struct
		{
			T *pT;
		} UserDataType;

		// TODO: Find out where this enum is used.
		public enum
		{
			NUMBER,
			STRING
		};

		// Defines the structure of the PropertyTypes list
		// (used for telling the bindings how to handle
		// properties in a given object).
		public struct PropertyType
		{
			const char* Name;
			int (T::*Getter)(lua_State * L);
			int (T::*Setter)(lua_State * L);
		}

		// Defines the structre of the FunctionTypes list
		// (used for telling the bindings which functions
		// are bound to Lua).
		public struct FunctionType
		{
			const char* Name;
			int (T::*Function)(lua_State * L);
		}

		// A static function for retrieving an object from
		// the arguments provided to a function.  It is also
		// used during property setting, where arg should be
		// -1 to get the value that was assigned.
		public static T* GetArgument(lua_State * L, int arg)
		{
			// Check to make sure that it's a table at that
			// position.  If it isn't, then it can't be a
			// class.
			// TODO: Check for numeric / strings and handle
			//       them if T is the correct type.
			if (lua_istable(L, narg + 1))
			{
				// Get the value at the first index of the table, and
				// push it onto the stack.
				lua_gettablevalue(L, narg + 1, 0);

				// Convert the value that was just pushed onto the
				// stack into userdata if possible.  The luaL_testudata
				// checks to make sure that the class is the correct
				// type before converting it.
				UserDataType* ud = static_cast<UserDataType*>(luaL_testudata(L, -1, T::ClassName));
				if (ud == NULL)
				{
					// Return NULL to indicate that the argument is not
					// of the specified type.
					return NULL;
				}

				// Pop the value from the stack as we no longer need it.
				lua_pop(L, 1);

				// Return a pointer to the object in the specified argument
				// position.
				return ud->pT;
			}
			else
			{
				// Return NULL to indicate that the argument could not
				// be handled by the Bindings<> class.
				return NULL;
			}
		}

		// Registers the given class with the Lua engine.
		public static void Register(lua_State * L)
		{
			// We use the std::string class to parse the
			// T::ClassName variable.  T::ClassName not only
			// contains the class name of the class, but also
			// the namespace context as well.
			std::string cls = T::ClassName;
			std::string buf = "";
			std::vector<std::string> elms = new std::vector<std::string>();
			for (int i = 0; i < cls.length(); i++)
			{
				if (cls[i] == '.')
				{
					if (buf.length() > 0)
					{
						elms.insert(elms.end(), buf);
						buf = "";
					}
				}
				else
					buf += cls[i];
			}
			if (buf.length() > 0)
			{
				elms.insert(elms.end(), buf);
				buf = "";
			}

			// Make sure there is enough stack space to perform
			// all of the namespace loading, etc..
			lua_checkstack(L, elms.size() + 5);

			// Now loop through elms, loading all but the last element
			// as namespace tables.
			int topop = 0;
			for (int i = 0; i < elms.size(); i++)
			{
				if (i < elms.size() - 1)
				{
					if (i == 0)
					{
						// Fetch the first from the global area.
						lua_getglobal(L, elms[i].c_str());
						topop++;
					}
					else
					{
						lua_getfield(L, -1, elms[i].c_str());
						topop++;
					}
				}
				else
				{
					if (i == 0)
					{
						// Set directly into the global namespace.
						lua_pushcfunction(L, &Bindings<T>::Constructor);
						lua_setglobal(L, T::ClassName);
					}
					else
					{
						// Set into the namespace table.
						lua_pushcfunction(L, &Bindings<T>::Constructor);
						lua_setfield(L, -2, T::ClassName);
					}
				}
			}
			lua_pop(L, topop);

			// Create a new metatable, used for storing the userdata
			// associated with our class registration.
			luaL_newmetatable(L, T::ClassName);
			int metatable = lua_gettop(L);

			// Now set the index of T::ClassName in the registry table
			// to be associated with the metatable.
			lua_pushvalue(L, -1);
			lua_pushstring(L, T::className);
			lua_rawset(L, LUA_REGISTRYINDEX);

			// Add our metatable functions such as __gc, __index and
			// __setindex.
			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, &Bindings<T>::GCObj);
			lua_settable(L, metatable);

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, &Bindings<T>::PropertyGetter);
			lua_settable(L, metatable);

			lua_pushstring(L, "__setindex");
			lua_pushcfunction(L, &Bindings<T>::PropertySetter);
			lua_settable(L, metatable);
		}

		// The callback function used when a class is constructed.
		public static int Constructor(lua_State * L)
		{
			// Create a new table with which to return
			// our new Lua object.
			lua_newtable(L);

			// Get the address of the new table on the
			// stack.
			int newtable = lua_gettop(L);

			// Push the index of the userdata.
			lua_pushnumber(L, 0);

			// Essentially here we're setting the userdata in
			// the metatable to the Bindings<T> object.  This
			// allows the function callbacks and property setter
			// and getter functions to know what C++ object they
			// are dealing with.
			T** ud = (T**)lua_newuserdata(L, sizeof(T*));
			T* obj = new T(L, true);
			*ud = obj;

			// Get the address of the userdata on the
			// stack.
			int userdata = lua_gettop(L);

			// Call the SetupObject() function which sets up the
			// properties and functions on the new object and
			// pushes it onto the stack.  We use this function so
			// that we don't repeat code between Constructor, CreateNew
			// and CreateFromExisting.
			Bindings<T>::SetupObject(L, newtable, userdata);

			// Return a value of 1, indicating that we're returning
			// a new object.
			return 1;
		}

		// Creates a new instance of the C++ object, like the Constructor
		// callback, but returns the object itself for use in C++ code
		// (unlike Constructor which is intended to be used from Lua).
		public static T* CreateNew(lua_State * L)
		{
			// Create a new table with which to return
			// our new Lua object.
			lua_newtable(L);

			// Get the address of the new table on the
			// stack.
			int newtable = lua_gettop(L);

			// Push the index of the userdata.
			lua_pushnumber(L, 0);

			// Essentially here we're setting the userdata in
			// the metatable to the Bindings<T> object.  This
			// allows the function callbacks and property setter
			// and getter functions to know what C++ object they
			// are dealing with.
			T** ud = (T**)lua_newuserdata(L, sizeof(T*));
			T* obj = new T(L, true);
			*ud = obj;

			// Get the address of the userdata on the
			// stack.
			int userdata = lua_gettop(L);

			// Call the SetupObject() function which sets up the
			// properties and functions on the new object and
			// pushes it onto the stack.  We use this function so
			// that we don't repeat code between Constructor, CreateNew
			// and CreateFromExisting.
			Bindings<T>::SetupObject(L, newtable, userdata);

			// Instead of returning the number of objects we pushed
			// onto the stack, we return the actual object itself since
			// CreateNew will be used from C++ code.
			return obj;
		}

		// Creates a new instance of the Lua object, which is wrapping
		// an existing C++ object.  Returns the object itself for use
		// in C++ code (which will be the same as the 'existing' argument).
		public static T* CreateFromExisting(lua_State * L, T* existing)
		{
			// Create a new table with which to return
			// our new Lua object.
			lua_newtable(L);

			// Get the address of the new table on the
			// stack.
			int newtable = lua_gettop(L);

			// Push the index of the userdata.
			lua_pushnumber(L, 0);

			// Essentially here we're setting the userdata in
			// the metatable to the Bindings<T> object.  This
			// allows the function callbacks and property setter
			// and getter functions to know what C++ object they
			// are dealing with.
			T** ud = (T**)lua_newuserdata(L, sizeof(T*));
			T* obj = existing;
			obj->IsExisting = true;
			*ud = obj;

			// Get the address of the userdata on the
			// stack.
			int userdata = lua_gettop(L);

			// Call the SetupObject() function which sets up the
			// properties and functions on the new object and
			// pushes it onto the stack.  We use this function so
			// that we don't repeat code between Constructor, CreateNew
			// and CreateFromExisting.
			Bindings<T>::SetupObject(L, newtable, userdata);

			// Instead of returning the number of objects we pushed
			// onto the stack, we return the actual object itself since
			// CreateNew will be used from C++ code.
			return obj;
		}

		// Sets up the object at the specified 'newtable' index and the
		// userdata associated with it at the specified 'userdata' index.
		// This function exists to reduce code duplication.
		private static void SetupObject(lua_State * L, int newtable, int userdata)
		{
			// Retrieve the metatable from the registry index (the
			// one we set in the Register function) and set it as
			// the metatable on the userdata.
			luaL_getmetatable(L, T::ClassName);
			lua_setmetatable(L, userdata);

			// Set the first index of the table to the userdata
			// (we pushed the index above using lua_pushnumber).
			lua_settable(L, newtable);

			// Now we set the same metatable that we associated with
			// the userdata as the metatable on the new object we
			// are returning.
			luaL_getmetatable(L, T::ClassName);
			lua_setmetatable(L, newtable);

			// Now we're going to register the properties with our
			// metatable, so that setting or getting the properties
			// on the Lua object will result in the appropriate
			// callbacks being issued.
			luaL_getmetatable(L, T::ClassName);
			for (int i = 0; T::Properties[i].Name != NULL; i++)
			{
				lua_pushstring(L, T::Properties[i].Name);
				lua_pushnumber(L, i);
				lua_settable(L, -3);
			}
			lua_pop(L, 1);

			// Now register the functions on the object with the
			// Bindings<T>::FunctionDispatch, which then handles
			// passing the correct context onto the class functions.
			// Note that unlike the properties, the functions are
			// associated with new object and not the metatable.
			for (int i = 0; T::Functions[i].Name; i++)
			{
				lua_pushstring(L, T::Functions[i].Name);
				lua_pushnumber(L, i);
				lua_pushcclosure(L, &Bindings<T>::FunctionDispatch, 1);
				lua_settable(L, newtable);
			}
		}

		// The callback which handles when properties are asked
		// to be retrieved from the object.
		public static int PropertyGetter(lua_State * L)
		{
			// When this function is called, the object
			// is at index 1 and the name of the property
			// is at index 2.

			// Retrieve the metatable for the object.
			lua_getmetatable(L, 1);

			// Push the name of the property onto the stack
			// and then fetch the index of that property.
			lua_pushvalue(L, 2);
			lua_rawget(L, -2);

			// Check to make sure that the value on the stack
			// is a number (i.e. the index of the property).
			// If it isn't a number, then the property doesn't
			// exist.
			if (lua_isnumber(L, -1))
			{
				// Store the index.
				int index = lua_tonumber(L, -1);

				// Get the first entry in the metatable, which
				// is the userdata associated with the object.
				lua_pushnumber(L, 0);
				lua_rawget(L, 1);

				// Retrieve the userdata.
				T** obj = static_cast<T**>(lua_touserdata(L, -1));

				// Push the value to the top of the stack.
				// TODO: Work out what this does in the context
				//       of property retrieval.
				lua_pushvalue(L, 3);

				// Call the property retrieval function.
				return ((*obj)->*(T::Properties[index].Getter))(L);
			}
			else
			{
				// The property doesn't exist.
				lua_pushnil(L);
				return 1;
			}
		}

		// The callback which handles when properties are asked
		// to be created or modified on the object.
		public static int PropertySetter(lua_State * L)
		{
			// When this function is called, the object
			// is at index 1 and the name of the property
			// is at index 2.  The value to associate the
			// property with is at index 3.
			
			// Retrieve the metatable for the object.
			lua_getmetatable(L, 1);

			// Push the name of the property onto the stack
			// and then fetch the index of that property.
			lua_pushvalue(L, 2);
			lua_rawget(L, -2);

			// Check to see if the result is nil.  If it is,
			// then the property we are setting doesn't exist
			// so we're just going to do a normal set.
			if (lua_isnil(L, -1))
			{
				// Pop the key and the metatable off the stack.
				lua_pop(L, 2);

				// And do a normal set.
				lua_rawset(L, 1);

				return 0;
			}
			else
			{
				// Store the index.
				int index = lua_tonumber(L, -1);

				// Get the first entry in the metatable, which
				// is the userdata associated with the object.
				lua_pushnumber(L, 0);
				lua_rawget(L, 1);

				// Retrieve the userdata.
				T** obj = static_cast<T**>(lua_touserdata(L, -1));

				// Push the value to the top of the stack.
				// This makes the value available at position -1
				// ready for the setter function to use it.
				lua_pushvalue(L, 3);

				// Call the property modification function.
				return ((*obj)->*(T::Properties[index].Setter))(L);
			}
		}

		// The function is called to handle calling the C++
		// functions from within Lua.
		public static int FunctionDispatch(lua_State * L)
		{
			// Check to make sure that the function was called
			// with the : operator and not the . operator.  The
			// . operator does not provide us with an instance
			// context.
			if (lua_gettop(L) == 0 || !lua_istable(L, 1))
			{
				throw new Bindings<void>::NoContextProvidedError();
				return 0;
			}

			// Retrieve the index of the function from the
			// closure that was created during setup.
			int i = (int)lua_tonumber(L, lua_upvalueindex(1));

			// The table is located at index 1, so we can pull
			// the userdata associated with it just like we
			// do in the property functions (except that we don't
			// need to load the metatable onto the stack).
			lua_pushnumber(L, 0);
			lua_rawget(L, 1);

			// Retrieve the userdata.
			T** obj = static_cast<T**>(lua_touserdata(L, -1));

			// Pop the original table off the stack so that
			// argument 1 is at index 1 when the function is
			// called.
			lua_pop(L, 1);

			// Call the function and return it's result.
			return ((*obj)->*(T::Functions[i].Function))(L);
		}

		// This function is the callback issued by the Lua
		// garbage collector when there are no more references
		// to an object in Lua.  All of the objects that are
		// binded inherit Bindings<void>::BaseObject at one
		// point, which means they are reference counted.
		public static int GCObj(lua_State * L)
		{
			// Get the userdata from the object.
			T** obj = static_cast<T**>(luaL_checkudata(L, -1, T::ClassName));

			// IsPrecious won't be required when objects are reference counted.
			if (!(*obj)->IsExisting) && !(*obj)->IsPrecious))
			{
				delete *obj;
			}

			return 0;
		}


	};
}
