//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// 
//********************************************************************


// OpenGlUtil.cpp: implementation of the OpenGlUtil class.
//
//////////////////////////////////////////////////////////////////////
#include <toolbox_stdafx.h>
#include "OpenGlUtil.h"
#include "TargaToOpenGl.h"
#include "ShaderPrograms.h"


int keyBoardTraceMode = 1;
#ifdef RECORD_LOG
	FILE * file;
#else 
	#ifdef READ_LOG
		FILE * file;
	#endif
#endif


#ifdef _MSC_VER
#if (_MSC_VER < 1300) 
		//VC7 or later, building with pre-VC7 runtime libraries
		//defined by VC6 C libs
		extern "C" long _ftol (double); 
		extern "C" long _ftol2( double dblSource ) 
		{ 
			return long( dblSource ); 
		}
#endif

#if (_MSC_VER < 1400) 
		extern "C" long _ftol2_sse() 
		{ 
			long val;
			_asm fistp qword ptr val;
			return val; 
		}
#endif
#endif

static int main_windowID;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static GLuint gAerialFontImage;
static dFloat gAerialFontUV[128][4] = 
{
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.019531f, 0.074219f},
	{0.023438f, 0.000000f, 0.046875f, 0.074219f},
	{0.050781f, 0.000000f, 0.082031f, 0.074219f},
	{0.085938f, 0.000000f, 0.121094f, 0.074219f},
	{0.125000f, 0.000000f, 0.160156f, 0.074219f},
	{0.164063f, 0.000000f, 0.226563f, 0.074219f},
	{0.230469f, 0.000000f, 0.277344f, 0.074219f},
	{0.281250f, 0.000000f, 0.296875f, 0.074219f},
	{0.300781f, 0.000000f, 0.324219f, 0.074219f},
	{0.328125f, 0.000000f, 0.351563f, 0.074219f},
	{0.355469f, 0.000000f, 0.382813f, 0.074219f},
	{0.386719f, 0.000000f, 0.425781f, 0.074219f},
	{0.429688f, 0.000000f, 0.449219f, 0.074219f},
	{0.453125f, 0.000000f, 0.476563f, 0.074219f},
	{0.480469f, 0.000000f, 0.500000f, 0.074219f},
	{0.503906f, 0.000000f, 0.523438f, 0.074219f},
	{0.527344f, 0.000000f, 0.562500f, 0.074219f},
	{0.566406f, 0.000000f, 0.601563f, 0.074219f},
	{0.605469f, 0.000000f, 0.640625f, 0.074219f},
	{0.644531f, 0.000000f, 0.679688f, 0.074219f},
	{0.683594f, 0.000000f, 0.718750f, 0.074219f},
	{0.722656f, 0.000000f, 0.757813f, 0.074219f},
	{0.761719f, 0.000000f, 0.796875f, 0.074219f},
	{0.800781f, 0.000000f, 0.835938f, 0.074219f},
	{0.839844f, 0.000000f, 0.875000f, 0.074219f},
	{0.878906f, 0.000000f, 0.914063f, 0.074219f},
	{0.917969f, 0.000000f, 0.941406f, 0.074219f},
	{0.945313f, 0.000000f, 0.968750f, 0.074219f},
	{0.000000f, 0.078125f, 0.039063f, 0.152344f},
	{0.042969f, 0.078125f, 0.082031f, 0.152344f},
	{0.085938f, 0.078125f, 0.125000f, 0.152344f},
	{0.128906f, 0.078125f, 0.167969f, 0.152344f},
	{0.171875f, 0.078125f, 0.238281f, 0.152344f},
	{0.242188f, 0.078125f, 0.285156f, 0.152344f},
	{0.289063f, 0.078125f, 0.335938f, 0.152344f},
	{0.339844f, 0.078125f, 0.386719f, 0.152344f},
	{0.390625f, 0.078125f, 0.437500f, 0.152344f},
	{0.441406f, 0.078125f, 0.484375f, 0.152344f},
	{0.488281f, 0.078125f, 0.527344f, 0.152344f},
	{0.531250f, 0.078125f, 0.582031f, 0.152344f},
	{0.585938f, 0.078125f, 0.632813f, 0.152344f},
	{0.636719f, 0.078125f, 0.652344f, 0.152344f},
	{0.656250f, 0.078125f, 0.691406f, 0.152344f},
	{0.695313f, 0.078125f, 0.742188f, 0.152344f},
	{0.746094f, 0.078125f, 0.785156f, 0.152344f},
	{0.789063f, 0.078125f, 0.839844f, 0.152344f},
	{0.843750f, 0.078125f, 0.890625f, 0.152344f},
	{0.894531f, 0.078125f, 0.945313f, 0.152344f},
	{0.949219f, 0.078125f, 0.992188f, 0.152344f},
	{0.000000f, 0.156250f, 0.050781f, 0.230469f},
	{0.054688f, 0.156250f, 0.101563f, 0.230469f},
	{0.105469f, 0.156250f, 0.148438f, 0.230469f},
	{0.152344f, 0.156250f, 0.191406f, 0.230469f},
	{0.195313f, 0.156250f, 0.242188f, 0.230469f},
	{0.246094f, 0.156250f, 0.289063f, 0.230469f},
	{0.292969f, 0.156250f, 0.359375f, 0.230469f},
	{0.363281f, 0.156250f, 0.406250f, 0.230469f},
	{0.410156f, 0.156250f, 0.449219f, 0.230469f},
	{0.453125f, 0.156250f, 0.488281f, 0.230469f},
	{0.492188f, 0.156250f, 0.515625f, 0.230469f},
	{0.519531f, 0.156250f, 0.539063f, 0.230469f},
	{0.542969f, 0.156250f, 0.566406f, 0.230469f},
	{0.570313f, 0.156250f, 0.609375f, 0.230469f},
	{0.613281f, 0.156250f, 0.648438f, 0.230469f},
	{0.652344f, 0.156250f, 0.675781f, 0.230469f},
	{0.679688f, 0.156250f, 0.714844f, 0.230469f},
	{0.718750f, 0.156250f, 0.757813f, 0.230469f},
	{0.761719f, 0.156250f, 0.796875f, 0.230469f},
	{0.800781f, 0.156250f, 0.839844f, 0.230469f},
	{0.843750f, 0.156250f, 0.878906f, 0.230469f},
	{0.882813f, 0.156250f, 0.902344f, 0.230469f},
	{0.906250f, 0.156250f, 0.945313f, 0.230469f},
	{0.949219f, 0.156250f, 0.988281f, 0.230469f},
	{0.000000f, 0.234375f, 0.015625f, 0.308594f},
	{0.019531f, 0.234375f, 0.035156f, 0.308594f},
	{0.039063f, 0.234375f, 0.074219f, 0.308594f},
	{0.078125f, 0.234375f, 0.093750f, 0.308594f},
	{0.097656f, 0.234375f, 0.152344f, 0.308594f},
	{0.156250f, 0.234375f, 0.195313f, 0.308594f},
	{0.199219f, 0.234375f, 0.238281f, 0.308594f},
	{0.242188f, 0.234375f, 0.281250f, 0.308594f},
	{0.285156f, 0.234375f, 0.324219f, 0.308594f},
	{0.328125f, 0.234375f, 0.355469f, 0.308594f},
	{0.359375f, 0.234375f, 0.394531f, 0.308594f},
	{0.398438f, 0.234375f, 0.421875f, 0.308594f},
	{0.425781f, 0.234375f, 0.464844f, 0.308594f},
	{0.468750f, 0.234375f, 0.503906f, 0.308594f},
	{0.507813f, 0.234375f, 0.558594f, 0.308594f},
	{0.562500f, 0.234375f, 0.597656f, 0.308594f},
	{0.601563f, 0.234375f, 0.636719f, 0.308594f},
	{0.640625f, 0.234375f, 0.675781f, 0.308594f},
	{0.679688f, 0.234375f, 0.707031f, 0.308594f},
	{0.710938f, 0.234375f, 0.726563f, 0.308594f},
	{0.730469f, 0.234375f, 0.757813f, 0.308594f},
	{0.761719f, 0.234375f, 0.800781f, 0.308594f}
};											   

static dMOUSE_POINT mouse0;
//static int mouseBottonDown = 0;
static char keybuffer[256];
static RenderLoop renderCallback;

static void setup_opengl( int width, int height )
{
	// Our shading model--Gouraud (smooth). 
	glShadeModel (GL_SMOOTH);

	// Culling. 
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );

	// z buffer test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);


	// Set the clear color. 
	glClearColor(0.5, 0.5, 0.5, 0.75);

	// Setup our viewport. 
//	glViewport (0, 0, width, height);


	// Change to the projection matrix and set our viewing volume.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
}


static void MouseMotion (int x, int y)
{
	mouse0.x = x;
	mouse0.y = y;
//	glutPostRedisplay(); 
	glutPostWindowRedisplay (main_windowID);
}

static void CaptureMousePos (int button, int state, int x, int y)
{
	switch (button) 
	{
		case GLUT_LEFT_BUTTON:
		{
			keybuffer[KeyCode_L_BUTTON] = (state == GLUT_DOWN) ? 1 : 0;
			break;
		}

		case GLUT_RIGHT_BUTTON:
		{
			keybuffer[KeyCode_R_BUTTON] = (state == GLUT_DOWN) ? 1 : 0;
			break;
		}
	}
}


static void Animate()
{
	glutPostWindowRedisplay (main_windowID);
} 


static void Reshape(int width__, int height__)
{
	int tx, ty, tw, th;

	
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	dFloat aspectRatio = dFloat (4.0f / 3.0f);
	
	if (tw < (th * aspectRatio)) {
		tw = int (dFloat (th) * aspectRatio) ;
	} else {
		th = int (dFloat (tw) / aspectRatio) ;
	}
	glViewport( tx, ty, tw, th );

	dFloat nearPlane = 0.1f;
	dFloat farPlane = 1024.0f;
	dFloat left = -nearPlane * aspectRatio; 
	dFloat right = nearPlane * aspectRatio; 
	dFloat bottom = -nearPlane; 
	dFloat top = nearPlane;
	glFrustum (left, right, bottom, top, nearPlane, farPlane);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	gluPerspective(90.0, width/height, 0.2, 5000.0);
	gluPerspective(60.0, aspectRatio, nearPlane, farPlane);

	glMatrixMode(GL_MODELVIEW);

//	glutPostRedisplay();
	glutPostWindowRedisplay (main_windowID);
} 

static void NormalKeboard(unsigned char key, int x, int y) 
{
	keybuffer[key] = 1;
}

static void ReleaseNormalKeboard(unsigned char key, int x, int y) 
{
	keybuffer[key] = 0;
}

static int specialKeyOn = 0;
static void SpecialKeboard(int key, int x, int y) 
{
	specialKeyOn = 1;
	keybuffer[key] = 1;
}

static void Display()
{
	// clear the screen
	glMatrixMode (GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderCallback ();

	// draw everything and swap the display buffer
	glFlush();
	glutSwapBuffers();

	if (specialKeyOn) {
		specialKeyOn = 0;
		for (int i = 0; i < KeyCode_lastSpecialKey; i ++) {
			keybuffer[i] = 0;
		}
	}
} 

void MainLoop()
{
	glutMainLoop();  
}






int InitOpenGl(int argc, char **argv, const char *title, RenderLoop renderFnt, int width, int height)
{
	int windowID;

	// ini gl utility libarry
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create the window
	glutInitWindowSize(width, height);

//	glutInitWindowPosition( 50, 50 );
	windowID = glutCreateWindow(title);
	main_windowID = windowID;

//char *s;
//s = (char*)glGetString(GL_VENDOR);

	// ini open gl extenstions
	glewInit();
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_vertex_buffer_object) {  
//		ShaderPrograms::GetCache().CreateAllEffects();
	}


	// uncomment this to run in full screen (very slow)
	//glutFullScreen();

	// set the background color
	glClearColor(0.5, 0.5, 0.5, 0.75);

	// set the shading model
	glShadeModel(GL_SMOOTH);

	setup_opengl (width, height);

	// set on ambient directional light
	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 0.0 };
	GLfloat lightAmbientColor[] = { 0.7f, 0.7f, 0.7f, 0.0 };
	GLfloat lightPosition[] = { 500.0f, 200.0f, 500.0f, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// load the font texture
	gAerialFontImage = LoadTexture ("Arial.tga");

	// save the render function
	renderCallback = renderFnt;

	glutIdleFunc(Animate);
	glutDisplayFunc(Display); 
	glutReshapeFunc(Reshape);

	glutMouseFunc(CaptureMousePos);
	glutMotionFunc(MouseMotion); 
	glutPassiveMotionFunc(MouseMotion); 
	
	glutKeyboardFunc(NormalKeboard);
	glutSpecialFunc(SpecialKeboard);	
	glutKeyboardUpFunc(ReleaseNormalKeboard);


#ifdef RECORD_LOG
	file = fopen ("log.log", "wb");
#endif

#ifdef READ_LOG
	file = fopen ("log.log", "rb");
#endif

	// clue glut to the Gui library 
	GLUI_Master.set_glutReshapeFunc (Reshape);  
	GLUI_Master.set_glutKeyboardFunc (NormalKeboard);
	GLUI_Master.set_glutSpecialFunc(SpecialKeboard);
	GLUI_Master.set_glutMouseFunc (CaptureMousePos);

	return windowID;
}


void GetCursorPos(dMOUSE_POINT& point)
{
	point = mouse0;

	if (keyBoardTraceMode){
#ifdef RECORD_LOG
		fwrite (& point, sizeof (dMOUSE_POINT), 1, file);
		fflush (file);
#endif

#ifdef READ_LOG
		fread (& point, sizeof (dMOUSE_POINT), 1, file);
#endif
	}
}

int dGetKeyState(int key)
{
	int code;
	code = keybuffer[key];

	if (keyBoardTraceMode){
#ifdef RECORD_LOG
		fwrite (&code, sizeof (int), 1, file);
		fflush (file);
#endif

#ifdef READ_LOG
		fread (&code, sizeof (int), 1, file);
#endif
	}
	return code;
}


dMatrix GetModelViewMatrix ()
{
	int i;
	int j;

	GLdouble modelview[16]; 
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
	dMatrix camMatrix (GetIdentityMatrix());
	for (i = 0; i < 4; i ++) {
		for (j = 0; j < 4; j ++) {
			camMatrix[i][j] = dFloat (modelview[i * 4 + j]);
		}
	}
	return camMatrix;
}


void SetCamera (const dVector& eye, const dVector& target)
{
	// set up the view orientation looking at the origin from slightly above
	// and to the left
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.m_x, eye.m_y, eye.m_z, target.m_x, target.m_y, target.m_z, 0.0, 1.0, 0.0);

/*
	dMatrix matrix (GetIdentityMatrix());
	matrix.m_right = eye - target;
	matrix.m_right.m_w = 0.0f;
	matrix.m_right = matrix.m_right.Scale (1.0f / dSqrt(matrix.m_right % matrix.m_right));
	matrix.m_front = dVector (0.0f, 1.0f, 0.0f, 0.0f) * matrix.m_right;
	matrix.m_front = matrix.m_front.Scale (1.0f / dSqrt(matrix.m_front % matrix.m_front));
	matrix.m_up = matrix.m_right * matrix.m_front;
	matrix.m_posit = eye;
	matrix = matrix.Inverse();
//	SetCamera (matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrix(&matrix[0][0]);
*/
}

unsigned dRand ()
{
	#define RAND_MUL 31415821u
	static unsigned randSeed = 0;
 	randSeed = RAND_MUL * randSeed + 1; 
	return randSeed & dRAND_MAX;
}



void Print (const dVector& color, dFloat x, dFloat y, const char *fmt, ... )
{
	// enter 2d display mode 
	int len;
	int witdh;
	int height;
	dFloat sx;
	dFloat sy;
	va_list argptr;
	char string[1024];


	// Note, there may be other things you need to change,
	//depending on how you have your OpenGL state set up.
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable (GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glColor3f(color.m_x, color.m_y, color.m_z);
	// This allows alpha blending of 2D textures with the scene 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	GLint viewport[4]; 
	//Retrieves the viewport and stores it in the variable
	glGetIntegerv(GL_VIEWPORT, viewport); 
	witdh = viewport[2];
	height = viewport[3];

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)witdh, height, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	GLuint shaderProgram;
	shaderProgram = ShaderPrograms::GetCache().m_decalEffect;
	if (shaderProgram) {
		glUseProgram(shaderProgram);
	}



	// draw the text on the screen
	glBindTexture(GL_TEXTURE_2D, gAerialFontImage);
	
	va_start (argptr, fmt);
	vsprintf (string, fmt, argptr);
	va_end( argptr );
	len = (int) strlen (string );

	glBegin(GL_QUADS);

	sx = x + 0.5f;
	sy = y + 0.5f;
	for (int i = 0; i < len; i++) {
		dFloat w;
		dFloat h;
		dFloat tx1;
		dFloat ty1;
		dFloat tx2;
		dFloat ty2;
		unsigned char c;

		c = string[i];
		tx1 = gAerialFontUV[c][0];
		ty1 = gAerialFontUV[c][1];
		tx2 = gAerialFontUV[c][2];
		ty2 = gAerialFontUV[c][3];
		w = (tx2-tx1) * 256.0f;
		h = (ty2-ty1) * 256.0f;

		glTexCoord2f (tx1, 1.0f-ty1); glVertex2f(sx + 0, sy + 0);
		glTexCoord2f (tx2, 1.0f-ty1); glVertex2f(sx + w, sy + 0);
		glTexCoord2f (tx2, 1.0f-ty2); glVertex2f(sx + w, sy + h);
		glTexCoord2f (tx1, 1.0f-ty2); glVertex2f(sx + 0, sy + h);
		sx += w;
	}
	glEnd();

	// exit 2d display mode
	glDisable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();

//	setup_opengl (screen->w, screen->h);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


dVector WorldToScreen (const dVector& world)
{
	int win[4]; 
	GLint viewport[4]; 

	//Retrieves the viewport and stores it in the variable
	// get a point on the display arai of teh windows
	GLUI_Master.get_viewport_area(&win[0], &win[1], &win[2], &win[3]);
	viewport[0] = GLint (win[0]);
	viewport[1] = GLint (win[1]);
	viewport[2] = GLint (win[2]);
	viewport[3] = GLint (win[3]);


	//Where the 16 doubles of the matrix are to be stored
	GLdouble modelview[16]; 

	//Retrieve the matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 

	GLdouble projection[16]; 
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLdouble winX;
	GLdouble winY;
	GLdouble winZ; //The coordinates to pass in

	//Now windows coordinates start with (0,0) being at the top left 
	//whereas OpenGL cords start lower left so we have to do this to convert it: 
	//Remember we got viewport value before 
	GLdouble objx = world.m_x;
	GLdouble objy = world.m_y;
	GLdouble objz = world.m_z;

	// use the real GL view port
	glGetIntegerv(GL_VIEWPORT, viewport); 
	gluProject (objx, objy, objz, modelview, projection, viewport, &winX, &winY, &winZ);

	winY = (dFloat)viewport[3] - winY; 

	return dVector (dFloat(winX), dFloat (winY), dFloat(winZ), 0.0f);
}



dVector ScreenToWorld (const dVector& screen)
{
	//Where the values will be stored
	int win[4]; 
	GLint viewport[4]; 

	//Retrieves the viewport and stores it in the variable
	// get a point on the display arai of teh windows
	GLUI_Master.get_viewport_area(&win[0], &win[1], &win[2], &win[3]);
	viewport[0] = GLint (win[0]);
	viewport[1] = GLint (win[1]);
	viewport[2] = GLint (win[2]);
	viewport[3] = GLint (win[3]);


	//Where the 16 doubles of the matrix are to be stored
	GLdouble modelview[16]; 

	//Retrieve the matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 

	GLdouble projection[16]; 
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLdouble winX = 0.0;
	GLdouble winY = 0.0;
	GLdouble winZ = 0.0; //The coordinates to pass in

	winX = screen.m_x; //Store the x cord
	winY = screen.m_y; //Store the y cord
	winZ = screen.m_z; //Store the Z cord

	//Now windows coordinates start with (0,0) being at the top left 
	//whereas OpenGL cords start lower left so we have to do this to convert it: 
	//Remember we got viewport value before 
	winY = (dFloat)viewport[3] - winY; 

	GLdouble objx;
	GLdouble objy;
	GLdouble objz;

	// use the real GL view port
	glGetIntegerv(GL_VIEWPORT, viewport); 
	gluUnProject (winX, winY, winZ, modelview, projection, viewport, &objx, &objy, &objz);

	return dVector (dFloat(objx), dFloat(objy), dFloat(objz));
}

void ShowMousePicking (const dVector& p0, const dVector& p1, dFloat radius)
{
	GLuint shaderProgram;
	shaderProgram = ShaderPrograms::GetCache().m_solidColor;
	if (shaderProgram) {
		glUseProgram(shaderProgram);
	}
	
	// set up the cube's texture
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glVertex3f(p0.m_x, p0.m_y, p0.m_z);
	glVertex3f(p1.m_x, p1.m_y, p1.m_z);
	glEnd();
	glLineWidth(1.0f);

	// draw the bone points
	glColor3f(1.0f, 1.0f, 0.0f);
	glPointSize(6.0f);
	glBegin(GL_POINTS);
	glVertex3f(p0.m_x, p0.m_y, p0.m_z);
	glVertex3f(p1.m_x, p1.m_y, p1.m_z);
	glEnd();
	glPointSize(1.0f);

	glEnable (GL_LIGHTING);

//	glPopMatrix();
}



