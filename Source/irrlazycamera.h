#ifndef HEADER_IRRLAZYCAMERA
#define HEADER_IRRLAZYCAMERA
namespace scene
{
	class irrLazyCamera 
	{
		// have to manually add in roketname because it is not an irrlicht class
		irr::core::stringc RoketName;
		irr::scene::ISceneNode* m_pTargetNode; 
		irr::scene::ICameraSceneNode* m_cam; 
 
		cIrrVector3D lastTargetPos; 
     
		cIrrNumber m_height; //distance above object camera tries to cNumber at 
		cIrrNumber m_leash;  //max distance object can be from camera before it moves 
		cIrrNumber m_speed;  //rate at which camera moves per Update() 
     
		public: 
			// class constructor 
			irrLazyCamera(irr::scene::ISceneNode* targetNode,
					irr::scene::ISceneManager* smgr, 
					cIrrNumber height = 80.0f, cIrrNumber leash = 100.0f,
					cIrrNumber speed = 1.0f, irr::s32 id = -1); 
       			// class destructor 
			~irrLazyCamera(); 
			irr::scene::ICameraSceneNode* getCam() {  return m_cam; } 
			void Update();
			// roketname functions
			void RoketSetName(irr::core::stringc newname);
			irr::core::stringc RoketGetName();
	};
}
#endif
