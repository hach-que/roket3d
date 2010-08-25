#ifndef HEADER_RSCENENODE
#define HEADER_RSCENENODE

namespace physics
{
	class Roket_PhysicsBody;
}

namespace core
{
	class RDevice;
}

namespace scene
{
	class RSceneNode : public RObject
	{
		// Newton Physics Objects
		public:
			ePhysicsType physType;
			physics::Roket_PhysicsBody* physBody;
			core::RDevice* roketdevice;

		protected:
			effectHandler* xeffect;
		
		public:
			safeVP ident;
			bool isNodeSet;
		
		// Physics thingos
		private:
			irr::scene::IMeshBuffer *mb;
			irr::scene::IDynamicMeshBuffer* ter_buffer;
			irr::video::S3DVertex* mb_vertices_normal;
			irr::video::S3DVertex2TCoords* mb_vertices_2t;
			irr::video::S3DVertexTangents* mb_vertices_tangent;
			int test_mbc;
			bool has_indicies;
			int total_indicies, total_vertexes;
			irr::u16 *mb_indices; 
			float vertarray[9];
			irr::core::matrix4 posdesc;
			NewtonCollision* collision;
			/*int cMeshBuffer, j;
			int v1i, v2i, v3i;
			irr::scene::IMesh* singleframe;
			float vArray[9]; // vertex array (3*3 floats)
			int tmpCount;
			int totalbuffers;
			irr::scene::IMeshBuffer* meshbuffer;
			irr::video::S3DVertex2TCoords* mb_vertices;
			irr::u32 mb_vertices_total;
			float newvertex[9][3];
			int totalindices;
			irr::core::vector3df p[9];*/

		public:
			irr::scene::IMeshSceneNode* base_self;
			virtual irr::scene::ISceneNode *&self() { return (irr::scene::ISceneNode*&)base_self; };

			RSceneNode(lua_State* L, bool instantedByUser);
			RSceneNode();
			~RSceneNode();
			//int drawAll(lua_State* L);
			int setPosition(lua_State* L);
			int setRotation(lua_State* L);
			int getPosition(lua_State* L);
			int getRotation(lua_State* L);
			virtual void setNode(irr::scene::ISceneNode* node, effectHandler* xe);
			virtual irr::scene::ISceneNode* getNode();
			virtual void setNodeAdvanced(irr::scene::IMeshSceneNode* node, effectHandler* xe);
			virtual irr::scene::IMeshSceneNode* getNodeAdvanced();
			int applyTexture(lua_State* L);
			int applyFlag(lua_State* L);
			int applyMaterialType(lua_State* L);
			int applyPhysicsSettings(lua_State* L);
			int applyForce(lua_State* L);
			int applyTorque(lua_State* L);
			int setParent(lua_State* L);
			int destroy(lua_State* L);
			int destroyPhysics(lua_State* L);
			int setName(lua_State* L);
			int getName(lua_State* L);
			int setID(lua_State* L);
			int getID(lua_State* L);
			
			void setRoketDevice(core::RDevice* rd);

			// Physics functions
			void setupPhysics(ePhysicsType type, NewtonWorld* physworld, irr::core::vector3df position, irr::scene::IMesh* mesh, irr::scene::ITerrainSceneNode* terrain = 0);
			
			static const char *className;
			static const Luna < RSceneNode >::FunctionType Functions[];
			static const Luna < RSceneNode >::PropertyType Properties[];
	};
}
#endif
