#include "roket3d.h"
#include "irrlazycamera.h"
// Written by keless from the Irrlicht Forums

namespace scene
{
	 // class constructor 
	irrLazyCamera::irrLazyCamera( irr::scene::ISceneNode* targetNode,
					irr::scene::ISceneManager* smgr,
					cIrrNumber height /*30.0f*/, cIrrNumber leash /*100.0f*/,
					cIrrNumber speed /*5.0f*/, irr::s32 id /*-1*/) 
	{ 
		m_speed = speed;
		m_height = height;
		m_leash = leash;
		
		//create a camera
		lastTargetPos = targetNode->getPosition();
		m_cam = smgr->addCameraSceneNode(0,
				lastTargetPos + cIrrVector3D(0, 20, 20),
				lastTargetPos,
				id );
		m_pTargetNode = targetNode;
		m_pTargetNode->grab(); //make sure the node wont dissappear on us
	}
	
	// class destructor 
	irrLazyCamera::~irrLazyCamera() 
	{ 
		m_pTargetNode->drop(); //make sure we let go of the node resource
	}
	
	irr::core::stringc irrLazyCamera::RoketGetName()
	{
		return RoketName;
	}
		
	void irrLazyCamera::RoketSetName(irr::core::stringc newname)
	{
		RoketName = newname;
	}

	void irrLazyCamera::Update()
	{
		if(!m_cam || !m_pTargetNode) return;
		
		irr::core::vector3df currTargetPos = m_pTargetNode->getPosition();
		
		//if too far away, move camera closer
		cIrrVector3D camToTarg = currTargetPos - m_cam->getPosition();
		
		//leash is only in the X-Z plane, so only count distance using X and Z 
		cIrrVector2D xzDist( camToTarg.X, camToTarg.Z );
		
		if(xzDist.getLength() > m_leash) //need to move closer 
		{
			camToTarg = cIrrVector3D( xzDist.X, 0, xzDist.Y);
			camToTarg = camToTarg.normalize() * m_speed;
			
			//set X-Z position 
			m_cam->setPosition( m_cam->getPosition() + camToTarg ); //move closer
			//set Y position
			//the amount of leway given to how close we need to be to the right height
			cIrrNumber h_tolerance = m_height / 10;
			
			//distance from prefered height position
			cIrrNumber h_delta = m_cam->getPosition().Y - currTargetPos.Y + m_height;
			
			if (m_cam->getPosition().Y<(currTargetPos.Y+m_height)-h_tolerance)
			{
				m_cam->setPosition(
					cIrrVector3D( m_cam->getPosition().X, m_cam->getPosition().Y + h_delta/2,
					m_cam->getPosition().Z)
						  );
			}
			else if (m_cam->getPosition().Y>(currTargetPos.Y+m_height)+h_tolerance)
			{ 
				m_cam->setPosition(
					cIrrVector3D( m_cam->getPosition().X, m_cam->getPosition().Y - h_delta/2,
					m_cam->getPosition().Z)
						  ); 
			} 
		}
		
		m_cam->setTarget( currTargetPos ); //look at Target position 
		lastTargetPos = currTargetPos; 
	}
}
