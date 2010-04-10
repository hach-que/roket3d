#ifndef HEADER_RVIDEODRIVER
#define HEADER_RVIDEODRIVER
namespace video
{
	class RVideoDriver : public RObject
	{
		irr::video::IVideoDriver * vid;
		effectHandler* xeffect;
		core::RDevice* roketdevice;

		public:
			RVideoDriver(lua_State* L, bool instantedByUser);
			int setVideoDriver(irr::video::IVideoDriver* enviro);
			void setCRoketDevice(core::RDevice* rd);
			irr::video::IVideoDriver* getVideoDriver();
			bool isVideoDriverSet;

			// transformation_matrix
			int transformationReset(lua_State* L);
			int transformationSetPosition(lua_State* L);
			int transformationSetRotation(lua_State* L);
			int transformationSetScale(lua_State* L);
			int materialSet(lua_State* L);

// 			int addDynamicLight(lua_State* L);
// 			int addExternalImageLoader(lua_State* L);
// 			int addExternalImageWriter(lua_State* L);
// 			int addMaterialRenderer(lua_State* L);
// 			int addTexture(lua_State* L);
			int beginScene(lua_State* L);
 			int clearZBuffer(lua_State* L);
// 			int createAttributesFromMaterial(lua_State* L);
// 			int createImageFromData(lua_State* L);
// 			int createRenderTargetTexture(lua_State* L);
 			int createScreenShot(lua_State* L);
// 			int deleteAllDynamicLights(lua_State* L);
 			int draw2DImage(lua_State* L);
 			int draw2DLine(lua_State* L);
 			int draw2DPolygon(lua_State* L);
 			int draw2DRectangle(lua_State* L);
			int draw2DRectangleColor(lua_State* L);
 			int draw3DBox(lua_State* L);
 			int draw3DLine(lua_State* L);
 			int draw3DTriangle(lua_State* L);
			int drawPixel(lua_State* L);
// 			int drawIndexedTriangleFan(lua_State* L);
// 			int drawIndexedTriangleList(lua_State* L);
// 			int drawMeshBuffer(lua_State* L);
// 			int drawStencilShadow(lua_State* L);
// 			int drawStencilShadowVolume(lua_State* L);
// 			int drawVertexPrimitiveList(lua_State* L);
// 			int enableClipPlane(lua_State* L);
			int endScene(lua_State* L);
// 			int fillMaterialStructureFromAttributes(lua_State* L);
// 			int findTexture(lua_State* L);
// 			int getCurrentRenderTargetSize(lua_State* L);
// 			int getDynamicLight(lua_State* L);
// 			int getDynamicLightCount(lua_State* L);
// 			int getExposedVideoData(lua_State* L);
			int getFPS(lua_State* L);
// 			int getGPUProgrammingServices(lua_State* L);
// 			int getMaterialRenderer(lua_State* L);
// 			int getMaterialRendererCount(lua_State* L);
// 			int getMaterialRendererName(lua_State* L);
// 			int getMaximalDynamicLightAmount(lua_State* L);
// 			int getMaximalPrimitiveCount(lua_State* L);
// 			int getName(lua_State* L);
// 			int getPrimitiveCountDrawn(lua_State* L);
// 			int getScreenSize(lua_State* L);
			int getTexture(lua_State* L);
// 			int getTextureByIndex(lua_State* L);
// 			int getTextureCount(lua_State* L);
// 			int getTextureCreationFlag(lua_State* L);
// 			int getTransform(lua_State* L);
// 			int getViewPort(lua_State* L);
// 			int makeColorKeyTexture(lua_State* L);
// 			int makeNormalMapTexture(lua_State* L);
// 			int queryFeature(lua_State* L);
 			int removeAllTextures(lua_State* L);
 			int removeTexture(lua_State* L);
 			int renameTexture(lua_State* L);
// 			int setAmbientLight(lua_State* L);
// 			int setClipPlane(lua_State* L);
 			int setFog(lua_State* L);
// 			int setMaterial(lua_State* L);
// 			int setMaterialRendererName(lua_State* L);
// 			int setRenderTarget(lua_State* L);
// 			int setTextureCreationFlag(lua_State* L);
// 			int setTransform(lua_State* L);
 			int setViewPort(lua_State* L);
// 			int writeImageToFile(lua_State* L);

			void setCXEffect(effectHandler* xe);

			static const char *className;
			static const Luna < RVideoDriver >::FunctionType Functions[];
			static const Luna < RVideoDriver >::PropertyType Properties[];
	};
}
#endif
