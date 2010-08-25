// CustomDGRayCastCar.cpp: implementation of the CustomDGRayCastCar class.
// This raycast vehicle is currently a work in progress by Dave Gravel - 2009.
//////////////////////////////////////////////////////////////////////
#include "CustomJointLibraryStdAfx.h"
#include "CustomDGRayCastCar.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CustomDGRayCastCar::CustomDGRayCastCar(int maxTireCount, const dMatrix& cordenateSytem, NewtonBody* carBody)
	:NewtonCustomJoint(2 * maxTireCount, carBody, NULL)
{
	dVector com;
	dMatrix tmp;

	m_tiresRollSide = 0;
    // chassis rotation fix...
	m_chassisRotationLimit = 0.98f;
	// engine acceleration...
    m_engineForce = 100.0f;
	// steer turn left right force...
    m_steerForce  = 100.0f;

	// set the chassis matrix at the center of mass
	NewtonBodyGetCentreOfMass(m_body0, &com[0]);
	com.m_w = 1.0f;

	// set the joint reference point at the center of mass of the body
	dMatrix chassisMatrix (cordenateSytem);
	chassisMatrix.m_posit += chassisMatrix.RotateVector(com);

	CalculateLocalMatrix (chassisMatrix, m_localFrame, tmp);

	// allocate space for the tires;
	m_tiresCount = 0;
	m_tires = new Tire[maxTireCount];

	m_curSpeed = 0.0f;
	m_aerodynamicDrag = 0.1f; 
	m_aerodynamicDownForce = 0.1f; 

	dFloat Ixx; 
	dFloat Iyy; 
	dFloat Izz; 
	NewtonBodyGetMassMatrix(m_body0, &m_mass, &Ixx, &Iyy, &Izz);

	// register the callback for tire integration
//	NewtonUserJointSetFeedbackCollectorCallback (m_joint, IntegrateTires);
}

CustomDGRayCastCar::~CustomDGRayCastCar()
{
	NewtonWorld *world;

	world = NewtonBodyGetWorld (m_body0);
	for (int i = 0; i < m_tiresCount; i ++) {
		NewtonReleaseCollision (world, m_tires[i].m_shape);
	}

	if(m_tires) {
		delete[] m_tires;
	}
}

void CustomDGRayCastCar::SetVehicleVar (dFloat chassisRotationLimit, dFloat defaultEngineForce, dFloat defaultSteerForce)
{
  m_chassisRotationLimit = chassisRotationLimit;
  m_engineForce = defaultEngineForce;
  m_steerForce  = defaultSteerForce;
}

int CustomDGRayCastCar::GetTiresCount() const
{
	return m_tiresCount;
}

const NewtonCollision* CustomDGRayCastCar::GetTiresShape (int tireIndex) const
{
  const Tire& tire = m_tires[tireIndex];	
  return tire.m_shape;
}


void CustomDGRayCastCar::GetInfo (NewtonJointRecord* info) const
{
}

//this function is to be overloaded by a derive class
void CustomDGRayCastCar::SetSteering (dFloat angle)
{
}

//this function is to be overloaded by a derive class
void CustomDGRayCastCar::SetBrake (dFloat torque)
{
}

//this function is to be overloaded by a derive class
void CustomDGRayCastCar::SetTorque (dFloat torque)
{
}

dFloat CustomDGRayCastCar::GetSpeed() const
{
	return m_curSpeed;
}


void CustomDGRayCastCar::SetTireMaxRPS (int tireIndex, dFloat maxTireRPS)
{
	m_tires[tireIndex].m_maxTireRPS = maxTireRPS;
}

CustomDGRayCastCar::Tire& CustomDGRayCastCar::GetTire (int index) const
{
	return m_tires[index];
}

dFloat CustomDGRayCastCar::GetParametricPosition (int index) const
{
	return m_tires[index].m_posit / m_tires[index].m_raycastLength;
}

void CustomDGRayCastCar::SetTireSteerAngle (int index, dFloat angle, dFloat turnforce)
{

   m_tires[index].m_steerAngle = angle;
   m_tires[index].m_localAxis.m_z = dCos (angle);
   m_tires[index].m_localAxis.m_x = dSin (angle);
	if (m_tiresRollSide==0) {
	  m_tires[index].m_turnforce = turnforce;
	} else {
	  m_tires[index].m_turnforce = -turnforce;
	}
}

void CustomDGRayCastCar::SetTireTorque (int index, dFloat torque)
{
//torque=-600.0f;
	m_tires[index].m_torque = torque;
}

void CustomDGRayCastCar::SetTireBrake (int index, dFloat torque)
{
	m_tires[index].m_breakTorque = torque;
}


void CustomDGRayCastCar::AddSingleSuspensionTire (
	void *userData,
	const dVector& localPosition, 
	dFloat mass,
	dFloat radius, 
	dFloat width,
	dFloat friction,
	dFloat suspensionLimit,
	dFloat springConst,
	dFloat springDamper,
	int castMode)
{
	// calculate the tire local base pose matrix
	dMatrix bodyMatrix;
	m_tires[m_tiresCount].m_suspensionMatrix      = GetIdentityMatrix();
	m_tires[m_tiresCount].m_steerMatrix           = GetIdentityMatrix();
	m_tires[m_tiresCount].m_contactPoint          = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_tireAxelPosit         = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_localAxelPosit        = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_tireAxelVeloc         = dVector (0.0f, 0.0f, 0.0f, 1.0f);
    m_tires[m_tiresCount].m_lateralPin            = dVector (0.0f, 0.0f, 0.0f, 1.0f);
    m_tires[m_tiresCount].m_longitudinalPin       = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_tireSideForce         = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	m_tires[m_tiresCount].m_HitBody               = NULL;
	m_tires[m_tiresCount].m_torque                = 0.0f;
	m_tires[m_tiresCount].m_sideHit               = 0;
	m_tires[m_tiresCount].m_isBraking             = 0;
	m_tires[m_tiresCount].m_turnforce             = 0.0f;
	m_tires[m_tiresCount].m_Relpoint              = localPosition;
	m_tires[m_tiresCount].m_harpoint              = m_localFrame.UntransformVector(localPosition); 

	m_tires[m_tiresCount].m_localAxis             = m_localFrame.UnrotateVector(dVector (0.0f, 0.0f, 1.0f, 0.0f));
	m_tires[m_tiresCount].m_localAxis.m_w         = 0.0f;

	m_tires[m_tiresCount].m_userData              = userData;
	m_tires[m_tiresCount].m_angularVelocity       = 0.0f;
	m_tires[m_tiresCount].m_spinAngle             = 0.0f;
	m_tires[m_tiresCount].m_steerAngle            = 0.0f;
	
	m_tires[m_tiresCount].m_tireWidth             = width;
	m_tires[m_tiresCount].m_posit                 = localPosition[1];
	m_tires[m_tiresCount].m_raycastLength         = -localPosition[1];
	//if ( suspensionLimit < 0.01f ) {
    //  suspensionLimit = 0.01f; 
	//}
	m_tires[m_tiresCount].m_suspensionLimit       = suspensionLimit;
	m_tires[m_tiresCount].m_tireLoad              = 0.0f;
	m_tires[m_tiresCount].m_breakTorque           = 0.0f;
	m_tires[m_tiresCount].m_localSuspentionSpeed  = 0.0f;
	
	m_tires[m_tiresCount].m_springConst           = springConst;
	m_tires[m_tiresCount].m_springDamper          = springDamper;
	m_tires[m_tiresCount].m_groundFriction        = friction;

	m_tires[m_tiresCount].m_tireUseConvexCastMode = castMode; 
//	m_tires[m_tiresCount].m_tireJacobianRowIndex = -1;

	// make a convex shape to represent the tire collision
	#define TIRE_SHAPE_SIZE 12
	dVector shapePoints[TIRE_SHAPE_SIZE * 2];
	for (int i = 0; i < TIRE_SHAPE_SIZE; i ++) {
		shapePoints[i].m_x = -width * 0.5f;	
		shapePoints[i].m_y = radius * dCos (2.0f * 3.1416 * dFloat(i)/ dFloat(TIRE_SHAPE_SIZE));
		shapePoints[i].m_z = radius * dSin (2.0f * 3.1416 * dFloat(i)/ dFloat(TIRE_SHAPE_SIZE));
		shapePoints[i + TIRE_SHAPE_SIZE].m_x = -shapePoints[i].m_x;
		shapePoints[i + TIRE_SHAPE_SIZE].m_y = shapePoints[i].m_y;
		shapePoints[i + TIRE_SHAPE_SIZE].m_z = shapePoints[i].m_z;
	}
	m_tires[m_tiresCount].m_shape = NewtonCreateConvexHull (m_world, TIRE_SHAPE_SIZE * 2, &shapePoints[0].m_x, sizeof (dVector), 0.0f, NULL);
	
	// calculate the tire geometrical parameters
	m_tires[m_tiresCount].m_radius = radius;
//	m_tires[m_tiresCount].m_radiusInv  = 1.0f / m_tires[m_tiresCount].m_radius;
	m_tires[m_tiresCount].m_mass = mass;	
	m_tires[m_tiresCount].m_massInv = 1.0f / m_tires[m_tiresCount].m_mass;	
	m_tires[m_tiresCount].m_Ixx = mass * radius * radius / 2.0f;
	m_tires[m_tiresCount].m_IxxInv = 1.0f / m_tires[m_tiresCount].m_Ixx;
	SetTireMaxRPS (m_tiresCount, 150.0f / radius);

	m_tiresCount ++;
}


const dMatrix& CustomDGRayCastCar::GetChassisMatrixLocal () const
{
	return m_localFrame;
}

dMatrix CustomDGRayCastCar::CalculateSuspensionMatrix (int tireIndex, dFloat distance) const
{
	const Tire& tire = m_tires[tireIndex];

	dMatrix matrix;
	// calculate the steering angle matrix for the axis of rotation
	matrix.m_front = tire.m_localAxis;
	matrix.m_up    = dVector (0.0f, 1.0f, 0.0f, 0.0f);
	matrix.m_right = dVector (-tire.m_localAxis.m_z, 0.0f, tire.m_localAxis.m_x, 0.0f);
	matrix.m_posit = tire.m_harpoint - m_localFrame.m_up.Scale (distance);
	return matrix;
}

dMatrix CustomDGRayCastCar::CalculateTireMatrix (int tireIndex) const
{
	const Tire& tire = m_tires[tireIndex];

	// calculate the rotation angle matrix
	dMatrix angleMatrix (dPitchMatrix(tire.m_spinAngle));

	// get the tire body matrix
	dMatrix bodyMatrix;
	NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
	return angleMatrix * CalculateSuspensionMatrix (tireIndex, tire.m_posit) * m_localFrame * bodyMatrix;

}


unsigned CustomDGRayCastCar::ConvexCastPrefilter(const NewtonBody* body, const NewtonCollision* collision, void* userData)
{
	NewtonBody* me;

	me = (NewtonBody*) userData;
	// do no cast myself
	return (me != body);
}


void CustomDGRayCastCar::CalculateTireCollision (Tire& tire, int threadIndex) const
{
  tire.m_sideHit = 0;
  int floorcontact = 0;
  //tire.m_tireSideForce = dVector (0.0f, 0.0f, 0.0f, 1.0f);
  tire.m_HitBody = NULL;
  if (tire.m_tireUseConvexCastMode) {
    //dMatrix bodyMatrix;
    //NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
    //dMatrix chassisMatrix (m_localFrame * bodyMatrix); 
    NewtonWorldConvexCastReturnInfo info;
    //NewtonWorldConvexCastReturnInfo info2;
    //NewtonWorldConvexCastReturnInfo info3;
    dVector destination (tire.m_suspensionMatrix.TransformVector(m_localFrame.m_up.Scale (-(tire.m_raycastLength))));   
    //dVector destination2 (/*tire.m_suspensionMatrix*/chassisMatrix.TransformVector(m_localFrame.m_right.Scale (-tire.m_tireWidth * 0.5f)));
    //dVector destination3 (/*tire.m_suspensionMatrix*/chassisMatrix.TransformVector(m_localFrame.m_right.Scale (tire.m_tireWidth * 0.5f)));  
    dFloat hitParam;
	//dFloat hitParam1;
	//dFloat hitParam2;
    if (NewtonWorldConvexCast (m_world, &tire.m_suspensionMatrix[0][0], &destination[0], tire.m_shape, &hitParam, (void*)m_body0, ConvexCastPrefilter, &info, 1, threadIndex)){
      tire.m_posit = hitParam * tire.m_raycastLength;
      tire.m_contactPoint = info.m_point;
      tire.m_contactNormal = info.m_normal;
      // It's now a user var
	  //tire.m_groundFriction = 1.1f;
	  tire.m_HitBody = (NewtonBody*)info.m_hitBody; 
	  floorcontact = 1;
    /*} else 
    if (NewtonWorldConvexCast (m_world, &tire.m_suspensionMatrix[0][0], &destination2[0], tire.m_shape, &hitParam1, (void*)m_body0, ConvexCastPrefilter, &info2, 1, threadIndex)){
	  if (floorcontact) {
	    tire.m_posit = hitParam * tire.m_raycastLength;
	  } else {
        tire.m_posit = hitParam1 * tire.m_raycastLength;
	  }
	  tire.m_HitBody = (NewtonBody*)info2.m_hitBody; 
      tire.m_contactPoint = info2.m_point;
      tire.m_contactNormal = info2.m_normal;
      tire.m_groundFriction = 1.0f;
      tire.m_tireSideForce += chassisMatrix.m_up.Scale(m_mass*10+50*10+1.0f);
	  tire.m_sideHit = 1;
    } else 
    if (NewtonWorldConvexCast (m_world, &tire.m_suspensionMatrix[0][0], &destination3[0], tire.m_shape, &hitParam2, (void*)m_body0, ConvexCastPrefilter, &info3, 1, threadIndex)){
	  if (floorcontact) {
	    tire.m_posit = hitParam * tire.m_raycastLength;
	  } else {
        tire.m_posit = hitParam2 * tire.m_raycastLength;
	  }	
	  tire.m_HitBody = (NewtonBody*)info3.m_hitBody; 
      tire.m_contactPoint = info3.m_point;
      tire.m_contactNormal = info3.m_normal;
      tire.m_groundFriction = 1.0f;	
      tire.m_tireSideForce += chassisMatrix.m_up.Scale(m_mass*10+50*10+1.0f);
      tire.m_sideHit = 2;*/
    } else {
	  tire.m_posit = tire.m_raycastLength;
	  //tire.m_groundFriction = 0.0f;
      tire.m_contactPoint = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	  tire.m_lateralPin = dVector (0.0f, 0.0f, 0.0f, 0.0f);
      tire.m_longitudinalPin = dVector (0.0f, 0.0f, 0.0f, 0.0f);
	  tire.m_longitudinalDir = dVector (0.0f, 0.0f, 0.0f, 0.0f);
	  tire.m_lateralDir = dVector (0.0f, 0.0f, 0.0f, 0.0f);
    }
  } else {
    struct RayCastInfo
    {
      RayCastInfo(const NewtonBody* body)
	  {
	    m_param = 1.0f;
	    m_me = body;
	    m_hitBody = NULL;
	    m_contactID = 0;
	    m_normal = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	  }
	  static dFloat RayCast (const NewtonBody* body, const dFloat* normal, int collisionID, void* userData, dFloat intersetParam)
	  {
	    RayCastInfo& caster = *((RayCastInfo*) userData); 
	    // if this body is not the vehicle, see if a close hit
	    if (body != caster.m_me) {
	      if (intersetParam < caster.m_param) {
	        // this is a close hit, record the information. 
		    caster.m_param = intersetParam;
		    caster.m_hitBody = body;
		    caster.m_contactID = collisionID;
		    caster.m_normal = dVector (normal[0], normal[1], normal[2], 1.0f);
		  } 
	    }
	    return intersetParam;
      }
      dFloat m_param;
      dVector m_normal;
      const NewtonBody* m_me;
      const NewtonBody* m_hitBody;
      int m_contactID;
    };
    RayCastInfo info (m_body0);
    // extend the ray by the radius of the tire
    dFloat dist (tire.m_raycastLength + tire.m_radius);
    dVector destination (tire.m_suspensionMatrix.TransformVector(m_localFrame.m_up.Scale (-dist)));	
    // cast a ray to the world ConvexCastPrefilter
    NewtonWorldRayCast(m_world, &tire.m_suspensionMatrix.m_posit[0], &destination[0], RayCastInfo::RayCast, &info, &ConvexCastPrefilter);
    // if the ray hit something, it means the tire has some traction
    if (info.m_hitBody) {
      dFloat intesectionDist;
	  tire.m_HitBody = (NewtonBody*)info.m_hitBody; 
      tire.m_contactPoint = tire.m_suspensionMatrix.m_posit + (destination - tire.m_suspensionMatrix.m_posit).Scale (info.m_param); 
      tire.m_contactNormal = info.m_normal;  
      // TO DO: get the material properties for tire frictions on different roads 
      intesectionDist = (dist * info.m_param - tire.m_radius);
	  if (intesectionDist<0.0f) {
        intesectionDist = 0.0f;
	  } else
      if (intesectionDist > tire.m_raycastLength) {
        intesectionDist = tire.m_raycastLength;
	  }
      tire.m_posit = intesectionDist;
	  // It's now a user var
	  //tire.m_groundFriction = 1.1f;
    } else {
      tire.m_posit = tire.m_raycastLength;
	  // It's now a user var
	  //tire.m_groundFriction = 0.0f;
    }
  }
}

void CustomDGRayCastCar::SubmitConstrainst(dFloat timestep, int threadIndex)
{
  dFloat invTimestep;
  dMatrix bodyMatrix;  
  // get the simulation time
  invTimestep = 1.0f / timestep ;
  // get the vehicle global matrix, and use it in several calculations
  NewtonBodyGetMatrix(m_body0, &bodyMatrix[0][0]);
  dMatrix chassisMatrix (m_localFrame * bodyMatrix);
  //CalculateGlobalMatrix (m_localMatrix0, m_localMatrix1, matrix0, matrix1);
  // calculate all suspension forces due to spring and damper
  dVector m_chassisForce (0.0f, 0.0f, 0.0f, 0.0f);
  dVector m_chassisTorque (0.0f, 0.0f, 0.0f, 0.0f);
  // get the chassis instantaneous linear and angular velocity in the local space of the chassis
  int longitidunalForceIndex;
  int isloadedtire = 0;
  longitidunalForceIndex = 0;
  NewtonBodyGetVelocity(m_body0, &m_chassisVelocity[0]);
  NewtonBodyGetOmega(m_body0, &m_chassisOmega[0]);
  for (int i = 0; i < m_tiresCount; i ++) {
	Tire& tire = m_tires[i];
	tire.m_isBraking = 0;
	// calculate all suspension matrices in global space and tire collision
	tire.m_suspensionMatrix = CalculateSuspensionMatrix (i, 0.0f) * chassisMatrix;
	// calculate the tire collision
	CalculateTireCollision (tire,threadIndex);
	// calculate the linear velocity of the tire at the ground contact
	tire.m_tireAxelPosit = (chassisMatrix.TransformVector(tire.m_harpoint - m_localFrame.m_up.Scale (tire.m_posit)));
	tire.m_localAxelPosit = (tire.m_tireAxelPosit - chassisMatrix.m_posit);
    tire.m_tireAxelVeloc = (m_chassisVelocity + m_chassisOmega * tire.m_localAxelPosit); 
	//
    tire.m_lateralPin = (chassisMatrix.RotateVector (tire.m_localAxis));
	tire.m_longitudinalPin = (chassisMatrix.m_up * tire.m_lateralPin);
	tire.m_longitudinalDir = tire.m_longitudinalPin;
	tire.m_lateralDir = tire.m_lateralPin; 
    if (tire.m_posit < tire.m_raycastLength)  {
      dFloat speed;
      // TO DO: need to calculate the velocity if the other body at the point
	  // for now assume the ground is a static body
	  dVector hitBodyVeloc(0, 0, 0, 0);
	  if (tire.m_HitBody){
        NewtonBodyGetVelocity(tire.m_HitBody,&hitBodyVeloc[0]);
	  } 
	  // calculate the relative velocity
	  dVector relVeloc (tire.m_tireAxelVeloc - hitBodyVeloc);
	  speed = -(relVeloc % chassisMatrix.m_up);
	  // now calculate the tire load at the contact point
	  dFloat distance = ApplySuspenssionLimit(tire);
	  tire.m_tireLoad = - NewtonCalculateSpringDamperAcceleration (timestep, tire.m_springConst, distance, tire.m_springDamper, speed);
	  if (tire.m_tireLoad < 0.0f) {
	    // since the tire is not a body with real mass it can only push the chassis.
		tire.m_tireLoad = 0.0f;
	  } else {
	    //this suspension is applying a normalize force to the car chassis, need to scales by the mass of the car
		tire.m_tireLoad *= m_mass * 0.5f;
		// apply the tire model to these wheel		
		ApplyTireFrictionVelocitySiding(tire, chassisMatrix, tire.m_tireAxelVeloc, tire.m_tireAxelPosit, timestep, invTimestep, longitidunalForceIndex);
		isloadedtire = 1;
	  }
	  // convert the tire load force magnitude to a torque and force.
	  dVector tireForce (chassisMatrix.m_up.Scale (tire.m_tireLoad));
	  // accumulate the force and torque form this suspension
	  m_chassisForce = tireForce;
	  // tire force on side... unfinished not sure how to do this...
      /* if (tire.m_sideHit==1) {
        //printf("hit Side Left <- ");
        m_chassisForce += tire.m_tireSideForce;
      } else
      if (tire.m_sideHit==2) {
        //printf("hit Side Right -> ");
        m_chassisForce += tire.m_tireSideForce;
      }*/
	  if (isloadedtire!=0) {
        m_chassisForce += chassisMatrix.m_front.Scale(tire.m_torque*-m_engineForce); 
	    if (dAbs(m_curSpeed)!=0.0f) {
          m_chassisForce += chassisMatrix.m_right.Scale(tire.m_turnforce*m_steerForce); 
	  }
      if (tire.m_groundFriction!=0) {
        //ApplyTireFrictionModel(chassisMatrix, timestep);
	    ApplyChassisForceAndTorque(m_chassisForce,tire.m_tireAxelPosit); 
		ApplyOmegaCorrection();  
		NewtonBodySetOmega(m_body0,&m_chassisOmega[0]);
        ApplyDeceleration(tire);
	  } 
	 }
    } else {
	  tire.m_posit = tire.m_raycastLength;
	  //tire is on the air  not force applied to the vehicle.
	  tire.m_tireLoad = 0.0f;
//	  tire.m_tireJacobianRowIndex = -1;
	  dFloat torque;
	  torque = tire.m_torque - tire.m_angularVelocity * tire.m_Ixx * 0.1f;
	  tire.m_angularVelocity  += torque * tire.m_IxxInv * timestep;
	}
	ApplyTiresTorqueVisual(tire,timestep,threadIndex);
	// set the current vehicle speed
	m_curSpeed = bodyMatrix.m_front % m_chassisVelocity;
	if (m_curSpeed>0) { 
      m_tiresRollSide = 0; 
	} else {
      m_tiresRollSide = 1;
	}
  }
}

void CustomDGRayCastCar::ApplyTiresTorqueVisual(Tire& tire, dFloat timestep, int threadIndex)
{
  dFloat timestepInv;
  // get the simulation time
  timestepInv = 1.0f / timestep;
  dVector tireRadius (tire.m_contactPoint - tire.m_tireAxelPosit);
  //check if any engine torque or brake torque is applied to the tire
  if (dAbs(tire.m_torque) < 1.0e-3f){
    //tire is coasting, calculate the tire zero slip angular velocity
    // this is the velocity that satisfy the constraint equation
	// V % dir + W * R % dir = 0
	// where V is the tire Axel velocity
	// W is the tire local angular velocity
	// R is the tire radius
	// dir is the longitudinal direction of of the tire.		
	dFloat tireLinearSpeed;
	dFloat tireContactSpeed;
	tireLinearSpeed = tire.m_tireAxelVeloc % tire.m_longitudinalDir;
	tireContactSpeed = (tire.m_lateralDir * tireRadius) % tire.m_longitudinalDir;
    // this checkup is suposed to fix a infinit division by zero...
    if ( dAbs(tireContactSpeed)  > 1.0e-3) { 
      tire.m_angularVelocity = - (tireLinearSpeed) / (tireContactSpeed);
    } else {
      tire.m_angularVelocity = 0.0f;
    }
	//tire.m_angularVelocity = - tireLinearSpeed / tireContactSpeed;
  } else {
	// tire is under some power, need to do the free body integration to apply the net torque
	dFloat tireLinearSpeed;
	dFloat tireContactSpeed;
	tireLinearSpeed = tire.m_tireAxelVeloc % tire.m_longitudinalDir;
	
	tireContactSpeed = (tire.m_lateralDir * tireRadius) % tire.m_longitudinalDir;
    dFloat nettorque = 0.0f;
    // this checkup is suposed to fix a infinit division by zero...
    if ( dAbs(tireContactSpeed)  > 1.0e-3) { 
      nettorque = - (tireLinearSpeed) / (tireContactSpeed);
    } else {
      nettorque = 0.0f;
    }
	//tire.m_angularVelocity = - tireLinearSpeed / tireContactSpeed;
	dFloat torque;
	torque = tire.m_torque - nettorque - tire.m_angularVelocity * tire.m_Ixx * 0.1f;
	tire.m_angularVelocity  += torque * tire.m_IxxInv * timestep;
  }
  // integrate tire angular velocity and rotation
  tire.m_spinAngle = dMod (tire.m_spinAngle + tire.m_angularVelocity * timestep, 3.1416f * 2.0f); 
  // reset tire torque to zero after integration; 
  tire.m_torque = 0.0f;
}

void CustomDGRayCastCar::ApplyTireFrictionModel(const dMatrix& chassisMatrix, dFloat timestep)
{
  // m_chassisMassReduction only use for debug and test from the dll, it's not the real implementation
  // add aerodynamic forces
  dVector verticalVeloc (chassisMatrix.m_up.Scale (chassisMatrix.m_up % m_chassisVelocity));
  dVector horizontalVeloc (m_chassisVelocity - verticalVeloc);
  m_chassisForce -= horizontalVeloc.Scale ( m_aerodynamicDrag * m_mass * 0.5f );
  m_chassisForce -= chassisMatrix.m_up.Scale ( m_aerodynamicDownForce * m_mass * 0.5f * sqrtf (horizontalVeloc % horizontalVeloc));
}

dFloat CustomDGRayCastCar::ApplySuspenssionLimit(Tire& tire)
{
  dFloat distance;
  distance = ( tire.m_raycastLength - (tire.m_posit) );
  if ( distance > ( tire.m_raycastLength - tire.m_suspensionLimit ) ){ 
    distance = tire.m_raycastLength + tire.m_suspensionLimit;
  }
  //printf("%f ",distance);
  return distance;
}

void CustomDGRayCastCar::ApplyOmegaCorrection()
{ // Fix the body rotation...
  m_chassisOmega = m_chassisOmega.Scale( m_chassisRotationLimit );
}

void CustomDGRayCastCar::ApplyChassisForceAndTorque(const dVector& vForce, const dVector& vPoint)
{
  NewtonBodyAddForce( m_body0, &vForce[0] );
  ApplyChassisTorque( vForce, vPoint ); 
}

void CustomDGRayCastCar::ApplyDeceleration(Tire& tire)
{
  if (dAbs(tire.m_torque) < 1.0e-3f){
    dVector cvel = m_chassisVelocity;   
    cvel = cvel.Scale(0.9975f);
    cvel.m_y = m_chassisVelocity.m_y;
    NewtonBodySetVelocity(m_body0,&cvel.m_x);
  }	
}

void CustomDGRayCastCar::ApplyChassisTorque(const dVector& vForce, const dVector& vPoint)
{
  dVector Torque;
  dMatrix M;
  dVector com;
  NewtonBodyGetCentreOfMass( m_body0, &com[0] );
  NewtonBodyGetMatrix(m_body0,&M[0][0]);
  Torque = ( vPoint - M.TransformVector( dVector( com.m_x, com.m_y, com.m_z, 1.0f ) ) ) * vForce;
  NewtonBodyAddTorque( m_body0, &Torque[0] );
}

void CustomDGRayCastCar::ApplyTireFrictionVelocitySiding(Tire& tire, const dMatrix& chassisMatrix, const dVector& tireAxelVeloc, const dVector& tireAxelPosit, dFloat timestep, dFloat invTimestep, int& longitudinalForceIndex)
{
  tire.m_lateralPin = (chassisMatrix.RotateVector (tire.m_localAxis));
  tire.m_longitudinalPin = (chassisMatrix.m_up * tire.m_lateralPin);
  tire.m_longitudinalDir = tire.m_longitudinalPin;
  tire.m_lateralDir = tire.m_lateralPin; 
  // TO DO: need to subtract the velocity at the contact point of the hit body
  // for now assume the ground is a static body
  dVector hitBodyContactVeloc (0, 0, 0, 0);
  if (tire.m_HitBody){
    NewtonBodyGetVelocity(tire.m_HitBody,&hitBodyContactVeloc[0]);
  } 
  // calculate relative velocity at the tire center
  dVector tireAxelRelativeVelocity (tireAxelVeloc - hitBodyContactVeloc); 
  // now calculate relative velocity a velocity at contact point
  dVector tireAngularVelocity (tire.m_lateralPin.Scale (tire.m_angularVelocity));
  dVector tireRadius (tire.m_contactPoint - tireAxelPosit);
  dVector tireContactVelocity (tireAngularVelocity * tireRadius);	
  dVector tireContactRelativeVelocity (tireAxelRelativeVelocity + tireContactVelocity); 
  tire.m_tireRadius = tireRadius;
  // Apply brake, need some little fix here.
  if (dAbs(tire.m_breakTorque) > 1.0e-3f) {
    tire.m_isBraking = 1;
    tire.m_torque = 0.0f;
	tire.m_turnforce = tire.m_turnforce * 0.5f;
	tire.m_breakTorque /= timestep;
	NewtonUserJointAddLinearRow (m_joint, &tireAxelPosit[0], &tireAxelPosit[0], &tire.m_longitudinalPin[0]);//&chassisMatrix.m_front[0]
	NewtonUserJointSetRowMaximumFriction(m_joint, tire.m_breakTorque);
    NewtonUserJointSetRowMinimumFriction(m_joint, -tire.m_breakTorque);
  } 
  tire.m_breakTorque = 0.0f;  
  // the tire is in coasting mode
  //		dFloat tireContactSpeed;
  //		dFloat tireRelativeSpeed;
  //		dFloat lateralForceMagnitud;
  //these tire is coasting, so the lateral friction dominates the behaviors
  dFloat invMag2;
  dFloat frictionCircleMag;
  dFloat lateralFrictionForceMag;
  dFloat longitudinalFrictionForceMag;
  frictionCircleMag = tire.m_tireLoad * tire.m_groundFriction;
  lateralFrictionForceMag = frictionCircleMag;
  longitudinalFrictionForceMag = tire.m_tireLoad * 1.0f;
  invMag2 = frictionCircleMag / dSqrt (lateralFrictionForceMag * lateralFrictionForceMag + longitudinalFrictionForceMag * longitudinalFrictionForceMag);
  lateralFrictionForceMag *= invMag2;
  longitudinalFrictionForceMag = invMag2;
  NewtonUserJointAddLinearRow (m_joint, &tireAxelPosit[0], &tireAxelPosit[0], &tire.m_lateralPin[0]);
  NewtonUserJointSetRowMaximumFriction(m_joint,  lateralFrictionForceMag);
  NewtonUserJointSetRowMinimumFriction(m_joint,  -lateralFrictionForceMag);
  // save the tire contact longitudinal velocity for integration after the solver
  tire.m_currentSlipVeloc = tireAxelRelativeVelocity % tire.m_longitudinalPin;
}




