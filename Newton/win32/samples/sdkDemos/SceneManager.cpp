// NewtonDemos.cpp : Defines the entry point for the console application.
//

#include <toolbox_stdafx.h>
#include <Newton.h>
#include "OpenGlUtil.h"
#include "SceneManager.h"
#include "HiResTimer.h"
#include "DebugDisplay.h"
#include "dRuntimeProfiler.h"



static GLUI *glui;
static GLUI_Checkbox* autoSleep;
static GLUI_Checkbox* showIslands;
static GLUI_Checkbox* showContacts;
static GLUI_Checkbox* showCollision;
static GLUI_Checkbox* singleIslandTreading;
static GLUI_RadioGroup* hardwareMode;
static GLUI_RadioGroup* setSolverMode;


static GLUI_Listbox* gameSelection;
static GLUI_Listbox* selectNumberOfthreads;

static int main_window;
static dInt32 memory;


static int showfps; 
static int showProfiler[7]; 


static int hideGeometry; 
static int simulationStopped; 
static int re_entrantUpdate;

//#define START_DEMO  0
//#define START_DEMO  25
#define START_DEMO  35

static SceneManager demo;

struct DemosRecord
{
	int m_id;
	SceneManager::Create m_callback;
};



static int demosCount;
static DemosRecord demosArray[64];

static void DrawScene ();
static void CleanUp ();
static void CreateWorld (int index);

static void SetThreadCount (int id);
static void ChangeSleepMode (int id);
static void ChangeSolverMode (int id);
static void LaunchDemoNewDemo (int id);
static void ChangeShowCollision (int id);
static void ChangeShowContacts (int id);
static void ChangeShowIslands (int id);
static void SetRunStopMode (int id);
static void ChangeChangeIslandTrading (int id);

static void ChangeHardwareMode (int id);
static void RegisterDemo (const char* lable, SceneManager::Create callback);

CHiResTimer timer;

dRuntimeProfiler profiler0 (0, 620 * 0 / 8 + 45, 40);
dRuntimeProfiler profiler1 (1, 620 * 1 / 8 + 45, 40);
dRuntimeProfiler profiler2 (2, 620 * 2 / 8 + 45, 40);
dRuntimeProfiler profiler3 (3, 620 * 3 / 8 + 45, 40);
dRuntimeProfiler profiler4 (4, 620 * 4 / 8 + 45, 40);
dRuntimeProfiler profiler5 (5, 620 * 5 / 8 + 45, 40);
dRuntimeProfiler profiler6 (6, 620 * 6 / 8 + 45, 40);
dRuntimeProfiler profiler7 (7, 620 * 7 / 8 + 45, 40);


dRuntimeProfiler *profilers[] = 
{
	&profiler0, 
	&profiler1, 
	&profiler2, 
	&profiler3,
	&profiler4,
	&profiler5,
	&profiler6,
	&profiler7,
};


int main(int argc, char* argv[])
{

	// initialize opengl	
	main_window = InitOpenGl (argc, argv, "Newton Archemedia demos", DrawScene);

	// create the Gui window main frame 
	glui = GLUI_Master.create_glui_subwindow (main_window, GLUI_SUBWINDOW_RIGHT );
	glui->add_statictext ((char*)"Demos Selection");

	// add demos selection list box
	gameSelection = glui->add_listbox((char*)"", NULL, 0, LaunchDemoNewDemo);
//	gameSelection->orig_value = 110;
	gameSelection->text_x_offset = 5;

	// Register all demo here
	RegisterDemo ((char*)"Simple Sphere", BasicSphereSpin);
	RegisterDemo ((char*)"Closest Distance", ClosestDistance);
	RegisterDemo ((char*)"Convex Cast", ConvexCast);
	RegisterDemo ((char*)"Restitution Coeficients", Restitution);
	RegisterDemo ((char*)"Friction Coeficients", Friction);
	RegisterDemo ((char*)"Precessing Tops", PrecessingTops);

	RegisterDemo ((char*)"Box Stacks", BasicBoxStacks);
	RegisterDemo ((char*)"Cylinder Stacks", BasicCylinderStacks);
	RegisterDemo ((char*)"Convex Stacks", BasicConvexStacks);
	
	RegisterDemo ((char*)"Jenga", Jenga);
	RegisterDemo ((char*)"Jenga Tall", JengaTall);
	RegisterDemo ((char*)"Pyramid", CreatePyramid);
	RegisterDemo ((char*)"Pyramid Tall", CreatePyramidTall);
	RegisterDemo ((char*)"Tower", CreateTower);
	RegisterDemo ((char*)"Tower Tall", CreateTowerTall);
	RegisterDemo ((char*)"Basic Walls", CreateWalls);

	RegisterDemo ((char*)"Unstable Stacks", UnstableStacks);
	RegisterDemo ((char*)"Unstable towers", UnstableStruture);

	RegisterDemo ((char*)"Compound Shapes", CompoundCollision);
	RegisterDemo ((char*)"Scaled Shapes", ScaledCollision);
	
//	RegisterDemo ((char*)"Breakable tower", BreakableStruture);
				  
	RegisterDemo ((char*)"Plane Collision", SimplePlaneCollision);
	RegisterDemo ((char*)"Simple Mesh Collision", SimpleMeshLevelCollision);
	RegisterDemo ((char*)"Optimized Mesh Collision", OptimizedMeshLevelCollision);
	RegisterDemo ((char*)"Height Field Collision", SimpleHeighMapColliion);
	RegisterDemo ((char*)"User Mesh Collision", UserHeighMapColliion);
	RegisterDemo ((char*)"SceneCollision", SceneCollision);

	RegisterDemo ((char*)"Archimedes Buoyancy", ArchimedesBuoyancy);
	RegisterDemo ((char*)"Black Hole", Magnets);
	RegisterDemo ((char*)"White Hole", Bomb);
				  
	RegisterDemo ((char*)"Legacy Joints", LegacyJoints);
	RegisterDemo ((char*)"Custom Joints", BasicCustomJoints);
	RegisterDemo ((char*)"Basic Robots", BasicRobots);
//	RegisterDemo ((char*)"Motorized Robots", MotorizedRobots);
	RegisterDemo ((char*)"Traction Vehicle", TracktionJoints);

	RegisterDemo ((char*)"Destrete Ragdoll", DescreteRagDoll);
	RegisterDemo ((char*)"Skinned Ragdoll", SkinRagDoll);

	RegisterDemo ((char*)"Raycast car", RayCastCar);
//	RegisterDemo ((char*)"Player Controller", PlayerController);
	

	// add solve mode selection
//	glui->add_statictext( "" );
	GLUI_Panel* panel;
	panel = glui->add_panel ((char*)"select solver Mode", GLUI_PANEL_EMBOSSED );
	setSolverMode = glui->add_radiogroup_to_panel (panel, NULL, 0, ChangeSolverMode);
	glui->add_radiobutton_to_group (setSolverMode, (char*)"linear 1 pass");
	glui->add_radiobutton_to_group (setSolverMode, (char*)"linear 2 passes");
	glui->add_radiobutton_to_group (setSolverMode, (char*)"linear 4 passes");
	glui->add_radiobutton_to_group (setSolverMode, (char*)"quadratic");

	// set  hardware mode
	panel = glui->add_panel ((char*)"select solver Mode", GLUI_PANEL_EMBOSSED );
	hardwareMode = glui->add_radiogroup_to_panel (panel, NULL, 0, ChangeHardwareMode);
	glui->add_radiobutton_to_group (hardwareMode, (char*)"simd");
	glui->add_radiobutton_to_group (hardwareMode, (char*)"x87");
	

	// add solve mode selection
	selectNumberOfthreads = glui->add_listbox((char*)"Threads", NULL, 0, SetThreadCount);
	selectNumberOfthreads->add_item (1, (char*)"1");
	selectNumberOfthreads->add_item (2, (char*)"2");
	selectNumberOfthreads->add_item (3, (char*)"3");
	selectNumberOfthreads->add_item (4, (char*)"4");
	selectNumberOfthreads->add_item (8, (char*)"8");


	// add setting options
	glui->add_statictext ((char*)"");
	glui->add_checkbox ((char*)"show fps", &showfps); 
	glui->add_checkbox ((char*)"hide geometry", &hideGeometry); 

	autoSleep = glui->add_checkbox ((char*)"auto sleep off", NULL, 0, ChangeSleepMode); 
	showIslands = glui->add_checkbox ((char*)"show islands", NULL, 0, ChangeShowIslands);  
	showContacts = glui->add_checkbox ((char*)"show contacts", NULL, 0, ChangeShowContacts);  
	showCollision = glui->add_checkbox ((char*)"show collision", NULL, 0, ChangeShowCollision);  
	singleIslandTreading = glui->add_checkbox ((char*)"single island threading off", NULL, 0, ChangeChangeIslandTrading);  


	GLUI_Panel* profilerPanel;
	GLUI_Panel* dynamicsProfilerPanel;
	GLUI_Panel* collisionProfilerPanel;
	
	profilerPanel = glui->add_panel ((char*)"profiler options", GLUI_PANEL_EMBOSSED );
	glui->add_checkbox_to_panel (profilerPanel, (char*)"show profiler", &showProfiler[0]); 

	collisionProfilerPanel = glui->add_panel_to_panel (profilerPanel, (char*)"", GLUI_PANEL_EMBOSSED );
	glui->add_checkbox_to_panel (collisionProfilerPanel, (char*)"show collision  profiler", &showProfiler[1]); 
		panel = glui->add_panel_to_panel (collisionProfilerPanel, (char*)"", GLUI_PANEL_EMBOSSED );
		glui->add_checkbox_to_panel (panel, (char*)"broad phase", &showProfiler[3]); 
		glui->add_checkbox_to_panel (panel, (char*)"narrow phase", &showProfiler[4]); 

	dynamicsProfilerPanel = glui->add_panel_to_panel (profilerPanel, (char*)"", GLUI_PANEL_EMBOSSED );
	glui->add_checkbox_to_panel (dynamicsProfilerPanel, (char*)"show dynamics profiler", &showProfiler[2]); 
		panel = glui->add_panel_to_panel (dynamicsProfilerPanel, (char*)"", GLUI_PANEL_EMBOSSED );
		glui->add_checkbox_to_panel (panel, (char*)"solver", &showProfiler[6]); 
		glui->add_checkbox_to_panel (panel, (char*)"integrate", &showProfiler[5]); 


	// add button for restart the demo
	glui->add_statictext ((char*)"");
	glui->add_button ((char*)"Reset demo", 0, LaunchDemoNewDemo);
	glui->add_button ((char*)"Stop/Run", 0, SetRunStopMode);
	
	
	//launch first demo
	gameSelection->set_int_val (START_DEMO);
	LaunchDemoNewDemo (0);

	// Execute opengl main loop for ever
	MainLoop();   

	return 0;
}



void SceneManager::SetAutoSleep (int mode)
{
	if (m_autoSleep) {
		m_autoSleep (*this, mode);
	}
}

void SceneManager::ApplyControl ()
{
	if (m_control) {
		m_control(*this);
	}
}

void SceneManager::Render ()
{
	dFloat cubeColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, cubeColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cubeColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for (dListNode* node = GetFirst(); node; node = node->GetNext()) {
		OGLModel* model;
		model = node->GetInfo();
		model->Render ();
	}
}

void SceneManager::AddModel (OGLModel* model)
{
	Append (model);
}

void SceneManager::RemoveModel (OGLModel* model)
{
	for(dListNode* node = GetFirst(); node; node = node->GetNext()) {
		if (model == node->GetInfo()) {
			Remove (node);
			break;
		}
	}
	delete model;
}

void SceneManager::CleanUp ()
{
	while (GetFirst()) {
		delete GetFirst()->GetInfo();
		Remove(GetFirst());
	}
}


// memory allocation for Newton
void*  PhysicsAlloc (int sizeInBytes)
{
	memory += sizeInBytes;
	return malloc (sizeInBytes);
}

// memory de-allocation for Newton
void  PhysicsFree (void *ptr, int sizeInBytes)
{
	memory -= sizeInBytes;
	free (ptr);
}


// destroy the world and every thing in it
void CleanUp ()
{
	// destroy all remaining visual objects 
	demo.CleanUp();

	// destroy the Newton world
	if (demo.m_world) {
		NewtonDestroy (demo.m_world);
	}


	memset (&demo, 0, sizeof (demo));
	// check that there are no memory leak on exit
	_ASSERTE (NewtonGetMemoryUsed () == 0);
}

void CreateWorld (int index)
{
	NewtonWorld* world;

	// create the newton world
	world = NewtonCreate (PhysicsAlloc, PhysicsFree);
	demo.m_world = world;

	// set performance counter
	NewtonSetPerformanceClock (world, CHiResTimer::GetTimeInMicrosenconds);

	// ste teh exy callback for GLUT proper temination
	atexit(CleanUp); 

	// load this demo into world
	demosArray[index].m_callback(demo);

	// select instruction set mode
	hardwareMode->set_int_val (1);
//	hardwareMode->set_int_val (0);
	ChangeHardwareMode (0);

	// set the solver mode
//setSolverMode->set_int_val (2);
	ChangeSolverMode (0);

	// set the Max Thread
//selectNumberOfthreads->set_int_val (1);
selectNumberOfthreads->set_int_val (4);
	SetThreadCount (0);

	// set auto sleep mode on
//autoSleep->set_int_val (1);
	ChangeSleepMode (0);

//showCollision->set_int_val(1);
	ChangeShowCollision (0);

//showContacts->set_int_val(1);
	ChangeShowContacts (0);


	ChangeChangeIslandTrading (0);

	// set the adaptive friction model for faster speed 
//	NewtonSetFrictionModel (nWorld, 1);

}


void SetRunStopMode (int id)
{
	simulationStopped = !simulationStopped; 
}


static void SetDebugMode()
{
	int state;
	state = (showCollision->get_int_val() || showContacts->get_int_val() || showIslands->get_int_val()) ? 0 : 1; 
	DebugDisplaySetMode(!state);
}

static void ChangeShowIslands (int id)
{
	SetDebugMode();
	_ASSERTE (demo.m_showIslands);
	demo.m_showIslands (demo, showIslands->get_int_val());
}

void ChangeShowContacts (int id)
{
	SetDebugMode();
	_ASSERTE (demo.m_showContacts);
	demo.m_showContacts(demo, showContacts->get_int_val());
}


void ChangeChangeIslandTrading (int id)
{
	_ASSERTE (demo.m_showContacts);
	NewtonSetMultiThreadSolverOnSingleIsland (demo.m_world, singleIslandTreading->get_int_val() ? 0 : 1);
}

void ChangeShowCollision (int id)
{
	SetDebugMode();
	_ASSERTE (demo.m_setMeshCollision);
	demo.m_setMeshCollision(demo, showCollision->get_int_val());
}


void ChangeSleepMode (int id)
{
	int state;

	state = autoSleep->get_int_val(); 

	demo.SetAutoSleep (state);
}

void ChangeHardwareMode (int id) 
{

	int selected;
	char desc[256];

	selected = hardwareMode->get_int_val();
	switch (selected)
	{
		case 1:
			NewtonSetPlatformArchitecture (demo.m_world, 0);
			break;

		default:
		{
			NewtonSetPlatformArchitecture (demo.m_world, 1);
			if (NewtonGetPlatformArchitecture(demo.m_world, desc) != 1) {
				hardwareMode->set_int_val(0);
			}
			break;
		}
	}

}



void ChangeSolverMode (int id) 
{
	int selected;

	selected = setSolverMode->get_int_val();
	switch (selected)
	{
		case 0:
			NewtonSetSolverModel (demo.m_world, 1);
			break;

		case 1:
			NewtonSetSolverModel (demo.m_world, 2);
			break;

		case 2:
			NewtonSetSolverModel (demo.m_world, 4);
			break;


		default:
			NewtonSetSolverModel (demo.m_world, 0);
			break;
	}

	
}


static void SetThreadCount (int id)
{
	int threads;
	threads = selectNumberOfthreads->get_int_val();

	NewtonSetThreadsCount (demo.m_world, threads);
}


void LaunchDemoNewDemo (int id)
{
	int entry;
	
	// find what demo was selected and launch it
	entry = gameSelection->get_int_val();
	for (int i = 0; i < demosCount; i ++) {
		if(demosArray[i].m_id == entry) {
			glutSetWindow(main_window);

			if (demo.m_world) {
				CleanUp ();
			}

			CreateWorld (i);
//			glutPostRedisplay();
			glutPostWindowRedisplay (main_window);
			break;
		}
	}
}




void RegisterDemo (const char* lable, SceneManager::Create create)
{
	gameSelection->add_item (demosCount, (char*)lable);
	demosArray[demosCount].m_id = demosCount;
	demosArray[demosCount].m_callback = create;
	demosCount ++;
}

// DrawScene()
void DrawScene ()
{
	dFloat timeStep;

	// read the keyboard
	if (dGetKeyState (KeyCode_ESCAPE)) {
		exit(0);
	}

	// get the time step
	timeStep = timer.GetElapsedSeconds();

timeStep = 1.0f/60.0f;

	// move the camera and get keyboard controls
//	demo.ApplyControl ();

	// update the Newton physics world
	if (!re_entrantUpdate) {
		re_entrantUpdate = 1;
		if (!simulationStopped) {
			NewtonUpdate (demo.m_world, timeStep);
//			NewtonCollisionUpdate (demo.m_world);
		}
		re_entrantUpdate = 0;
	}


	// read the control 	
	demo.ApplyControl ();

	// render the scene
	glEnable (GL_LIGHTING);
	glPushMatrix();	
	if (!hideGeometry) {
		demo.Render ();
	}
	glPopMatrix();

	if (showCollision->get_int_val()) {
		DebugRenderWorldCollision (demo.m_world);
	}

	DebugRenderDebugInfo ();

	if (!simulationStopped) {
//	if (showProfiler && !simulationStopped) {
		int profileFlags;
		profileFlags = 0;
		for (int i = 0; i < int (sizeof (showProfiler) / sizeof (showProfiler[0])); i ++) {
			profileFlags |=  showProfiler[i] ? (1 << i) : 0;
		}

//profileFlags |= 1<<3;
//profileFlags |= 1<<4;
//profileFlags |= 1<<6;
		if (profileFlags) {
			int threads = NewtonGetThreadsCount(demo.m_world);
			int maxThread = sizeof (profilers) / sizeof (profilers[0]);
			threads = threads >= maxThread ? maxThread : threads;
			profilers[0]->DrawLabel (profilers[0]->m_oringin_x - 40.0f, profilers[0]->m_oringin_y + 140.0f, "time in milliseconds");
			for (int i = 0; i < threads; i ++){
				profilers[i]->Render (demo.m_world, profileFlags);
			}
		}
	
	}

	if (showfps) {
		dVector color (1.0, 1.0f, 1.0f);
		Print (color, 4,  4, "FPS (%f)    KiloBytes used (%d)", 1.0f / timeStep, NewtonGetMemoryUsed() / 1024);
	}


//	Print (color, 4, 24, "f3 - Toggle multi threading, treadsCount %d", NewtonGetThreadsCount(nWorld));
//	Print (color, 4, 44, "f4 - Toggle Physics Profile");
//	char hardwareDecription[32];
//	NewtonGetPlatformArchitecture (nWorld, hardwareDecription);
//	Print (color, 4, 64, "f6 - Instruction set mode: %s", hardwareDecription);

} 
