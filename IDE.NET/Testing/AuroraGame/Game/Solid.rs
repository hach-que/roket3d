class "Game.Solid"
inherits "Engine.Template"

function Solid:__init(x, y)
	self.__base(self)

	self.Position = Engine.Collections.Vector2D(x, y)
end

function Solid.GenerateBorder(area)
	-- Generate a border.
	for x = 0, 640 - 32, 32 do
		area.Objects[#area.Objects + 1] = Game.Solid(x, 0)
	end
	for x = 0, 640 - 32, 32 do
		area.Objects[#area.Objects + 1] = Game.Solid(x, 480 - 32)
	end
	for y = 32, 480 - 64, 32 do
		area.Objects[#area.Objects + 1] = Game.Solid(0, y)
	end
	for y = 32, 480 - 64, 32 do
		area.Objects[#area.Objects + 1] = Game.Solid(640 - 32, y)
	end
end

function Solid.GenerateStairs(area)
	-- Generate the stairs.
	for i = 0, 4, 1 do
		area.Objects[#area.Objects + 1] = Game.Solid(128 + i * 32, 480 - 96 - i * 32)
	end
end

function Solid:__draw(device)
	device.VideoDriver:Draw2DRectangle(
		Engine.Collections.Rectangle(self.Position.X, self.Position.Y, self.Position.X + 32, self.Position.Y + 32),
		Engine.Collections.Color(255, 0, 0, 0)
		);
end
	
function Solid.AtPosition(area, x, y)
	-- Check to see if there's a solid at the specified position.
	for k, v in pairs(area.Objects) do
		if (v is Game.Solid) then
			if (x >= v.Position.X and x <= v.Position.X + 32 and y >= v.Position.Y and y <= v.Position.Y + 32) then
				return true
			end
		end
	end
	
	return false
end


