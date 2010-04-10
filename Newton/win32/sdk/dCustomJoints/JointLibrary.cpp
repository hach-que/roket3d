//////////////////////////////////////////////////////////////////////
#include "CustomJointLibraryStdAfx.h"
#include "JointLibrary.h"
#include "Custom6DOF.h"
#include "CustomPickBody.h"
#include "NewtonCustomJoint.h"
#include "CustomPlayerController.h"
#include "CustomMultiBodyVehicle.h"
#include "CustomDGRayCastCar.h"

// Generic 6 degree of Freedom Joint
NewtonCustomJoint *CreateCustomJoint6DOF (const dFloat* pinsAndPivoChildFrame, const dFloat* pinsAndPivoParentFrame, const NewtonBody* child, const NewtonBody* parent)
{
	return new Custom6DOF (*(dMatrix*) pinsAndPivoChildFrame, *(dMatrix*) pinsAndPivoParentFrame, child, parent);
}


void CustomJoint6DOF_SetLinearLimits (NewtonCustomJoint* customJoint6DOF, const dFloat* minLinearLimits, const dFloat* maxLinearLimits)
{
	((Custom6DOF*)customJoint6DOF)->SetLinearLimits (*(dVector*)minLinearLimits, *(dVector*)maxLinearLimits); 
}

void CustomJoint6DOF_SetAngularLimits (NewtonCustomJoint* customJoint6DOF, const dFloat* minAngularLimits, const dFloat* maxAngularLimits)
{
	((Custom6DOF*)customJoint6DOF)->SetAngularLimits(*(dVector*)minAngularLimits, *(dVector*)maxAngularLimits); 
}

void CustomJoint6DOF_GetLinearLimits (NewtonCustomJoint* customJoint6DOF, dFloat* minLinearLimits, dFloat* maxLinearLimits)
{
	((Custom6DOF*)customJoint6DOF)->GetLinearLimits (*(dVector*)minLinearLimits, *(dVector*)maxLinearLimits); 
}

void CustomJoint6DOF_GetAngularLimits (NewtonCustomJoint* customJoint6DOF, dFloat* minAngularLimits, dFloat* maxAngularLimits)
{
	((Custom6DOF*)customJoint6DOF)->GetAngularLimits(*(dVector*)minAngularLimits, *(dVector*)maxAngularLimits); 
}

void CustomJoint6DOF_SetReverseUniversal (NewtonCustomJoint* customJoint6DOF, int order)
{
	((Custom6DOF*)customJoint6DOF)->SetReverserUniversal(order);
}




// player controller functions 
class PlayerController: public CustomPlayerController 
{
	public: 

	typedef int (*PlayerCanPuchBody) (NewtonCustomJoint *me, const NewtonBody* hitBody);

	PlayerController (const dVector& pin, const NewtonBody* child, dFloat maxStairStepFactor)
		:CustomPlayerController (pin, child, maxStairStepFactor)
	{
		m_canPuchOtherBodies = CanPushThisBodyCallback;
	}

	virtual bool CanPushBody (const NewtonBody* hitBody) const 
	{
		if (m_canPuchOtherBodies) {
			return m_canPuchOtherBodies ((NewtonCustomJoint *)this, hitBody) ? true : false;
		} 
		return true;
	}


	static int CanPushThisBodyCallback(NewtonCustomJoint *me, const NewtonBody* hitBody) 
	{
		return 1;
	}


	PlayerCanPuchBody m_canPuchOtherBodies;
};

NewtonCustomJoint *CreateCustomPlayerController (const dFloat* pin, const NewtonBody* player, dFloat maxStairStepFactor)
{
	return (NewtonCustomJoint *) new CustomPlayerController (*(dVector*) pin, player, maxStairStepFactor);
}

void CustomPlayerControllerSetPushActorCallback (NewtonCustomJoint* playerController, PlayerCanPuchThisBodyCalback callback)
{
	((PlayerController*)playerController)->m_canPuchOtherBodies = (PlayerController::PlayerCanPuchBody)callback;
}

void CustomPlayerControllerSetVelocity (NewtonCustomJoint* playerController, dFloat forwardSpeed, dFloat sideSpeed, dFloat heading)
{
	((PlayerController*)playerController)->SetVelocity (forwardSpeed, sideSpeed, heading);
}

void CustomPlayerControllerSetMaxSlope (NewtonCustomJoint* playerController, dFloat maxSlopeAngleIndRadian)
{
	((PlayerController*)playerController)->SetMaxSlope (maxSlopeAngleIndRadian);
}
dFloat CustomPlayerControllerGetMaxSlope (NewtonCustomJoint* playerController)
{
	return ((PlayerController*)playerController)->GetMaxSlope();
}

const NewtonCollision* CustomPlayerControllerGetVerticalSensorShape (NewtonCustomJoint* playerController)
{
	return ((PlayerController*)playerController)->GetVerticalSensorShape();
}

const NewtonCollision* CustomPlayerControllerGetHorizontalSensorShape (NewtonCustomJoint* playerController)
{
	return ((PlayerController*)playerController)->GetHorizontalSensorShape ();
}

const NewtonCollision* CustomPlayerControllerGetDynamicsSensorShape (NewtonCustomJoint* playerController)
{
	return ((PlayerController*)playerController)->GetDynamicsSensorShape ();
}


// MultiBody Vehicle interface
NewtonCustomJoint *CreateCustomMultiBodyVehicle (const dFloat* frontDir, const dFloat* upDir, const NewtonBody* carBody)
{
// hacks for leadwerk
//dVector com;
//NewtonBodyGetCentreOfMass(carBody, &com[0]);
//NewtonBodySetCentreOfMass(carBody, &com[0]);
	return (NewtonCustomJoint *) new CustomMultiBodyVehicle (*((dVector*) frontDir), *((dVector*) upDir), carBody);
}


int CustomMultiBodyVehicleAddTire (NewtonCustomJoint *car, const void* userData, const dFloat* localPosition, 
								  dFloat mass, dFloat radius, dFloat width,
								  dFloat suspensionLength, dFloat springConst, dFloat springDamper)
{
	dVector posit (localPosition[0], localPosition[1], localPosition[2], 0.0f);

/*
dMatrix bodyMatrix;
const NewtonBody* carBody = ((CustomMultiBodyVehicle*)car)->GetBody0();
NewtonBodyGetMatrix(carBody, &bodyMatrix[0][0]);
posit = bodyMatrix.UntransformVector (posit);

springConst = 1005.0f;
springDamper = 0.0f;
*/
	
	return ((CustomMultiBodyVehicle*)car)->AddSingleSuspensionTire ((void*)userData, &posit[0], 
																	mass, radius, width, suspensionLength, springConst, springDamper);
}




int CustomMultiBodyVehicleAddSlipDifferencial (NewtonCustomJoint *car, int leftTireIndex, int rightToreIndex, dFloat maxFriction)
{
	return ((CustomMultiBodyVehicle*)car)->AddSlipDifferencial(leftTireIndex, rightToreIndex, maxFriction);
}


int CustomMultiBodyVehicleGetTiresCount(NewtonCustomJoint *car)
{
	return ((CustomMultiBodyVehicle*)car)->GetTiresCount();
}

const NewtonBody* CustomMultiBodyVehicleGetTireBody(NewtonCustomJoint *car, int tireIndex)
{
	return ((CustomMultiBodyVehicle*)car)->GetTireBody(tireIndex);
}

dFloat CustomMultiBodyVehicleGetSpeed(NewtonCustomJoint *car)
{
	return ((CustomMultiBodyVehicle*)car)->GetSpeed();
}

void CustomMultiBodyVehicleApplyTorque (NewtonCustomJoint *car, int tireIndex, dFloat torque)
{
	((CustomMultiBodyVehicle*)car)->ApplyTireTorque(tireIndex, torque);
}

void CustomMultiBodyVehicleApplySteering (NewtonCustomJoint *car, int tireIndex, dFloat angle)
{
	((CustomMultiBodyVehicle*)car)->ApplyTireSteerAngle(tireIndex, angle);
}

void CustomMultiBodyVehicleApplyBrake (NewtonCustomJoint *car, int tireIndex, dFloat brakeTorque)
{
	((CustomMultiBodyVehicle*)car)->ApplyTireBrake (tireIndex, brakeTorque);
}

void CustomMultiBodyVehicleApplyTireRollingDrag (NewtonCustomJoint *car, int tireIndex, dFloat angularDampingCoef)
{
	((CustomMultiBodyVehicle*)car)->ApplyTireRollingDrag (tireIndex, angularDampingCoef);
}

dFloat CustomMultiBodyVehicleGetTireSteerAngle (NewtonCustomJoint *car, int tireIndex)
{
	return ((CustomMultiBodyVehicle*)car)->GetSetTireSteerAngle (tireIndex);
}



// Pick Body joint
NewtonCustomJoint *CreateCustomPickBody (const NewtonBody* body, dFloat* handleInGlobalSpace)
{
	return new CustomPickBody (body, *((dVector*)handleInGlobalSpace));
}

void CustomPickBodySetPickMode (NewtonCustomJoint* pick, int mode)
{
	((CustomPickBody*)pick)->SetPickMode (mode);
}

void CustomPickBodySetMaxLinearFriction(NewtonCustomJoint* pick, dFloat accel) 
{
	((CustomPickBody*)pick)->SetMaxLinearFriction(accel); 
}

void CustomPickBodySetMaxAngularFriction(NewtonCustomJoint* pick, dFloat alpha) 
{
	((CustomPickBody*)pick)->SetMaxAngularFriction(alpha);
}


void CustomPickBodySetTargetRotation (NewtonCustomJoint* pick, dFloat* rotation) 
{
	((CustomPickBody*)pick)->SetTargetRotation (*((dQuaternion*) rotation)) ;
}

void CustomPickBodySetTargetPosit (NewtonCustomJoint* pick, dFloat* posit) 
{
	((CustomPickBody*)pick)->SetTargetPosit ((*(dVector*)posit));
}

void CustomPickBodySetTargetMatrix (NewtonCustomJoint* pick, dFloat* matrix) 
{
	((CustomPickBody*)pick)->SetTargetMatrix ((*(dMatrix*) matrix)); 
}

void CustomPickBodyGetTargetMatrix (NewtonCustomJoint* pick, dFloat* matrix)
{
	dMatrix& retMatrix = (*(dMatrix*) matrix);
	retMatrix = ((CustomPickBody*)pick)->GetTargetMatrix();
}




// BEGIN k00m (Dave Gravel simple raycast world vehicle)
NewtonCustomJoint *DGRaycastVehicleCreate (int maxTireCount, int chassisAxis, int tractionAxis, int steerAxis, int frictionAxis, const dFloat* cordenateSytem, NewtonBody* carBody, const dFloat* gravity)
{
  return new CustomDGRayCastCar (maxTireCount, chassisAxis, tractionAxis, steerAxis, frictionAxis, *((dMatrix*) cordenateSytem), carBody, *((dVector*) gravity));
}

void DGRaycastVehicleAddTire (NewtonCustomJoint *car, void *userData, const dFloat* localPosition, dFloat mass, dFloat radius, dFloat width, dFloat suspensionLength, dFloat springConst, dFloat springDamper, int castMode)
{
  ((CustomDGRayCastCar*)car)->AddSingleSuspensionTire(userData,localPosition,mass,radius,width,suspensionLength,springConst,springDamper,castMode);
}

const NewtonCollision* DGRayCarTireShape(NewtonCustomJoint *car, int tireIndex) 
{
  return ((CustomDGRayCastCar*)car)->GetTiresShape(tireIndex); 
}

void DGRayCarGetChassisMatrixLocal(NewtonCustomJoint *car, dFloat* chassisMatrix)
{
  (*(dMatrix*)chassisMatrix) = ((CustomDGRayCastCar*)car)->GetChassisMatrixLocal();
}

void DGRayCarTireMatrix(NewtonCustomJoint *car, int tire, dFloat* tireMatrix)
{
  (*(dMatrix*)tireMatrix) = ((CustomDGRayCastCar*)car)->CalculateTireMatrix(tire);
}

void DGRayCarSuspensionMatrix(NewtonCustomJoint *car, int tire, dFloat param, dFloat* SuspensionMatrix)
{
  (*(dMatrix*)SuspensionMatrix) = ((CustomDGRayCastCar*)car)->CalculateSuspensionMatrix(tire,param);
}

void DGRaycastVehicleSetTireBrake (NewtonCustomJoint *car, int index, dFloat torque)
{
  ((CustomDGRayCastCar*)car)->SetTireBrake(index,torque);
}

void DGRaycastVehicleSetTireTorque (NewtonCustomJoint *car, int index, dFloat torque)
{
  ((CustomDGRayCastCar*)car)->SetTireTorque(index,torque);
}

void DGRaycastVehicleSetTireSteerAngle (NewtonCustomJoint *car, int index, dFloat angle, dFloat turnforce)
{
  ((CustomDGRayCastCar*)car)->SetTireSteerAngle(index,angle,turnforce);
}

dFloat DGRaycastVehicleGetSpeed(NewtonCustomJoint *car)
{
  return ((CustomDGRayCastCar*)car)->GetSpeed();
}

int DGRaycastVehicleGetTiresCount(NewtonCustomJoint *car)
{
  return ((CustomDGRayCastCar*)car)->GetTiresCount();
}

// only use for debug from the dll client app side, it's not the real implementation
// Some stuff is really wrong on it.
void DGRaycastVehicleSetVar(NewtonCustomJoint *car, dFloat chassisOmegaCorrection, dFloat engineForce, dFloat steerForce)
{
  ((CustomDGRayCastCar*)car)->SetVehicleVar(chassisOmegaCorrection,engineForce,steerForce);
}

void DGRaycastVehicleDestroy (NewtonCustomJoint *car)
{
  delete car;
}

// END





// common Joints functions
int CustomGetJointID (NewtonCustomJoint *joint)
{
	return joint->GetJointID ();
}

void CustomSetJointID (NewtonCustomJoint *joint, int rttI)
{
	joint->SetJointID(rttI);
}

const NewtonBody* CustomGetBody0 (NewtonCustomJoint *joint)
{
	return joint->GetBody0();
}

const NewtonBody* CustomGetBody1 (NewtonCustomJoint *joint)
{
	return joint->GetBody1();
}

int CustomGetBodiesCollisionState (NewtonCustomJoint *joint)
{
	return joint->GetBodiesCollisionState();
}

void CustomSetBodiesCollisionState (NewtonCustomJoint *joint, int state)
{
	joint->SetBodiesCollisionState(state);
}

void CustomDestroyJoint(NewtonCustomJoint *joint)
{
	delete joint;
}



