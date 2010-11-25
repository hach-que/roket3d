class "Game.Player"
inherits "Engine.Actor"

require "Game.Solid"

-- Mark our variables as per-instance.
context
{
	Movement = instance,
	Width = instance,
	Height = instance,
	Color = instance
}

-- Define our movement array which holds the state
-- of keyboard keys.
Movement = {
	Left = false,
	Right = false,
	Up = false
}

-- Basic collision stuff
Width = 32
Height = 32
Color = Engine.Collections.Color(255, 64, 64, 64)

-- Override the default position and gravity settings
X = 200
Y = 200
Gravity = Engine.Collections.Vector2D(0, 0.5)

function Player:__init(device)
	base.__init(self)

	device:Bind(Engine.Events.Keyboard.Held["Left"], function(pressed) self:OnLeft(pressed) end);
	device:Bind(Engine.Events.Keyboard.Held["Right"], function(pressed) self:OnRight(pressed) end);
	device:Bind(Engine.Events.Keyboard.Held["Up"], function(pressed) self:OnUp(pressed) end);
end

function Player:OnLeft(pressed)
	-- On keyboard left
	self.Movement["Left"] = pressed;
end

function Player:OnRight(pressed)
	-- On keyboard right
	self.Movement["Right"] = pressed;
end

function Player:OnUp(pressed)
	-- On keyboard up
	self.Movement["Up"] = pressed;
end

function Player:Contacting(offset)
	if (not self:AtPosition(self.X + 1, self.Y + 32 + offset) and
		not self:AtPosition(self.X + 31, self.Y + 32 + offset)) then
		return false
	end
	
	return true
end

function Player:AtPosition(x, y)
	return Game.Solid.AtPosition(self.Area, x, y)
end

function Player:__step()
	-- Get the template to handle movement and collisions.
	base.__step(self)

	-- Get a rectangle that represents our position.
	rect = Engine.Collections.Rectangle(self.X, self.Y, self.X + self.Width, self.Y + self.Height);
	
	-- Handle left-right movement.
	if (self.Movement["Left"]) then
		if (not self:AtPosition(self.X - 4, self.Y + 1) and not self:AtPosition(self.X + 32 - 4, self.Y + 31)) then
			self.X = self.X - 4;
		else
			while (not self:AtPosition(self.X, self.Y + 1) and not self:AtPosition(self.X, self.Y + 31)) do
				self.X = self.X - 1;
			end
		end
	end
	if (self.Movement["Right"]) then
		if (not self:AtPosition(self.X + 32 + 4, self.Y + 1) and not self:AtPosition(self.X + 32 + 4, self.Y + 31)) then
			self.X = self.X + 4;
		else
			while (not self:AtPosition(self.X + 32, self.Y + 1) and not self:AtPosition(self.X + 32, self.Y + 31)) do
				self.X = self.X + 1;
			end
		end
	end
	if (self.Movement["Up"]) then
		if (self:Contacting(0)) then
			self.Speed.Y = -10;
		end
	end
	
	-- Handle gravity.
	self.Speed.Y = self.Speed.Y + self.Gravity.Y;
	if (self.Speed.Y > 12) then
		self.Speed.Y = 12;
	end
	
	-- Check all of the solid platform objects to see if we're colliding.
	if (not self:Contacting(self.Speed.Y) and not self:Contacting(-33 + self.Speed.Y)) then
		-- Move down.
		self.Y = self.Y + self.Speed.Y
	elseif (not self:Contacting(-33 + self.Speed.Y)) then
		-- Move as far down as we can.
		self.Speed.Y = 0;
		while (not self:Contacting(0)) do
			self.Y = self.Y + 1
		end
	elseif (self:Contacting(-33 + self.Speed.Y)) then
		-- Stop moving upward!
		self.Speed.Y = self.Gravity.Y;
		while (not self:Contacting(-33)) do
			self.Y = self.Y - 1
		end
	end
end

function Player:__draw(device)
	device.VideoDriver:Draw2DRectangle(Engine.Collections.Rectangle(self.X, self.Y, self.X + self.Width, self.Y + self.Height), self.Color);
end
