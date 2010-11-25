class "Game.Areas.Main"
inherits "Engine.Area"

require "Game.Player"
require "Game.Solid"

-- Set the background color.
BackgroundColor = Engine.Collections.Color(255, 223, 223, 255)

function Main:__init()
	-- Add our player.
	player = Game.Player(device)
	self.Objects[#self.Objects] = player

	-- Add our solids.
	Game.Solid.GenerateBorder(self)
	Game.Solid.GenerateStairs(self)
end
