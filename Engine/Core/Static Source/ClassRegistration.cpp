// This file registers all of the available built-in classes
// with the engine.
#include "ClassRegistration.h"
#include "autobind/types.h"
#include "autobind/binding/lua.h"

// Include all of the classes to be registered.
#include "E.Collections/RColor.h"
#include "E.Collections/RCube3D.h"
#include "E.Collections/RMatrix.h"
#include "E.Collections/RPlane.h"
#include "E.Collections/RQuaternion.h"
#include "E.Collections/RLine2D.h"
#include "E.Collections/RLine3D.h"
#include "E.Collections/RRectangle.h"
#include "E.Collections/RTriangle3D.h"
#include "E.Collections/RVector2D.h"
#include "E.Collections/RVector3D.h"
#include "E.Unmanaged/Audio/RAudioController.h"
#include "E.Unmanaged/Audio/RAudioSound.h"
#include "E.Unmanaged/Core/RCursorControl.h"
#include "E.Unmanaged/Core/RDevice.h"
#include "E.Unmanaged/Core/REvent.h"
//#include "E.Unmanaged/Ext/RThread.h"
#include "E.Unmanaged/GUI/RGUIButton.h"
#include "E.Unmanaged/GUI/RGUICheckBox.h"
#include "E.Unmanaged/GUI/RGUIComboBox.h"
#include "E.Unmanaged/GUI/RGUIEditBox.h"
#include "E.Unmanaged/GUI/RGUIElement.h"
#include "E.Unmanaged/GUI/RGUIEnvironment.h"
#include "E.Unmanaged/GUI/RGUIFileOpenDialog.h"
#include "E.Unmanaged/GUI/RGUIFont.h"
#include "E.Unmanaged/GUI/RGUIImage.h"
#include "E.Unmanaged/GUI/RGUIListBox.h"
#include "E.Unmanaged/GUI/RGUIMeshViewer.h"
#include "E.Unmanaged/GUI/RGUIScrollBar.h"
#include "E.Unmanaged/GUI/RGUISkin.h"
#include "E.Unmanaged/GUI/RGUIStaticText.h"
#include "E.Unmanaged/GUI/RGUIWindow.h"
#include "E.Unmanaged/Net/RNetSocket.h"
#include "E.Unmanaged/Physics/RPhysicsJoint.h"
#include "E.Unmanaged/Physics/RPhysicsManager.h"
#include "E.Unmanaged/Physics/RPhysicsMaterialHandler.h"
#include "E.Unmanaged/Scene/RParticleSystemNode.h"
#include "E.Unmanaged/Scene/RSceneBillboardNode.h"
#include "E.Unmanaged/Scene/RSceneBillboardTextNode.h"
#include "E.Unmanaged/Scene/RSceneCamera.h"
#include "E.Unmanaged/Scene/RSceneCollisionManager.h"
#include "E.Unmanaged/Scene/RSceneLazyCamera.h"
#include "E.Unmanaged/Scene/RSceneLightNode.h"
#include "E.Unmanaged/Scene/RSceneManager.h"
#include "E.Unmanaged/Scene/RSceneMesh.h"
#include "E.Unmanaged/Scene/RSceneMeshNode.h"
#include "E.Unmanaged/Scene/RSceneNode.h"
#include "E.Unmanaged/Scene/RSceneSpecialEffects.h"
#include "E.Unmanaged/Scene/RSceneTerrainNode.h"
#include "E.Unmanaged/Video/RVideoDriver.h"
#include "E.Unmanaged/Video/RVideoMaterial.h"
#include "E.Unmanaged/Video/RVideoTexture.h"

#include "AccessViolationException.h"
#include "ArgumentCountMismatchException.h"
#include "ArgumentTypeNotValidException.h"
#include "ContextNotProvidedException.h"
#include "DataMisalignedException.h"
#include "DebuggerNotAttachedException.h"
#include "DivideByZeroException.h"
#include "EngineException.h"
#include "EntryPointNotFoundException.h"
#include "Exception.h"
#include "GraphicsEngineNotFoundException.h"
#include "IllegalInstructionException.h"
#include "InheritedClassNotFoundException.h"
#include "InternalConstructorOnlyException.h"
#include "InterpreterException.h"
#include "InterpreterStateNotValidException.h"
#include "InvalidObjectThrownException.h"
#include "InvalidOperationException.h"
#include "InvalidSyntaxException.h"
#include "KeyNotFoundException.h"
#include "LowLevelException.h"
#include "NilReferenceException.h"
#include "ObjectDisposedException.h"
#include "OutOfMemoryException.h"
#include "OverflowException.h"
#include "PathNotFoundException.h"
#include "PermissionDeniedException.h"
#include "PrivilegedInstructionException.h"
#include "ReadOnlyPropertyException.h"
#include "UnderflowException.h"

namespace Roket3D
{
	void RegisterAllClasses(lua_State * L)
	{
		Bindings<Engine::Collections::RColor>::Register(L);
		Bindings<Engine::Collections::RCube3D>::Register(L);
		Bindings<Engine::Collections::RMatrix>::Register(L);
		Bindings<Engine::Collections::RPlane>::Register(L);
		Bindings<Engine::Collections::RQuaternion>::Register(L);
		Bindings<Engine::Collections::RLine2D>::Register(L);
		Bindings<Engine::Collections::RLine3D>::Register(L);
		Bindings<Engine::Collections::RRectangle>::Register(L);
		Bindings<Engine::Collections::RTriangle3D>::Register(L);
		Bindings<Engine::Collections::RVector2D>::Register(L);
		Bindings<Engine::Collections::RVector3D>::Register(L);
		Bindings<Engine::Unmanaged::Audio::RAudioController>::Register(L);
		Bindings<Engine::Unmanaged::Audio::RAudioSound>::Register(L);
		Bindings<Engine::Unmanaged::Core::RCursorControl>::Register(L);
		Bindings<Engine::Unmanaged::Core::RDevice>::Register(L);
		Bindings<Engine::Unmanaged::Core::REvent>::Register(L);
//		Bindings<Engine::Unmanaged::Ext::RThread>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIButton>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUICheckBox>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIComboBox>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIEditBox>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIElement>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIEnvironment>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIFileOpenDialog>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIFont>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIImage>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIListBox>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIMeshViewer>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIScrollBar>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUISkin>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIStaticText>::Register(L);
		Bindings<Engine::Unmanaged::GUI::RGUIWindow>::Register(L);
		Bindings<Engine::Unmanaged::Net::RNetSocket>::Register(L);
		Bindings<Engine::Unmanaged::Physics::RPhysicsJoint>::Register(L);
		Bindings<Engine::Unmanaged::Physics::RPhysicsManager>::Register(L);
		Bindings<Engine::Unmanaged::Physics::RPhysicsMaterialHandler>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RParticleSystemNode>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneBillboardNode>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneBillboardTextNode>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneCamera>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneCollisionManager>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneLazyCamera>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneLightNode>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneManager>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneMesh>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneMeshNode>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneNode>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneSpecialEffects>::Register(L);
		Bindings<Engine::Unmanaged::Scene::RSceneTerrainNode>::Register(L);
		Bindings<Engine::Unmanaged::Video::RVideoDriver>::Register(L);
		Bindings<Engine::Unmanaged::Video::RVideoMaterial>::Register(L);
		Bindings<Engine::Unmanaged::Video::RVideoTexture>::Register(L);

		// Register the exception classes.  Please note that if they
		// aren't registered when they are fired, they won't be properly
		// accessible because the RaiseException function of the Lua
		// bindings relies on the entry being in the Lua registry.
		Bindings<Engine::AccessViolationException>::Register(L);
		Bindings<Engine::ArgumentCountMismatchException>::Register(L);
		Bindings<Engine::ArgumentTypeNotValidException>::Register(L);
		Bindings<Engine::ContextNotProvidedException>::Register(L);
		Bindings<Engine::DataMisalignedException>::Register(L);
		Bindings<Engine::DebuggerNotAttachedException>::Register(L);
		Bindings<Engine::DivideByZeroException>::Register(L);
		Bindings<Engine::EngineException>::Register(L);
		Bindings<Engine::EntryPointNotFoundException>::Register(L);
		Bindings<Engine::Exception>::Register(L);
		Bindings<Engine::GraphicsEngineNotFoundException>::Register(L);
		Bindings<Engine::IllegalInstructionException>::Register(L);
		Bindings<Engine::InheritedClassNotFoundException>::Register(L);
		Bindings<Engine::InternalConstructorOnlyException>::Register(L);
		Bindings<Engine::InterpreterException>::Register(L);
		Bindings<Engine::InterpreterStateNotValidException>::Register(L);
		Bindings<Engine::InvalidObjectThrownException>::Register(L);
		Bindings<Engine::InvalidOperationException>::Register(L);
		Bindings<Engine::InvalidSyntaxException>::Register(L);
		Bindings<Engine::KeyNotFoundException>::Register(L);
		Bindings<Engine::LowLevelException>::Register(L);
		Bindings<Engine::NilReferenceException>::Register(L);
		Bindings<Engine::ObjectDisposedException>::Register(L);
		Bindings<Engine::OutOfMemoryException>::Register(L);
		Bindings<Engine::OverflowException>::Register(L);
		Bindings<Engine::PathNotFoundException>::Register(L);
		Bindings<Engine::PermissionDeniedException>::Register(L);
		Bindings<Engine::PrivilegedInstructionException>::Register(L);
		Bindings<Engine::ReadOnlyPropertyException>::Register(L);
		Bindings<Engine::UnderflowException>::Register(L);
	}
}