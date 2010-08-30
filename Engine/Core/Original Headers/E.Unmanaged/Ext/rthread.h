#if 0==1
#ifndef HEADER_RTHREAD
#define HEADER_RTHREAD
#include "threadenc.h"

namespace ext
{
	class RThread : public ThreadEnc
	{
		RThread(lua_State *L) {}
		int main();
		irr::core::stringc luaFile;
			
		public:
			// Variables from RObject because we already have a base class
			bool isExisting;

			RThread(lua_State* L, bool instantedByUser);
			bool lost;
			int changeFile(lua_State* L);
			int stop(lua_State* L);
			int isFinished(lua_State* L);
			int waitTillFinished(lua_State* L);
			// TODO Implement these functions
			//int sleep(lua_State* L);
			
			static const char *className;
			static const Luna < RThread >::FunctionType Functions[];
			static const Luna < RThread >::PropertyType Properties[];
	};
}

#if !defined(WINDOWS_COMPILE)
template <> int Luna<ext::RThread>::gc_obj(lua_State *L);
#endif

#endif
#endif