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
#include <map>

typedef lua_Number numeric;

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

template<class T>
	class table
	{
	public:
		// Enum for returning the type of object.
		typedef enum
		{
			t_nil,
			t_bool,
			t_numeric,
			t_string,
			t_table,
			t_object,
			t_other
		} ValueType;

	private:
		// Structure for storing keys and values within m_Data.
		typedef struct
		{
			m_TypeStorage type;
			union
			{
				bool b;
				numeric n;
				string s;
				table * t;
				T * o;
			} store;

			struct(bool v)
			{
				type = t_bool;
				store.b = v;
			}

			struct(numeric v)
			{
				type = t_numeric;
				store.n = v;
			}

			struct(string v)
			{
				type = t_string;
				store.s = v;
			}

			struct(table * v)
			{
				type = t_table;
				store.t = v;
			}

			struct(T * v)
			{
				type = t_object;
				store.o = v;
			}
		} m_ValueStorage;

	public:
		// Structure that's returned when accessing via index.  Overloads
		// the assignment operator so that C++ users can assign to the
		// variable.
		typedef struct
		{
		private:
			table * container;
			m_ValueStorage key;
			m_ValueStorage value;

		public:
			ValueType type;

			struct(m_ValueStorage key, m_ValueStorage value)
			{
				type = value.type;
			}

			value& operator =(const bool& v)
			{
				container->Set(key, v);
				value.store.b = v;
				type = t_bool;
			}

			value& operator =(const numeric& v)
			{
				container->Set(key, v);
				value.store.n = v;
				type = t_numeric;
			}

			value& operator =(const string& v)
			{
				container->Set(key, v);
				value.store.s = v;
				type = t_string;
			}

			value& operator =(const table& v)
			{
				container->Set(key, v);
				value.store.t = v;
				type = t_table;
			}

			value& operator =(const T*& v)
			{
				container->Set(key, v);
				value.store.o = v;
				type = t_object;
			}

			operator bool()
			{
				if (value.type == t_bool)
					return value.b;
			}

			operator numeric()
			{
				if (value.type == t_numeric)
					return value.n;
			}

			operator string()
			{
				if (value.type == t_string)
					return value.s;
			}

			operator table *()
			{
				if (value.type == t_table)
					return value.t;
			}

			operator T *()
			{
				if (value.type == t_object)
					return value.o;
			}
		} value;

		inline table()
		{
		}

		inline table(table & copy)
		{
			// Copy contents of the other tables m_Data.
		}

		value operator [](bool key)
		{
			return value(m_ValueStorage(key), this->GetValueByKey(m_ValueStorage(key)));
		}

		value operator [](numeric key)
		{
			return value(m_ValueStorage(key), this->GetValueByKey(m_ValueStorage(key)));
		}

		value operator [](string key)
		{
			return value(m_ValueStorage(key), this->GetValueByKey(m_ValueStorage(key)));
		}

		value operator [](table key)
		{
			return value(m_ValueStorage(key), this->GetValueByKey(m_ValueStorage(key)));
		}

		value operator [](T * key)
		{
			return value(m_ValueStorage(key), this->GetValueByKey(m_ValueStorage(key)));
		}

		inline void Set(bool key, bool value)
		{
		}

		inline void Set(numeric key, bool value)
		{
		}

		inline void Set(string key, bool value)
		{
		}

		inline void Set(table key, bool value)
		{
		}

		inline void Set(T * key, bool value)
		{
		}

		inline void Set(bool key, numeric value)
		{
		}

		inline void Set(numeric key, numeric value)
		{
		}

		inline void Set(string key, numeric value)
		{
		}

		inline void Set(table key, numeric value)
		{
		}

		inline void Set(T * key, numeric value)
		{
		}

		inline void Set(bool key, string value)
		{
		}

		inline void Set(numeric key, string value)
		{
		}

		inline void Set(string key, string value)
		{
		}

		inline void Set(table key, string value)
		{
		}

		inline void Set(T * key, string value)
		{
		}

		inline void Set(bool key, table value)
		{
		}

		inline void Set(numeric key, table value)
		{
		}

		inline void Set(string key, table value)
		{
		}

		inline void Set(table key, table value)
		{
		}

		inline void Set(T * key, table value)
		{
		}

		inline void Set(bool key, T * value)
		{
		}

		inline void Set(numeric key, T * value)
		{
		}

		inline void Set(string key, T * value)
		{
		}

		inline void Set(table key, T * value)
		{
		}

		inline void Set(T * key, T * value)
		{
		}

		inline int GetTableIndex()
		{
			return this->m_TableIndex;
		}

		inline void PushAsResult()
		{
		}

	private:
		m_ValueStorage GetValueByKey(m_ValueStorage key)
		{
		}

		std::map<m_ValueStorage, m_ValueStorage> m_Data;
	}

// TODO: Replace the typedefs with actual classes
//       which properly handle the conversion and
//       can be handled properly by the Bindings<>
//       template.

#endif