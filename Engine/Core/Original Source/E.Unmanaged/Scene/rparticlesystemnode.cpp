#include "roket3d.h"
#include "E.Unmanaged/Scene/rparticlesystemnode.h"

namespace scene
{
	const char *RParticleSystemSceneNode::className = TYPE_RPARTICLESYSTEMSCENENODE;
	const Luna < RParticleSystemSceneNode >::FunctionType RParticleSystemSceneNode::Functions[] = {
		{"setPosition", &RParticleSystemSceneNode::setPosition},
  {"getPosition", &RParticleSystemSceneNode::getPosition},
  {"setRotation", &RParticleSystemSceneNode::setRotation},
  {"getRotation", &RParticleSystemSceneNode::getRotation},
  {"applyTexture", &RParticleSystemSceneNode::applyTexture},
  {"applyFlag", &RParticleSystemSceneNode::applyFlag},
  {"applyPhysicsSettings", &RParticleSystemSceneNode::applyPhysicsSettings},
  {"applyForce", &RParticleSystemSceneNode::applyForce},
  {"applyTorque", &RParticleSystemSceneNode::applyTorque},
  {"setParent", &RParticleSystemSceneNode::setParent},
  {"setName", &RParticleSystemSceneNode::setName},
  {"getName", &RParticleSystemSceneNode::getName},
  {"setID", &RParticleSystemSceneNode::setID},
  {"getID", &RParticleSystemSceneNode::getID},
  // particle functions
  {"createBoxEmitter", &RParticleSystemSceneNode::createBoxEmitter},
  {"createSphereEmitter", &RParticleSystemSceneNode::createSphereEmitter},
  {"createRingEmitter", &RParticleSystemSceneNode::createRingEmitter},
  {"createPointEmitter", &RParticleSystemSceneNode::createPointEmitter},
		{0}
	};
	const Luna < RParticleSystemSceneNode >::PropertyType RParticleSystemSceneNode::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RParticleSystemSceneNode::RParticleSystemSceneNode(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RParticleSystemSceneNodes.  You must collect one from an RSceneManager using the add functions.",LVL_ERR,L);
		isPrecious = true; // Lua should not destroy these.
	}
	
	void RParticleSystemSceneNode::setNode(irr::scene::IParticleSystemSceneNode* meshnode, effectHandler* xe)
	{
		extended_self = meshnode;
		ident.type("RParticleSystemSceneNode");
		ident.voidPointer(this);
		isNodeSet = true;
		xeffect = xe;
		extended_self->RoketSetUserData(&ident);
	}
	
	irr::scene::IParticleSystemSceneNode* RParticleSystemSceneNode::getNode()
	{
		return extended_self;
	}

	// ================== PARTICLE FUNCTIONS =======================
	// BOX EMITTER
	int RParticleSystemSceneNode::createBoxEmitter(lua_State* L)
	{
		ifisvalid(self());

		// Get arguments
		core::RCube3D* area = Luna<core::RCube3D>::check(L,1);
		core::RVector3D* direction = Luna<core::RVector3D>::check(L,2);
		cNumber minParticlesPerSecond = getmandatoryargument_cNumber(L,3);
		cNumber maxParticlesPerSecond = getmandatoryargument_cNumber(L,4);
		core::RColor* minStartColor = Luna<core::RColor>::check(L,5);
		core::RColor* maxStartColor = Luna<core::RColor>::check(L,6);
		cNumber lifeTimeMin = getmandatoryargument_cNumber(L,7); // in milliseconds
		cNumber lifeTimeMax = getmandatoryargument_cNumber(L,8);
		cNumber maxAngleDegrees = getmandatoryargument_cNumber(L,9);
		core::RVector2D* minStartSize = Luna<core::RVector2D>::check(L,10);
		core::RVector2D* maxStartSize = Luna<core::RVector2D>::check(L,11);

		cout << "Area is: " << area->getObject().MinEdge.X << ","
							<< area->getObject().MinEdge.Y << ","
							<< area->getObject().MinEdge.Z << " "
							<< area->getObject().MaxEdge.X << ","
							<< area->getObject().MaxEdge.Y << ","
							<< area->getObject().MaxEdge.Z << " " << endl;

		// Create box emitter
		irr::scene::IParticleBoxEmitter* em = extended_self->createBoxEmitter(
				area->getObject(),
				direction->getObject(),
				minParticlesPerSecond,
				maxParticlesPerSecond,
				minStartColor->getObject(),
				maxStartColor->getObject(),
				lifeTimeMin,
				lifeTimeMax,
				maxAngleDegrees,
				minStartSize->getObjectAsDimension(),
				maxStartSize->getObjectAsDimension());
		em->setBox(area->getObject());

		extended_self->setEmitter(em);
		em->drop();

		//irr::scene::IParticleAffector* paf = extended_self->createFadeOutParticleAffector();
        //extended_self->addAffector(paf);
        //paf->drop();

		return 0;

		endifisvalid();
	}

	// SPHERE EMITTER
	int RParticleSystemSceneNode::createSphereEmitter(lua_State* L)
	{
		ifisvalid(self());

		// Get arguments
		core::RVector3D* center = Luna<core::RVector3D>::check(L,1);
		cNumber radius = getmandatoryargument_cNumber(L,2);
		core::RVector3D* direction = Luna<core::RVector3D>::check(L,3);
		cNumber minParticlesPerSecond = getmandatoryargument_cNumber(L,4);
		cNumber maxParticlesPerSecond = getmandatoryargument_cNumber(L,5);
		core::RColor* minStartColor = Luna<core::RColor>::check(L,6);
		core::RColor* maxStartColor = Luna<core::RColor>::check(L,7);
		cNumber lifeTimeMin = getmandatoryargument_cNumber(L,8); // in milliseconds
		cNumber lifeTimeMax = getmandatoryargument_cNumber(L,9);
		cNumber maxAngleDegrees = getmandatoryargument_cNumber(L,10);
		core::RVector2D* minStartSize = Luna<core::RVector2D>::check(L,11);
		core::RVector2D* maxStartSize = Luna<core::RVector2D>::check(L,12);

		// Create sphere emitter
		irr::scene::IParticleEmitter* em = extended_self->createSphereEmitter(
				center->getObject(),
				radius,
				direction->getObject(),
				minParticlesPerSecond,
				maxParticlesPerSecond,
				minStartColor->getObject(),
				maxStartColor->getObject(),
				lifeTimeMin,
				lifeTimeMax,
				maxAngleDegrees,
				minStartSize->getObjectAsDimension(),
				maxStartSize->getObjectAsDimension());

		extended_self->setEmitter(em);
		em->drop();

        //irr::scene::IParticleAffector* paf = extended_self->createFadeOutParticleAffector();
        //extended_self->addAffector(paf);
        //paf->drop();
		return 0;

		endifisvalid();
	}

	// RING EMITTER
	int RParticleSystemSceneNode::createRingEmitter(lua_State* L)
	{
		ifisvalid(self());

		// Get arguments
		core::RVector3D* center = Luna<core::RVector3D>::check(L,1);
		cNumber radius = getmandatoryargument_cNumber(L,2);
		cNumber ringThickness = getmandatoryargument_cNumber(L,3);
		core::RVector3D* direction = Luna<core::RVector3D>::check(L,4);
		cNumber minParticlesPerSecond = getmandatoryargument_cNumber(L,5);
		cNumber maxParticlesPerSecond = getmandatoryargument_cNumber(L,6);
		core::RColor* minStartColor = Luna<core::RColor>::check(L,7);
		core::RColor* maxStartColor = Luna<core::RColor>::check(L,8);
		cNumber lifeTimeMin = getmandatoryargument_cNumber(L,9); // in milliseconds
		cNumber lifeTimeMax = getmandatoryargument_cNumber(L,10);
		cNumber maxAngleDegrees = getmandatoryargument_cNumber(L,11);
		core::RVector2D* minStartSize = Luna<core::RVector2D>::check(L,12);
		core::RVector2D* maxStartSize = Luna<core::RVector2D>::check(L,13);

		// Create ring emitter
		irr::scene::IParticleEmitter* em = extended_self->createRingEmitter(
				center->getObject(),
				radius,
				ringThickness,
				direction->getObject(),
				minParticlesPerSecond,
				maxParticlesPerSecond,
				minStartColor->getObject(),
				maxStartColor->getObject(),
				lifeTimeMin,
				lifeTimeMax,
				maxAngleDegrees,
				minStartSize->getObjectAsDimension(),
				maxStartSize->getObjectAsDimension());

		extended_self->setEmitter(em);
		em->drop();

        //irr::scene::IParticleAffector* paf = extended_self->createFadeOutParticleAffector();
        //extended_self->addAffector(paf);
        //paf->drop();
		return 0;

		endifisvalid();
	}

	// POINT EMITTER
	int RParticleSystemSceneNode::createPointEmitter(lua_State* L)
	{
		ifisvalid(self());

		// Get arguments
		core::RVector3D* direction = Luna<core::RVector3D>::check(L,1);
		cNumber minParticlesPerSecond = getmandatoryargument_cNumber(L,2);
		cNumber maxParticlesPerSecond = getmandatoryargument_cNumber(L,3);
		core::RColor* minStartColor = Luna<core::RColor>::check(L,4);
		core::RColor* maxStartColor = Luna<core::RColor>::check(L,5);
		cNumber lifeTimeMin = getmandatoryargument_cNumber(L,6); // in milliseconds
		cNumber lifeTimeMax = getmandatoryargument_cNumber(L,7);
		cNumber maxAngleDegrees = getmandatoryargument_cNumber(L,8);
		core::RVector2D* minStartSize = Luna<core::RVector2D>::check(L,9);
		core::RVector2D* maxStartSize = Luna<core::RVector2D>::check(L,10);

		// Create point emitter
		irr::scene::IParticleEmitter* em = extended_self->createPointEmitter(
				direction->getObject(),
				minParticlesPerSecond,
				maxParticlesPerSecond,
				minStartColor->getObject(),
				maxStartColor->getObject(),
				lifeTimeMin,
				lifeTimeMax,
				maxAngleDegrees,
				minStartSize->getObjectAsDimension(),
				maxStartSize->getObjectAsDimension());

		extended_self->setEmitter(em);
		em->drop();

        //irr::scene::IParticleAffector* paf = extended_self->createFadeOutParticleAffector();
        //extended_self->addAffector(paf);
        //paf->drop();
		return 0;

		endifisvalid();
	}


}
