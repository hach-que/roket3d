-- Temporary event stuff..
-- We don't yet have the Engine.Events namespace because there's no high-level
-- framework, so we have define the "type" of events to handle ourselves.
-- Eventually this will be in the high-level framework, so you'll just be able
-- to do device:Bind(Engine.Events.Keyboard.KeyPressed["Left"], ...);
Events = {
	DetectKeyEvent = function(event, code)
		if (event["type"] == 2) then
			if (event["keyboard_code"] == code) then
				return true
			end
		end
		
		return false
	end,
	
	GetKeyEventArgs = function(event)
		return { event["keyboard_code"], event["keyboard_pressed"] }
	end,
	
	Keys = {
		Left = 37,
		Up = 38,
		Right = 39,
		Down = 40
	}
}

Events["Left"] = Engine.Unmanaged.Core.Event(function(event) return Events.DetectKeyEvent(event, Events.Keys.Left); end, Events.GetKeyEventArgs)
Events["Up"] = Engine.Unmanaged.Core.Event(function(event) return Events.DetectKeyEvent(event, Events.Keys.Up); end, Events.GetKeyEventArgs)
Events["Right"] = Engine.Unmanaged.Core.Event(function(event) return Events.DetectKeyEvent(event, Events.Keys.Right); end, Events.GetKeyEventArgs)
Events["Down"] = Engine.Unmanaged.Core.Event(function(event) return Events.DetectKeyEvent(event, Events.Keys.Down); end, Events.GetKeyEventArgs)

-- The main player object.
Player = {
	X = 200,
	Y = 200,
	Color = Engine.Collections.Color(255, 63, 63, 255),
	Width = 32,
	Height = 32,
	Gravity = 0.5,
	VerticalSpeed = 0,
	Movement = {
		Left = false,
		Right = false,
		Up = false
	},
	
	BindKeys = function(device)
		device:Bind(Events.Left, Player.OnLeft);
		device:Bind(Events.Right, Player.OnRight);
		device:Bind(Events.Up, Player.OnUp);
	end,
	
	OnLeft = function(key, pressed)
		-- On keyboard left
		Player.Movement["Left"] = pressed;
	end,
	
	OnRight = function(key, pressed)
		-- On keyboard right
		Player.Movement["Right"] = pressed;
	end,
	
	OnUp = function(key, pressed)
		-- On keyboard up
		Player.Movement["Up"] = pressed;
	end,
	
	HandleStep = function()
		-- Get a rectangle that represents our position.
		rect = Engine.Collections.Rectangle(Player.X, Player.Y, Player.X + Player.Width, Player.Y + Player.Height);
		
		-- Handle left-right movement.
		if (Player.Movement["Left"]) then
			if (not Solids.AtPosition(Player.X - 4, Player.Y + 1) and not Solids.AtPosition(Player.X + 32 - 4, Player.Y + 31)) then
				Player.X = Player.X - 4;
			else
				while (not Solids.AtPosition(Player.X, Player.Y + 1) and not Solids.AtPosition(Player.X, Player.Y + 31)) do
					Player.X = Player.X - 1;
				end
			end
		end
		if (Player.Movement["Right"]) then
			if (not Solids.AtPosition(Player.X + 32 + 4, Player.Y + 1) and not Solids.AtPosition(Player.X + 32 + 4, Player.Y + 31)) then
				Player.X = Player.X + 4;
			else
				while (not Solids.AtPosition(Player.X + 32, Player.Y + 1) and not Solids.AtPosition(Player.X + 32, Player.Y + 31)) do
					Player.X = Player.X + 1;
				end
			end
		end
		if (Player.Movement["Up"]) then
			if (Player.Contacting(0)) then
				Player.VerticalSpeed = -10;
			end
		end
		
		-- Handle gravity.
		Player.VerticalSpeed = Player.VerticalSpeed + Player.Gravity;
		if (Player.VerticalSpeed > 12) then
			Player.VerticalSpeed = 12;
		end
		
		-- Check all of the solid platform objects to see if we're colliding.
		if (not Player.Contacting(Player.VerticalSpeed) and not Player.Contacting(-33 + Player.VerticalSpeed)) then
			-- Move down.
			Player.Y = Player.Y + Player.VerticalSpeed
		elseif (not Player.Contacting(-33 + Player.VerticalSpeed)) then
			-- Move as far down as we can.
			Player.VerticalSpeed = 0;
			while (not Player.Contacting(0)) do
				Player.Y = Player.Y + 1
			end
		elseif (Player.Contacting(-33 + Player.VerticalSpeed)) then
			-- Stop moving upward!
			Player.VerticalSpeed = Player.Gravity;
			while (not Player.Contacting(-33)) do
				Player.Y = Player.Y - 1
			end
		end
	end,
	
	Contacting = function(offset)
		if (not Solids.AtPosition(Player.X + 1, Player.Y + 32 + offset) and
		    not Solids.AtPosition(Player.X + 31, Player.Y + 32 + offset)) then
		    return false
		end
		
		return true
	end
}

-- A table which manages all of the solid objects.
Solids = {
	GenerateBorder = function()
		-- Generate a border.
		for x = 0, 640 - 32, 32 do
			Solids[#Solids + 1] = {x, 0}
		end
		for x = 0, 640 - 32, 32 do
			Solids[#Solids + 1] = {x, 480 - 32}
		end
		for y = 32, 480 - 64, 32 do
			Solids[#Solids + 1] = {0, y}
		end
		for y = 32, 480 - 64, 32 do
			Solids[#Solids + 1] = {640 - 32, y}
		end
	end,
	
	GenerateStairs = function()
		-- Generate the stairs.
		for i = 0, 4, 1 do
			Solids[#Solids + 1] = {128 + i * 32, 480 - 96 - i * 32}
		end
	end,
	
	Render = function(driver)
		-- Render the solids.
		for k, v in pairs(Solids) do
			if (type(v) == "table") then
				-- Coordinate pair.
				device.VideoDriver:Draw2DRectangle(
					Engine.Collections.Rectangle(v[1], v[2], v[1] + 32, v[2] + 32),
					Engine.Collections.Color(255, 0, 0, 0)
					);
			end
		end
	end,
	
	AtPosition = function(x, y)
		-- Check to see if there's a solid at the specified position.
		for k, v in pairs(Solids) do
			if (type(v) == "table") then
				if (x >= v[1] and x <= v[1] + 32 and y >= v[2] and y <= v[2] + 32) then
					return true
				end
			end
		end
		
		return false
	end
}

-- Generate the border of solids.
Solids.GenerateBorder();
Solids.GenerateStairs();

-- Run the game.
function Run()
    device = Engine.Unmanaged.Core.Device(
        Engine.Collections.Vector2D(640, 480),
        16, false, true, false);
	Player.BindKeys(device)
    
    backcolor = Engine.Collections.Color(255, 223, 223, 255)
	
    while (device.Running) do
        Player.HandleStep()
	
        device.VideoDriver:BeginScene(backcolor, true, true);
        
		-- Render our game.
		device.VideoDriver:Draw2DRectangle(Engine.Collections.Rectangle(Player.X, Player.Y, Player.X + Player.Width, Player.Y + Player.Height), Player.Color);
		Solids.Render(device.VideoDriver);
		
        device.VideoDriver:EndScene();
    end
    
    device:Close()
end




