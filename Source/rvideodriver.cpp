#include "roket3d.h"
#include "rvideodriver.h"

namespace video
{
	const char *RVideoDriver::className = TYPE_RVIDEODRIVER;
	const Luna < RVideoDriver >::FunctionType RVideoDriver::Functions[] = {
		{"beginScene", &RVideoDriver::beginScene},
  {"clearZBuffer", &RVideoDriver::clearZBuffer},
  {"createScreenShot", &RVideoDriver::createScreenShot},
  {"endScene", &RVideoDriver::endScene},
  {"getFPS", &RVideoDriver::getFPS},
  {"getTexture", &RVideoDriver::getTexture},
  // drawing functions
  {"draw2DImage", &RVideoDriver::draw2DImage},
  {"draw2DLine", &RVideoDriver::draw2DLine},
  {"draw2DPolygon", &RVideoDriver::draw2DPolygon},
  {"draw2DRectangle", &RVideoDriver::draw2DRectangle},
  {"draw2DRectangleColor", &RVideoDriver::draw2DRectangleColor},
  {"draw3DBox", &RVideoDriver::draw3DBox},
  {"draw3DLine", &RVideoDriver::draw3DLine},
  {"draw3DTriangle", &RVideoDriver::draw3DTriangle},
  {"drawPixel", &RVideoDriver::drawPixel},
  {"setFog", &RVideoDriver::setFog},
  {"setViewPort", &RVideoDriver::setViewPort},
  // transformation/settings
  {"transformationReset", &RVideoDriver::transformationReset},
  {"transformationSetPosition", &RVideoDriver::transformationSetPosition},
  {"transformationSetRotation", &RVideoDriver::transformationSetRotation},
  {"transformationSetScale", &RVideoDriver::transformationSetScale},
  {"materialSet", &RVideoDriver::materialSet},
  {0}
	};
	const Luna < RVideoDriver >::PropertyType RVideoDriver::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RVideoDriver::RVideoDriver(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RVideoDrivers.  You must collect one from an RDevice.",LVL_ERR,L);
		isVideoDriverSet = false;
	}
	
	int RVideoDriver::setVideoDriver(irr::video::IVideoDriver* vdrv)
	{
		vid=vdrv;
		isVideoDriverSet=true;
		return 0;
	}
	
	irr::video::IVideoDriver* RVideoDriver::getVideoDriver()
	{
		if (isVideoDriverSet)
			return vid;
		else
			return NULL;
	}
	
	int RVideoDriver::beginScene(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		int r = getmandatoryargument_cNumber(L,1);
		int g = getmandatoryargument_cNumber(L,2);
		int b = getmandatoryargument_cNumber(L,3);
		bool clearBackBuffer = getoptionalargument(L, 4, true);
		bool clearZBuffer = getoptionalargument(L, 5, true);
		
		xeffect->setClearColour(irr::video::SColor(255,r,g,b));
		vid->beginScene(clearBackBuffer, clearZBuffer, irr::video::SColor(0,r,g,b));
		return 0;
		
		endifisvalid();
	}
	
	int RVideoDriver::clearZBuffer(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		vid->clearZBuffer();
		return 0;
		
		endifisvalid();
	}

	void RVideoDriver::setCRoketDevice(core::RDevice* rd)
	{
		roketdevice = rd;
	}

	int RVideoDriver::createScreenShot(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		irr::video::IImage* img = vid->createScreenShot();
		irr::core::stringc n = "screenshot";
		n += roketdevice->currentStep;
		irr::video::ITexture* tex;
		if (vid->findTexture(n.c_str()) != NULL)
			tex = vid->findTexture(n.c_str());
		else
			tex = vid->addTexture(n.c_str(),img);

		video::RTexture* rtex = Luna<video::RTexture>::createNew(L);
		rtex->setTexture(tex);
		return 1;
		
		endifisvalid();
	}

	int RVideoDriver::endScene(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		vid->endScene();
		return 0;
		
		endifisvalid();
	}
	
	int RVideoDriver::getFPS(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		int fps = vid->getFPS();
		lua_pushnumber(L,fps);
		return 1;
		
		endifisvalid();
	}
	
	int RVideoDriver::getTexture(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		const char* filename = getmandatoryargument_constchar(L,1);
		if (!fileExists(filename))
		{
			debug(FILE_NOT_FOUND("load texture",filename).c_str(),LVL_ERR,L);
			return 0;
		}
		
		video::RTexture* tex = Luna < video::RTexture >::createNew(L);
		tex->setTexture(vid->getTexture(filename));
		return 1;
		
		endifisvalid();
	}

	void RVideoDriver::setCXEffect(effectHandler* xe)
	{
		xeffect = xe;
	}

	// ===================== TRANSFORMATION MATRIX =======================
	int RVideoDriver::transformationReset(lua_State* L)
	{
		ifistrue(isVideoDriverSet);

		roketdevice->transformation_matrix = irr::core::matrix4();
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::transformationSetPosition(lua_State* L)
	{
		ifistrue(isVideoDriverSet);

		core::RVector3D* transformation = Luna<core::RVector3D>::check(L,1);
		roketdevice->transformation_matrix.setTranslation(transformation->getObject());
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::transformationSetRotation(lua_State* L)
	{
		ifistrue(isVideoDriverSet);

		core::RVector3D* transformation = Luna<core::RVector3D>::check(L,1);
		roketdevice->transformation_matrix.setRotationDegrees(transformation->getObject());
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::transformationSetScale(lua_State* L)
	{
		ifistrue(isVideoDriverSet);

		core::RVector3D* transformation = Luna<core::RVector3D>::check(L,1);
		roketdevice->transformation_matrix.setScale(transformation->getObject());
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::materialSet(lua_State* L)
	{
		ifistrue(isVideoDriverSet);

		video::RMaterial* mat = Luna<video::RMaterial>::check(L,1);
		roketdevice->current_material = mat->getObject();
		return 0;
		
		endifisvalid();
	}

	// ===================== DRAWING FUNCTIONS ===========================
	int RVideoDriver::draw2DImage(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		//showstack(L);

		// Get Variables
		video::RTexture *tex = Luna<video::RTexture>::check(L, 1);
		core::RVector2D *pos = Luna<core::RVector2D>::check(L, 2);

		// Draw
		irr::core::position2di newpos;
		irr::core::position2df oldpos = pos->getObjectAsPosition();
		newpos.X = oldpos.X;
		newpos.Y = oldpos.Y;
		vid->draw2DImage(tex->getTexture(),newpos);

		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::draw2DLine(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RRay2D *line = Luna<core::RRay2D>::check(L, 1);
		core::RColor *col = Luna<core::RColor>::check(L, 2);

		irr::core::line2df irrline = line->getObject();

		// Draw
		irr::core::position2di start;
		irr::core::position2di end;
		start.X = irrline.start.X; start.Y = irrline.start.Y;
		end.X = irrline.end.X; end.Y = irrline.end.Y;
		vid->draw2DLine(start,end,col->getObject());

		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::draw2DPolygon(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RVector2D *center = Luna<core::RVector2D>::check(L, 1);
		cNumber radius = getmandatoryargument_cNumber(L, 2);
		core::RColor *col = Luna<core::RColor>::check(L, 3);
		cNumber vertexcount = getoptionalargument(L, 4, 10);

		// Draw
		irr::core::position2di irrcenter;
		irrcenter.X = center->getObjectDouble().X;
		irrcenter.Y = center->getObjectDouble().Y;
		vid->draw2DPolygon(irrcenter,radius,col->getObject(),vertexcount);

		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::draw2DRectangle(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RRectangle *rectangle = Luna<core::RRectangle>::check(L, 1);
		core::RColor *col = Luna<core::RColor>::check(L, 2);
		core::RRectangle *clip = Luna<core::RRectangle>::lightcheck(L, 3);

		// Draw
		if (clip == NULL)
			vid->draw2DRectangle(col->getObject(),rectangle->getObjectS32(),0);
		else
			vid->draw2DRectangle(col->getObject(),rectangle->getObjectS32(),&clip->getObjectS32());
	
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::draw2DRectangleColor(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RRectangle *rectangle = Luna<core::RRectangle>::check(L, 1);
		core::RColor *col_tl = Luna<core::RColor>::check(L, 2);
		core::RColor *col_tr = Luna<core::RColor>::check(L, 3);
		core::RColor *col_bl = Luna<core::RColor>::check(L, 4);
		core::RColor *col_br = Luna<core::RColor>::check(L, 5);
		core::RRectangle *clip = Luna<core::RRectangle>::lightcheck(L, 6);

		// Draw
		if (clip == NULL)
			vid->draw2DRectangle(rectangle->getObjectS32(),col_tl->getObject(),col_tr->getObject(),col_bl->getObject(),col_br->getObject(),0);
		else
			vid->draw2DRectangle(rectangle->getObjectS32(),col_tl->getObject(),col_tr->getObject(),col_bl->getObject(),col_br->getObject(),&clip->getObjectS32());
	
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::draw3DBox(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RCube3D *cube = Luna<core::RCube3D>::check(L, 1);
		core::RColor *col = Luna<core::RColor>::check(L, 2);

		// Draw
		vid->setMaterial(roketdevice->current_material);
		vid->setTransform(irr::video::ETS_WORLD, roketdevice->transformation_matrix);
		vid->draw3DBox(cube->getObject(),col->getObject());
	
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::draw3DLine(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RRay3D *line = Luna<core::RRay3D>::check(L, 1);
		core::RColor *col = Luna<core::RColor>::check(L, 2);

		// Draw
		vid->setMaterial(roketdevice->current_material);
		vid->setTransform(irr::video::ETS_WORLD, roketdevice->transformation_matrix);
		vid->draw3DLine(line->getObject().start,line->getObject().end,col->getObject());
	
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::draw3DTriangle(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RTriangle3D *triangle = Luna<core::RTriangle3D>::check(L, 1);
		core::RColor *col = Luna<core::RColor>::check(L, 2);

		// Draw
		vid->setMaterial(roketdevice->current_material);
		vid->setTransform(irr::video::ETS_WORLD, roketdevice->transformation_matrix);
		vid->draw3DTriangle(triangle->getObject(),col->getObject());
	
		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::drawPixel(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RVector2D *center = Luna<core::RVector2D>::check(L, 1);
		core::RColor *col = Luna<core::RColor>::check(L, 2);

		// Draw
		vid->drawPixel(center->getObjectDouble().X,center->getObjectDouble().Y,col->getObject());

		return 0;
		
		endifisvalid();
	}

	int RVideoDriver::setFog(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RColor *color = Luna<core::RColor>::check(L, 1);
		bool linearfog = getmandatoryargument_bool(L, 2);
		cNumber start = getmandatoryargument_cNumber(L, 3);
		cNumber end = getmandatoryargument_cNumber(L, 4);
		cNumber density = getmandatoryargument_cNumber(L, 5);
		bool pixelfog = getmandatoryargument_bool(L, 6);
		bool rangefog = getmandatoryargument_bool(L, 7);

		// Draw
		vid->setFog(color->getObject(),linearfog,start,end,density,pixelfog,rangefog);

		return 0;
		
		endifisvalid();
	}
		
	int RVideoDriver::setViewPort(lua_State* L)
	{
		ifistrue(isVideoDriverSet);
		
		// Get Variables
		core::RRectangle *viewport = Luna<core::RRectangle>::check(L, 1);

		// Draw
		irr::core::rect<irr::s32> r = viewport->getObjectS32();
		vid->setViewPort(viewport->getObjectS32());

		return 0;
		
		endifisvalid();
	}
}
