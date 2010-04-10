//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// basic Hierarchical Scene Node Class  
//********************************************************************

#ifndef AFX_DEBUG_DISPLAY_H____INCLUDED_
#define AFX_DEBUG_DISPLAY_H____INCLUDED_

#include <toolbox_stdafx.h>
#include <dVector.h>
#include <dMatrix.h>



bool DebugDisplayOn();
void DebugDisplaySetMode(bool mode);


void DebugDrawLine (const dVector& p0, const dVector& p1, dVector color = dVector (1.0f, 1.0f, 1.0f, 1.0f));


void DebugDrawAABB (const dVector& p0, const dVector& p1, dVector color = dVector (1.0f, 1.0f, 1.0f, 1.0f));
void DebugDrawPolygon (int count, dVector* points, dVector color = dVector (1.0f, 1.0f, 1.0f, 1.0f));
void DebugDrawContact (const dVector& point, const dVector& normal, dVector color = dVector (1.0f, 1.0f, 0.0f, 1.0f), float lenght = 1.0f);




void DebugDrawCollision (const NewtonCollision* collision, dMatrix& matrix, dVector color = dVector (1.0f, 1.0f, 1.0f, 1.0f));
void DebugRenderWorldCollision (const NewtonWorld* world);

void DebugRenderDebugInfo ();


#endif