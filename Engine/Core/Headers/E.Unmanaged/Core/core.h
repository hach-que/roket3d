#ifndef HEADER_RCORE
#define HEADER_RCORE

#include "eventreciever.h"
#include "rdevice.h"
#include "rcursorcontrol.h"

// Vectors, Matrixes, etc..
#include "rrectangle.h"
#include "rvector3d.h"
#include "rvector2d.h"
#include "rray2d.h"
#include "rray3d.h"
#include "rtriangle3d.h"
#include "rcube3d.h"
#include "rplane.h"
#include "rquaternion.h"
#include "rcolor.h"
#include "rmatrix.h"

void CoreInitalize (lua_State * L);

#endif
