#include <lua.hpp>
#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "PermissionDeniedException.h"
#include "PathNotFoundException.h"
#include "Exception.h"

#include "Utility.h"
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
#include <fstream>

namespace Roket3D
{
	bool Utility::FileExists(std::string path)
	{
		if (path.size() == 0)
			return false;
		std::fstream fin;
		fin.open(path.c_str(), std::ios::in);
		if( fin.is_open() )
		{
			fin.close();
			return true;
		}
		fin.close();
		return false;
	}

	void Utility::ChangeWorkingDirectory(std::string path)
	{
#ifdef WIN32
		if (_chdir(path.c_str()) == -1)
#else
		if (chdir(path.c_str()) == -1)
#endif
		{
			switch (errno)
			{
				case EACCES:
					throw new Engine::PermissionDeniedException(path);
#ifndef WIN32
				case ELOOP:
#endif
				case ENAMETOOLONG:
				case ENOENT:
				case ENOTDIR:
					throw new Engine::PathNotFoundException(path);
			}

			// Unknown return value
			throw new Engine::Exception();
		}
	}

	std::string Utility::Replace(std::string input, std::string from, std::string to, int count)
	{
		int lookHere = 0;
		int foundHere;
		int replaceCount = 0;
		while((foundHere = input.find(from, lookHere)) != std::string::npos && (count == -1 || replaceCount < count))
		{
			  input.replace(foundHere, from.size(), to);
			  lookHere = foundHere + to.size();
		}
		return input;
	}

	std::string Utility::ReplaceAll(std::string input, std::string from, std::string to)
	{
		return Utility::Replace(input, from, to, -1);
	}

	std::string Utility::ReplaceOnce(std::string input, std::string from, std::string to)
	{
		return Utility::Replace(input, from, to, 1);
	}

	// The next 2 functions are copied (and modified) from:
	// http://www.daniweb.com/forums/post422997.html#post422997
	std::wstring Utility::ToUnicode(::string text)
	{
		wostringstream wstm ;
		const ctype<wchar_t>& ctfacet = 
							use_facet< ctype<wchar_t> >( wstm.getloc() ) ;
		for( size_t i=0 ; i<strlen(text) ; ++i ) 
				  wstm << ctfacet.widen( text[i] ) ;
		return wstm.str();
	}
	
	std::string Utility::ToASCII(::wstring text)
	{
		ostringstream stm;
		const ctype<char>& ctfacet = 
							 use_facet< ctype<char> >( stm.getloc() ) ;
		for( size_t i=0 ; i<wcslen(text) ; ++i ) 
					  stm << ctfacet.narrow( text[i], 0 ) ;
		return stm.str();
	}
}