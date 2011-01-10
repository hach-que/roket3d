-- Load the bootstrap
require "_Unmanaged/Bootstrap"

-- Load our classes
require "Game.Worlds.Main"

-- Run the game.
function Run()
	-- Create the game device.
	device = Engine.Unmanaged.Core.Device(
		Engine.Collections.Vector2D(640, 480),
		16, false, true, false)
	Engine.World.Device = device

	-- Create the game world.
	main = Game.Worlds.Main()

	-- Create the camera.  This should probably be
	-- done by the World itself.
	main.Camera = device.SceneManager:AddCameraSceneNodeFPS();
	--	nil,
	--	Engine.Collections.Vector3D(50, 50, 50),
	--	Engine.Collections.Vector3D(0, 0, 0)
	--	);

	-- Run the world.
	Engine.World.Start(main)

	-- Close the device.
	device:Close()
end



