// AnimationTool.cpp : Defines the entry point for the console application.
//

#include "toolbox_stdafx.h"
#include <windows.h>
#include <Commdlg.h>
#include "OpenGlUtil.h"
#include "OGLModel.h"
#include "FloorPlane.h"
#include "VisualModel.h"
#include "dAnimationClip.h"
#include "dPoseGenerator.h"
#include "dBonesToPoseBinding.h"

#define CAMERA_SPEED	4.0f
#define FILE_NAME_SIZE	256

dFloat zoomDist = 5.0f;
dFloat rollAngle = -30 * 3.1416f / 180.0f;
dFloat yawAngle = 0.0f;
dVector cameraDir (1.0f, 0.0f, 0.0f, 0.0f);
dVector cameraEyepoint (-40.0f, 10.0f, 0.0f, 0.0f);

//dVector screenSpace; 
dFloat verticalPan = 0.0f;
dFloat horizontalPan = 0.0f;

VisualModel* model;
FloorPlane* floorPlane;


static GLUI *glui;
static int main_window;

GLUI_StaticText *modelName;
GLUI_Listbox* animationList;
GLUI_RadioGroup* cameraMode;

static void DrawScene ();
static void CleanUp ();

static void Keyboard();
static void LoadModel (int id);
static void SaveModel (int id);
static void AddAnimation (int id);
static void RemoveAnimation (int id);
static void AnimationListControl (int id);
static void CameraModeChange (int id);

static char* GetFileName (char *pathName) 
{
	char* name;
	name = strrchr (pathName, '/');
	if (!name) {
		name = strrchr (pathName, '\\');
	}
	return name + 1;
}


int main(int argc, char* argv[])
{

	// initialize opengl	
	main_window = InitOpenGl (argc, argv, "Newton Animation Tool", DrawScene);

	atexit(CleanUp); 


	// create the Gui window main frame 
	glui = GLUI_Master.create_glui_subwindow (main_window, GLUI_SUBWINDOW_RIGHT );
	glui->add_statictext ((char*)"Command Panel");
	modelName = glui->add_statictext ("");

	modelName = glui->add_statictext ((char*)" Name: ");
	glui->add_button ((char*)"Load Model", 0, LoadModel);
	glui->add_button ((char*)"Save Model", 0, SaveModel);
	modelName = glui->add_statictext ("");

	animationList = glui->add_listbox (" Anim", NULL, 0, AnimationListControl);
	glui->add_button ((char*)"Add Animation", 0, AddAnimation);
	glui->add_button ((char*)"Del Animation", 0, RemoveAnimation);

	GLUI_Panel* panel;
	panel = glui->add_panel ((char*)"Camera mode", GLUI_PANEL_EMBOSSED );
	cameraMode = glui->add_radiogroup_to_panel (panel, NULL, 0, CameraModeChange);
	glui->add_radiobutton_to_group (cameraMode, (char*)"Orbit");
	glui->add_radiobutton_to_group (cameraMode, (char*)"flight by");



//	glui->add_button ((char*)"Stop/Run", 0, SetRunStopMode);


/*
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
*/


	CameraModeChange (0);
	floorPlane = new FloorPlane;  

	// Execute opengl main loop for ever
	MainLoop();   

	return 0;
}


static void CleanUp ()
{
	delete floorPlane;
	if (model) {
		delete model;
	}
}






// DrawScene()
void DrawScene ()
{
	dFloat timeStep;
	// get the time step
//	timeStep = timer.GetElapsedSeconds();

timeStep = 1.0f/60.0f;

	// move the camera and get keyboard controls
//	demo.ApplyControl ();

	// update the Newton physics world

	// read the keyboard
	if (dGetKeyState (KeyCode_ESCAPE)) {
		exit(0);
	}

	// read the control 	
	Keyboard();

	// render the scene
	glEnable (GL_LIGHTING);
	glPushMatrix();	

	floorPlane->Render ();

static float xxx = 0.0;
	if (model) {

model->SetFrame (xxx);		
xxx += 0.01f;

		model->Render ();
	}

	glPopMatrix();

/*
	if (showCollision->get_int_val()) {
		DebugRenderWorldCollision (demo.m_world);
	}

	DebugRenderDebugInfo ();

	if (!simulationStopped) {
		int profileFlags;
		profileFlags = 0;
		for (int i = 0; i < int (sizeof (showProfiler) / sizeof (showProfiler[0])); i ++) {
			profileFlags |=  showProfiler[i] ? (1 << i) : 0;
		}
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
*/
} 


void LoadModel (int id)
{
   OPENFILENAME ofn;
    memset (&ofn, 0, sizeof (OPENFILENAME));

	
    
    char szFile[FILE_NAME_SIZE];
    char szFileTitle[FILE_NAME_SIZE];
    char szDefExt[32] = "mdl";
    char szFilter[128] = "Newton Files (*.mdl)\0*.mdl\0\0";
    szFile[0] = '\0';
    szFileTitle[0] = '\0';

    ofn.lStructSize = sizeof (OPENFILENAME);
    ofn.lpstrDefExt = szDefExt;
    ofn.lpstrFilter = szFilter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = FILE_NAME_SIZE;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = FILE_NAME_SIZE;
    ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrTitle = "Newton Load Model";

	if (GetOpenFileName (&ofn)) {
		if (model) {
			delete model;
		}
		char name[256];
		for (int i = 0; animationList->delete_item (i); i ++); 
		animationList->curr_text = "";

		sprintf (name, " Name: %s", GetFileName (szFile));
		modelName->set_text (name);		
		model = new VisualModel (szFile);

		if (model->m_animations.GetCount()) {
			for (ModelComponentList<dAnimationClip*>::dListNode* node = model->m_animations.GetFirst(); node; node = node->GetNext()) {
				ModelComponent<dAnimationClip*>& animation = node->GetInfo();

				animationList->add_item (model->m_animationsMenu.GetCount(), animation.m_name);
				model->m_animationsMenu.Insert (animation.m_data, model->m_animationsMenu.GetCount());
			}
			animationList->set_int_val(0);
			AnimationListControl (0);
		}
	}
}


void SaveModel (int id)
{
	if (model) {
		OPENFILENAME ofn;
		memset (&ofn, 0, sizeof (OPENFILENAME));
	    
		char szFile[FILE_NAME_SIZE ];
		char szFileTitle[FILE_NAME_SIZE];
		char szDefExt[32] = "mdl";
		char szFilter[128] = "Newton Files (*.mdl)\0*.mdl\0\0";
		szFile[0] = '\0';
		szFileTitle[0] = '\0';

		ofn.lStructSize = sizeof (OPENFILENAME);
		ofn.lpstrDefExt = szDefExt;
		ofn.lpstrFilter = szFilter;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = FILE_NAME_SIZE;
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = FILE_NAME_SIZE;
		ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
		ofn.lpstrTitle = "Newton Save Model";

		if (GetSaveFileName (&ofn)) {
			model->Save (szFile, false, false);
		}
	}
}

void AddAnimation (int id)
{
	if (model) {
	   OPENFILENAME ofn;
		memset (&ofn, 0, sizeof (OPENFILENAME));
	    
		char szFile[FILE_NAME_SIZE ];
		char szFileTitle[FILE_NAME_SIZE];
		char szDefExt[32] = "ani";
		char szFilter[128] = "Newton Files (*.ani)\0*.ani\0\0";
		szFile[0] = '\0';
		szFileTitle[0] = '\0';

		ofn.lStructSize = sizeof (OPENFILENAME);
		ofn.lpstrDefExt = szDefExt;
		ofn.lpstrFilter = szFilter;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = FILE_NAME_SIZE;
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = FILE_NAME_SIZE;
		ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
		ofn.lpstrTitle = "Newton Animation Loader";

		if (GetOpenFileName (&ofn)) {
			dAnimationClip* anim;
			anim = new dAnimationClip ();
			anim->Load (szFile);

			char name[256];
			dModel::GetFileName (szFile, name);
			animationList->add_item (model->m_animationsMenu.GetCount(), name);
			animationList->set_int_val(model->m_animationsMenu.GetCount());
			model->m_animationsMenu.Insert (anim, model->m_animationsMenu.GetCount());
			model->AddAnimation (name, anim);
			anim->Release();
		}
	}
}


void RemoveAnimation (int id)
{
	if (model) {
		int entry;
		entry = animationList->get_int_val();
		model->DeleteAnimation(entry);

		animationList->delete_item (entry);
		animationList->curr_text = "";
	}
}



void AnimationListControl (int id)
{
	int entry;
	
	// find what demo was selected and launch it
	entry = animationList->get_int_val();
	_ASSERTE (model);

	model->SelectAnimation (entry);	

	if (model->m_player) {
		int frameCount;
		frameCount = model->m_player->GetPoseGenerator()->GetAnimationClip()->GetFramesCount();
	}
}


void CameraModeChange (int id)
{
	yawAngle = 0.0f;
	rollAngle = -30.0f * 3.1416f / 180.0f;

	if (model) {
		model->FrameCamera ();
	}
}

//	Keyboard handler. 
void Keyboard()
{
	// read the mouse position and set the camera direction
	dMOUSE_POINT mouse1;
	dInt32 mouseLeftKey;
	static dMOUSE_POINT mouse0;

	GetCursorPos(mouse1);
	// this section control the camera object picking
	mouseLeftKey = dGetKeyState (KeyCode_L_BUTTON);
	// we are not in mouse pick mode, then we are in camera tracking mode

	int value = cameraMode->get_int_val ();
	if (value == 1) {
		if (mouseLeftKey) {
			// when click left mouse button the first time, we reset the camera
			// convert the mouse x position to delta yaw angle
			if (mouse1.x > (mouse0.x + 1)) {
				yawAngle += 1.0f * 3.1416f / 180.0f;
				if (yawAngle > (360.0f * 3.1416f / 180.0f)) {
					yawAngle -= (360.0f * 3.1416f / 180.0f);
				}
			} else if (mouse1.x < (mouse0.x - 1)) {
				yawAngle -= 1.0f * 3.1416f / 180.0f;
				if (yawAngle < 0.0f) {
					yawAngle += (360.0f * 3.1416f / 180.0f);
				}
			}

			if (mouse1.y > (mouse0.y + 1)) {
				rollAngle += 1.0f * 3.1416f / 180.0f;
				if (rollAngle > (80.0f * 3.1416f / 180.0f)) {
					rollAngle = 80.0f * 3.1416f / 180.0f;
				}
			} else if (mouse1.y < (mouse0.y - 1)) {
				rollAngle -= 1.0f * 3.1416f / 180.0f;
				if (rollAngle < -(80.0f * 3.1416f / 180.0f)) {
					rollAngle = -80.0f * 3.1416f / 180.0f;
				}
			}
			dMatrix cameraDirMat (dRollMatrix(rollAngle) * dYawMatrix(yawAngle));
			cameraDir = cameraDirMat.m_front;
		}

		// save mouse position and left mouse key state for next frame
		mouse0 = mouse1;

		// camera control
		if (dGetKeyState (KeyCode_w)) {
			cameraEyepoint += cameraDir.Scale (CAMERA_SPEED / 60.0f);
		} else if (dGetKeyState (KeyCode_s)) {
			cameraEyepoint -= cameraDir.Scale (CAMERA_SPEED / 60.0f);
		}

		if (dGetKeyState (KeyCode_d)) {
			dVector up (0.0f, 1.0f, 0.0f);
			dVector right (cameraDir * up);
			cameraEyepoint += right.Scale (CAMERA_SPEED / 60.0f);
		} else if (dGetKeyState (KeyCode_a)) {
			dVector up (0.0f, 1.0f, 0.0f);
			dVector right (cameraDir * up);
			cameraEyepoint -= right.Scale (CAMERA_SPEED / 60.0f);
		}

		dVector target (cameraEyepoint + cameraDir);
		SetCamera (cameraEyepoint, target);

	} else {

		if (mouseLeftKey) {
			// when click left mouse button the first time, we reset the camera
			// convert the mouse x position to delta yaw angle
			if (mouse1.x > (mouse0.x + 1)) {
				yawAngle += 1.0f * 3.1416f / 180.0f;
				if (yawAngle > (360.0f * 3.1416f / 180.0f)) {
					yawAngle -= (360.0f * 3.1416f / 180.0f);
				}
			} else if (mouse1.x < (mouse0.x - 1)) {
				yawAngle -= 1.0f * 3.1416f / 180.0f;
				if (yawAngle < 0.0f) {
					yawAngle += (360.0f * 3.1416f / 180.0f);
				}
			}

			if (mouse1.y > (mouse0.y + 1)) {
				rollAngle -= 1.0f * 3.1416f / 180.0f;
				if (rollAngle < -(80.0f * 3.1416f / 180.0f)) {
					rollAngle = -80.0f * 3.1416f / 180.0f;
				}
			} else if (mouse1.y < (mouse0.y - 1)) {
				rollAngle += 1.0f * 3.1416f / 180.0f;
				if (rollAngle > (80.0f * 3.1416f / 180.0f)) {
					rollAngle = 80.0f * 3.1416f / 180.0f;
				}
			}
		}


		// save mouse position and left mouse key state for next frame
		mouse0 = mouse1;


		if (dGetKeyState (KeyCode_w)) {
			verticalPan += CAMERA_SPEED / 60.0f;
		} else if (dGetKeyState (KeyCode_s)) {
			verticalPan -= CAMERA_SPEED / 60.0f;			
		}

		if (dGetKeyState (KeyCode_d)) {
			horizontalPan += CAMERA_SPEED / 60.0f;
		} else if (dGetKeyState (KeyCode_a)) {
			horizontalPan -= CAMERA_SPEED / 60.0f;
		}

		dMatrix cameraDirMat (dRollMatrix(rollAngle) * dYawMatrix(3.1416f * 0.5f - yawAngle));
		dVector dir (cameraDirMat.m_front);

		dVector origin (cameraDirMat.m_right.Scale (horizontalPan) + cameraDirMat.m_up.Scale (verticalPan));

		dVector eyePoint (origin - cameraDirMat.m_front.Scale (5.0f));
		SetCamera (eyePoint, origin);
	}
} 
