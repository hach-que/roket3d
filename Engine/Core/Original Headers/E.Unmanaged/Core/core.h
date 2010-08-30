#ifndef HEADER_RCORE
#define HEADER_RCORE

#include "eventreciever.h"
#include "rdevice.h"
#include "rcursorcontrol.h"

// Vectors, Matrixes, etc..
#include "E.Collections/rrectangle.h"
#include "E.Collections/rvector3d.h"
#include "E.Collections/rvector2d.h"
#include "E.Collections/rray2d.h"
#include "E.Collections/rray3d.h"
#include "E.Collections/rtriangle3d.h"
#include "E.Collections/rcube3d.h"
#include "E.Collections/rplane.h"
#include "E.Collections/rquaternion.h"
#include "E.Collections/rcolor.h"
#include "E.Collections/rmatrix.h"

void CoreInitalize (lua_State * L);

#endif
