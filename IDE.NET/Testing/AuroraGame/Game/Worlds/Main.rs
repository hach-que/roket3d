class "Game.Worlds.Main"
inherits "Engine.World"

require "Game.Player"

-- Set the background color.
BackgroundColor = Engine.Collections.Color(255, 223, 223, 255)

function Main:__init()
	-- Add our player.
	player = Game.Player(Engine.World.Device)
	self.Objects[#self.Objects] = player
end

