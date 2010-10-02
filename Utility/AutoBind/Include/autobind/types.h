/*
 * This header file is part of the AutoBind
 * toolset.  It typedef's generic types such
 * as string and numeric to their C++
 * equivilants.
 */

#ifndef AUTOBIND_types
#define AUTOBIND_types

// Include Lua.
#include <lua.hpp>
#include <malloc.h>
#include <string.h>
#include <string>

typedef lua_Number numeric;
typedef void table;

class string
{
public:
	inline string(const char* text)
	{
		// Allocate memory to be used by the string.
		this->len = strlen(text);
		this->data = (wchar_t*)malloc((this->len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < this->len; i += 1)
			this->data[i] = text[i];
		this->data[this->len] = 0; // NULL terminator.
	}

	inline string(const wchar_t* text)
	{
		// Allocate memory to be used by the string.
		this->len = wcslen(text);
		this->data = (wchar_t*)malloc((this->len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < this->len; i += 1)
			this->data[i] = text[i];
		this->data[this->len] = 0; // NULL terminator.
	}

	inline ~string()
	{
		// Free the memory used by the string.
		free(this->data);
	}

	operator const char*()
	{
		// Implicit conversion to const char*.  We have to
		// malloc a new string and return it as a reference
		// so that it's still valid when this object goes out
		// of scope.
		char* ret = (char*)malloc((len + 1) * sizeof(char));
		for (size_t i = 0; i < len; i += 1)
			ret[i] = this->data[i];
		ret[len] = 0; // NULL terminator.
		return const_cast<const char*>(ret);
	}

	operator const wchar_t*()
	{
		// Implicit conversion to const wchar_t*.  We have to
		// malloc a new string and return it as a reference
		// so that it's still valid when this object goes out
		// of scope.
		wchar_t* ret = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < len; i += 1)
			ret[i] = this->data[i];
		ret[len] = 0; // NULL terminator.
		return const_cast<const wchar_t*>(ret);
	}

	operator const std::string()
	{
		// Implicit conversion to const std::string.
		const std::string ret = (const char*)this->data;
		return ret;
	}

private:
	wchar_t* data;
	size_t len;
};

// TODO: Replace the typedefs with actual classes
//       which properly handle the conversion and
//       can be handled properly by the Bindings<>
//       template.

#endif