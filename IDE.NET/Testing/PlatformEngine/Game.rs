-- Load the bootstrap
require "_Unmanaged/Bootstrap"

-- Load our classes
require "Game.Areas.Main"

-- Run the game.
function Run()
	-- Create the game device.
	device = Engine.Unmanaged.Core.Device(
		Engine.Collections.Vector2D(640, 480),
		16, false, true, false)
	Engine.Area.Device = device
	print(Engine.Area.Device)

	-- Create the game area.
	main = Game.Areas.Main()

	-- Run the area.
	Engine.Area.Start(main)

	-- Close the device.
	device:Close()
end


