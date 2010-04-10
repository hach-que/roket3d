INCLUDES = -I$(top_srcdir)/src
METASOURCES = AUTO
bin_PROGRAMS = Roket3D
noinst_HEADERS = CScreenQuad.h CShaderPre.h core.h effectWrapper.h \
	effectWrapperCB.h eventreciever.h gui.h irrlazycamera.h luna.h physics.h rcursorcontrol.h \
	rdevice.h rguibutton.h rguicheckbox.h rguicombobox.h rguieditbox.h rguielement.h \
	rguienvironment.h rguifileopendialog.h rguifont.h rguiimage.h rguilistbox.h \
	rguimeshviewer.h rguiscrollbar.h rguiskin.h rguistatictext.h rguiwindow.h robject.h \
	roket3d.h roket_physicsbody.h roket_physicsmanager.h \
	roket_physicsmaterialhandler.h rphysicsmanager.h rphysicsmaterialhandler.h rray3d.h rscenecamera.h \
	rscenecollisionmanager.h rscenelazycamera.h rscenemanager.h rscenemesh.h rscenemeshnode.h \
	rscenenode.h rsceneterrainnode.h rscenetexture.h rtriangle3d.h rvector3d.h \
	rvideodriver.h scene.h video.h
Roket3D_SOURCES = CShaderPre.cpp audio.cpp core.cpp effectWrapper.cpp \
	eventreciever.cpp ext.cpp gui.cpp irrlazycamera.cpp main.cpp net.cpp physics.cpp \
	raudiocontroller.cpp raudiosound.cpp rcolor.cpp rcube3d.cpp rcursorcontrol.cpp rdevice.cpp \
	rguibutton.cpp rguicheckbox.cpp rguicombobox.cpp rguieditbox.cpp rguielement.cpp \
	rguienvironment.cpp rguifileopendialog.cpp rguifont.cpp rguiimage.cpp rguilistbox.cpp \
	rguimeshviewer.cpp rguiscrollbar.cpp rguiskin.cpp rguistatictext.cpp rguiwindow.cpp \
	rmatrix.cpp rnetsocket.cpp roket3d.cpp roket_physicsbody.cpp \
	roket_physicscallbacks.cpp roket_physicsjointball.cpp roket_physicsjointcorkscrew.cpp \
	roket_physicsjointcustom.cpp roket_physicsjointhinge.cpp roket_physicsjointslider.cpp \
	roket_physicsjointuniversal.cpp roket_physicsmanager.cpp roket_physicsmaterialhandler.cpp \
	rparticlesystemnode.cpp rphysicsjoint.cpp rphysicsmanager.cpp rphysicsmaterialhandler.cpp \
	rplane.cpp rquaternion.cpp rray2d.cpp rray3d.cpp rrectangle.cpp \
	rscenebillboardnode.cpp rscenebillboardtextnode.cpp rscenecamera.cpp rscenecollisionmanager.cpp \
	rscenelazycamera.cpp rscenelightnode.cpp rscenemanager.cpp rscenemesh.cpp rscenemeshnode.cpp \
	rscenenode.cpp rscenespecialeffects.cpp rsceneterrainnode.cpp rscenetexture.cpp \
	rthread.cpp rtriangle3d.cpp rvector2d.cpp rvector3d.cpp rvideodriver.cpp \
	rvideomaterial.cpp scene.cpp threadenc.cpp video.cpp
Roket3D_LDADD = -lGL -lGLU -lSDL -lSDL_net -lX11 -lXext \
	/home/jrhodes/Documents/Projects/Roket3D/RoketGraphics/lib/Linux/libRoketGraphics.a \
	/home/jrhodes/Documents/Projects/Roket3D/Libraries/RoketScript/src/liblua.a /windows/C/Roket3D/Newton/linux32/sdk/libNewton.a
noinst_DATA = main.lua ring_functions.lua
