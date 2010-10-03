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
#include <vector>

typedef lua_Number numeric;

class string
{
public:
	string()
	{
		this->len = 0;
		this->data = (wchar_t*)malloc(sizeof(wchar_t));
		this->data[0] = 0;
	}

	string(const char* text)
	{
		// Allocate memory to be used by the string.
		this->len = strlen(text);
		this->data = (wchar_t*)malloc((this->len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < this->len; i += 1)
			this->data[i] = text[i];
		this->data[this->len] = 0; // NULL terminator.
	}

	string(const wchar_t* text)
	{
		// Allocate memory to be used by the string.
		this->len = wcslen(text);
		this->data = (wchar_t*)malloc((this->len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < this->len; i += 1)
			this->data[i] = text[i];
		this->data[this->len] = 0; // NULL terminator.
	}

	~string()
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

	bool operator ==(const string& b) const
	{
		// String comparison.
		return (wcscmp(this->data, b.data) == 0);
	}

private:
	wchar_t* data;
	size_t len;
};

// Predeclare class references for storage structs.
class table;
class RObject;

namespace tableutil
{
	// Enum for returning the type of object.
	enum ValueType
	{
		t_nil,
		t_bool,
		t_numeric,
		t_string,
		t_table,
		t_object,
		t_other
	};

	// Structure for storing keys and values within m_Data.
	struct ValueStorage
	{
		ValueType type;
		struct
		{
			bool b;
			numeric n;
			::string s;
			table * t;
			RObject * o;
		} store;

		ValueStorage() { type = t_nil; }
		ValueStorage(bool v) { type = t_bool; store.b = v; }
		ValueStorage(numeric v) { type = t_numeric; store.n = v; }
		ValueStorage(::string v) { type = t_string; store.s = v; }
		ValueStorage(table * v) { type = t_table; store.t = v; }
		ValueStorage(RObject * v) { type = t_object; store.o = v; }

		bool operator ==(const ValueStorage& b) const
		{
			if (type != b.type) return false;
			switch (type)
			{
			case t_nil:
				return (b.type == t_nil);
			case t_bool:
				return (store.b == b.store.b);
			case t_numeric:
				return (store.n == b.store.n);
			case t_string:
				return (store.s == b.store.s);
			case t_table:
				return (store.t == b.store.t);
			case t_object:
				return (store.o == b.store.o);
			case t_other:
				return false;
			default:
				return false;
			}
		}
	};

	// Structure that's returned when accessing via index.  Overloads
	// the assignment operator so that C++ users can assign to the
	// variable.
	struct value
	{
	private:
		table * container;
		ValueStorage key;
		ValueStorage val;

	public:
		ValueType type;

		value(ValueStorage key, ValueStorage value)
		{
			type = value.type;
		}

		value& operator =(const bool& v);
		value& operator =(const numeric& v);
		value& operator =(const ::string& v);
		value& operator =(const table*& v);
		value& operator =(const RObject*& v);

		operator bool() { if (this->val.type == t_bool) return this->val.store.b; else return false; }
		operator numeric() { if (this->val.type == t_numeric) return this->val.store.n; else return 0; }
		operator ::string() { if (this->val.type == t_string) return this->val.store.s; else return ""; }
		operator table *() { if (this->val.type == t_table) return this->val.store.t; else return NULL; }
		operator RObject *() { if (this->val.type == t_object) return this->val.store.o; else return NULL; }
	};
}

class table
{
public:
	//inline table()
	//{
	//}

	//inline table(table & copy)
	//{
	//	// Copy contents of the other tables m_Data.
	//}

	void Set(bool key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table * key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(RObject * key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, bool value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }
	void Set(bool key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table * key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(RObject * key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, numeric value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }
	void Set(bool key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table * key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(RObject * key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, ::string value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }
	void Set(bool key, table * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, table * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, table * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table * key, table * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(RObject * key, table * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, table * value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }
	void Set(bool key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table * key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(RObject * key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, RObject * value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }

	tableutil::value operator [](bool key) { return tableutil::value(tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }
	tableutil::value operator [](numeric key) { return tableutil::value(tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }
	tableutil::value operator [](::string key) { return tableutil::value(tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }
	tableutil::value operator [](table * key) { return tableutil::value(tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }
	tableutil::value operator [](RObject * key) { return tableutil::value(tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }

	void SetKeyValue(tableutil::ValueStorage key, tableutil::ValueStorage value)
	{
		for (m_DataIterator i = this->m_Data.begin();
			i != this->m_Data.end(); i++)
		{
			if (i->first == key)
			{
				i->second = value;
				return;
			}
		}
		
		// Not found, add.
		this->m_Data.insert(this->m_Data.end(), m_Pair(key, value));
		return;
	}

	inline void PushAsResult()
	{
	}

private:
	tableutil::ValueStorage GetValueByKey(tableutil::ValueStorage key)
	{
		for (m_DataIterator i = this->m_Data.begin();
			i != this->m_Data.end(); i++)
		{
			if (i->first == key)
				return i->second;
		}

		return tableutil::ValueStorage(); // Return nil.
	}

	std::vector<std::pair<tableutil::ValueStorage, tableutil::ValueStorage>> m_Data;
	typedef std::vector<std::pair<tableutil::ValueStorage, tableutil::ValueStorage>>::iterator m_DataIterator;
	typedef std::pair<tableutil::ValueStorage, tableutil::ValueStorage> m_Pair;
};

tableutil::value& tableutil::value::operator =(const bool& v) { container->Set(this->key, v); this->val.store.b = v; this->type = this->val.type = t_bool; return *this; }
tableutil::value& tableutil::value::operator =(const numeric& v) { container->Set(this->key, v); this->val.store.n = v; this->type = this->val.type = t_numeric; return *this; }
tableutil::value& tableutil::value::operator =(const ::string& v) { container->Set(this->key, v); this->val.store.s = v; this->type = this->val.type = t_string; return *this; }
tableutil::value& tableutil::value::operator =(const table*& v)
{
	container->Set(this->key, const_cast<table*>(v));
	this->val.store.t = const_cast<table*>(v);
	this->type = this->val.type = t_table;
	return *this;
}
tableutil::value& tableutil::value::operator =(const RObject*& v)
{
	container->Set(this->key, const_cast<RObject*>(v));
	this->val.store.o = const_cast<RObject*>(v);
	this->type = this->val.type = t_object;
	return *this;
}

#endif