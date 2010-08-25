#*******************************************************
#
# Newton game dynamics 
# copy right by Julio Jerez 2003 - 2005 
#
#*******************************************************
#
# SDK Sample tutorial: 
#
#*******************************************************  


# ******************************************************
#
# project sorce files
#
# ******************************************************


# generic sdk flags
SDK_FLAGS = -O0 -c -Wall -D_LINUX_VER -g 
#SDK_FLAGS = -m32 -O3 -c -Wall -Werror -D_LINUX_VER -g 
#SDK_FLAGS = -m32 -O3 -c -Wall -D_LINUX_VER -g 

LIBS = -L/usr/X11R6/lib -lGL -lGLU -lglut -lglui -lGLEW

# this use newton as shared or static library
#NEWTON_LIB = -lNewton
NEWTON_LIB = $(SDK_NEWTON)/libNewton.a

SDK_MAIN = sdkDemos
SDK_DEMOS = sdkDemos/demos
SDK_TOOLBOX = sdkDemos/toolBox
SDK_XML = thirdParties/tinyxml
SDK_MATH = ../sdk/dMath
SDK_JOINTS = ../sdk/dCustomJoints
SDK_ANIMATION = ../sdk/dAnimation
SDK_CONTAINERS = ../sdk/dContainers
SDK_NEWTON = ../sdk



SDK_INCLUDE_PATH = -I $(SDK_MAIN) -I $(SDK_DEMOS) -I $(SDK_TOOLBOX) -I $(SDK_MATH) -I $(SDK_JOINTS) -I $(SDK_NEWTON) -I $(SDK_XML) -I $(SDK_ANIMATION) -I $(SDK_CONTAINERS)


SDK_MAIN_SRC = \
	$(SDK_MAIN)/OGLMesh.cpp \
	$(SDK_MAIN)/OGLModel.cpp \
	$(SDK_MAIN)/SceneManager.cpp \
	$(SDK_MAIN)/PhysicsUtils.cpp

SDK_DEMOS_SRC = \
	$(SDK_DEMOS)/ArchimedesBuoyancy.cpp \
	$(SDK_DEMOS)/BasicCustomJoints.cpp \
	$(SDK_DEMOS)/BasicFriction.cpp \
	$(SDK_DEMOS)/BasicRestitution.cpp \
	$(SDK_DEMOS)/BasicRobots.cpp \
	$(SDK_DEMOS)/BasicRagDoll.cpp \
	$(SDK_DEMOS)/BasicSphereSpin.cpp \
	$(SDK_DEMOS)/BasicStacking.cpp \
	$(SDK_DEMOS)/CompoundCollision.cpp \
	$(SDK_DEMOS)/ClosestDistance.cpp \
	$(SDK_DEMOS)/ConvexCast.cpp \
	$(SDK_DEMOS)/Jengas.cpp \
	$(SDK_DEMOS)/LegacyJoints.cpp \
	$(SDK_DEMOS)/MeshCollision.cpp \
	$(SDK_DEMOS)/PrecessingTops.cpp \
	$(SDK_DEMOS)/Pyramids.cpp \
	$(SDK_DEMOS)/RayCastCar.cpp \
	$(SDK_DEMOS)/ScaledCollision.cpp \
	$(SDK_DEMOS)/SpecialTrackJoints.cpp \
	$(SDK_DEMOS)/Towers.cpp \
	$(SDK_DEMOS)/TriggersAndForceFields.cpp \
	$(SDK_DEMOS)/UnstableStacks.cpp \
	$(SDK_DEMOS)/Walls.cpp 

SDK_TOOLBOX_SRC = \
	$(SDK_TOOLBOX)/OpenGlUtil.cpp \
	$(SDK_TOOLBOX)/DebugDisplay.cpp \
	$(SDK_TOOLBOX)/dRuntimeProfiler.cpp \
	$(SDK_TOOLBOX)/HeightFieldPrimitive.cpp \
	$(SDK_TOOLBOX)/HiResTimer.cpp  \
	$(SDK_TOOLBOX)/LevelPrimitive.cpp \
	$(SDK_TOOLBOX)/MousePick.cpp \
	$(SDK_TOOLBOX)/PlaneCollision.cpp \
	$(SDK_TOOLBOX)/RenderPrimitive.cpp \
	$(SDK_TOOLBOX)/ScenePrimitive.cpp \
	$(SDK_TOOLBOX)/ShaderPrograms.cpp \
	$(SDK_TOOLBOX)/SkyBox.cpp \
	$(SDK_TOOLBOX)/TargaToOpenGl.cpp \
	$(SDK_TOOLBOX)/UserHeightFieldCollision.cpp \
	$(SDK_TOOLBOX)/toolbox_stdafx.cpp 

SDK_XML_SRC = \
	$(SDK_XML)/tinystr.cpp \
	$(SDK_XML)/tinyxml.cpp \
	$(SDK_XML)/tinyxmlerror.cpp \
	$(SDK_XML)/tinyxmlparser.cpp  	

SDK_MATH_SRC = \
	$(SDK_MATH)/dMathDefines.cpp \
	$(SDK_MATH)/dMatrix.cpp \
	$(SDK_MATH)/dQuaternion.cpp 	
	 
SDK_JOINTS_SRC = \
	$(SDK_JOINTS)/Custom6DOF.cpp \
	$(SDK_JOINTS)/CustomBallAndSocket.cpp \
	$(SDK_JOINTS)/CustomCorkScrew.cpp \
	$(SDK_JOINTS)/CustomDryRollingFriction.cpp \
	$(SDK_JOINTS)/CustomGear.cpp \
	$(SDK_JOINTS)/CustomHinge.cpp \
	$(SDK_JOINTS)/CustomMultiBodyVehicle.cpp \
	$(SDK_JOINTS)/CustomPlayerController.cpp \
	$(SDK_JOINTS)/CustomPulley.cpp \
	$(SDK_JOINTS)/CustomRagDoll.cpp \
	$(SDK_JOINTS)/CustomDGRayCastCar.cpp \
	$(SDK_JOINTS)/CustomSlider.cpp \
	$(SDK_JOINTS)/CustomSlidingContact.cpp \
	$(SDK_JOINTS)/CustomUniversal.cpp \
	$(SDK_JOINTS)/CustomUpVector.cpp \
	$(SDK_JOINTS)/CustomPickBody.cpp \
	$(SDK_JOINTS)/CustomWormGear.cpp \
	$(SDK_JOINTS)/JointLibrary.cpp \
	$(SDK_JOINTS)/NewtonCustomJoint.cpp

SDK_CONTAINERS_SRC = \
	$(SDK_CONTAINERS)/dBaseHierarchy.cpp \
	$(SDK_CONTAINERS)/dContainersStdAfx.cpp \
	$(SDK_CONTAINERS)/dCRC.cpp \
	$(SDK_CONTAINERS)/dRefCounter.cpp \
	$(SDK_CONTAINERS)/dTree.cpp
	
SDK_ANIMATION_SRC = \
	$(SDK_ANIMATION)/dBone.cpp \
	$(SDK_ANIMATION)/dMesh.cpp \
	$(SDK_ANIMATION)/dModel.cpp \
	$(SDK_ANIMATION)/dPoseGenerator.cpp \
	$(SDK_ANIMATION)/dBonesToPoseBinding.cpp \
	$(SDK_ANIMATION)/dAnimationClip.cpp 

		
SDK_SRCS = $(SDK_MAIN_SRC) $(SDK_DEMOS_SRC) $(SDK_JOINTS_SRC) $(SDK_MATH_SRC) $(SDK_TOOLBOX_SRC) $(SDK_XML_SRC) $(SDK_ANIMATION_SRC) $(SDK_CONTAINERS_SRC)

SDK_OBJS = $(SDK_SRCS:.cpp=.o)

COMPILER = gcc
FLAGS = $(SDK_FLAGS) $(SDK_INCLUDE_PATH)

.SUFFIXES : .o .cpp
.cpp.o :
	$(COMPILER) $(FLAGS) -o $@ $<	

# main target
NewtonDemos : $(SDK_OBJS)
	gcc $? $(NEWTON_LIB) $(LIBS) -o $@.elf

# clean all objects target
clean :
	rm $(SDK_OBJS)
	touch $(SDK_SRCS)
	
	
	
