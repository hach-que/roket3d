#ifndef HEADER_ROKETPHYSICSCALLBACKS
#define HEADER_ROKETPHYSICSCALLBACKS
namespace physics
{
	void _cdecl TransformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex);
	void _cdecl ApplyForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex);
	void _cdecl JointDestructor(const NewtonJoint* joint);
}
#endif