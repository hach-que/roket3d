/*
 * This source file is part of the AutoBind
 * toolset.  It typedef's generic types such
 * as string and numeric to their C++
 * equivilants.
 */

#include "autobind/types.h"

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