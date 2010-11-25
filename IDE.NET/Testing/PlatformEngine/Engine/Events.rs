class "Engine.Events"

-- Tell the class system what functions and variables
-- are static.
context
{
	DetectKeyEvent = static,
	GetKeyEventArgs = static,
	Keyboard = static
}

-- Tell the system what functions and variables have
-- restricted access.
visibility
{
	DetectKeyEvent = private,
	GetKeyEventArgs = private,
	Keyboard = private
}

-- This function is called by the engine to determine whether a specific
-- event should be handled.
function Events.DetectKeyEvent(event, code, mode)
	if (event["type"] == 2) then
		if (event["keyboard_code"] == code) then
			if (mode == "pressed" and event["keyboard_pressed"]) then
				return true
			elseif (mode == "released" and not event["keyboard_released"]) then
				return true
			elseif (mode == "held") then
				return true
			end
		end
	end
	
	return false
end

-- These functions defines what arguments should be passed to the event handling
-- function by defining a table of arguments that should be passed.  The event
-- argument that is passed to this function is a table of values as laid out
-- in the event specification.
function Events.GetKeyEmptyEventArgs(event)
	return { }
end
function Events.GetKeyPressedEventArgs(event)
	return { event["keyboard_pressed"] }
end

-- Define our set of events for this static class (emulates a namespace, but one
-- that is defined in this file).
Keyboard = {

	-- Keyboard press detection.  Used like Pressed["Left"], hence
	-- it is a table with the __index metamethod.
	Pressed = setmetatable({}, {

		__index = function(tbl, key)
			if (class.Keyboard.Keys[key] ~= nil) then
				return Engine.Unmanaged.Core.Event(
					function(event) return class.DetectKeyEvent(event, class.Keyboard.Keys[key], "pressed"); end,
					class.GetKeyEmptyEventArgs
					);
			else
				return nil
			end
		end

	}),

	-- Keyboard release detection.  Used like Released["Left"], hence
	-- it is a table with the __index metamethod.
	Released = setmetatable({}, {

		__index = function(tbl, key)
			if (class.Keyboard.Keys[key] ~= nil) then
				return Engine.Unmanaged.Core.Event(
					function(event) return class.DetectKeyEvent(event, class.Keyboard.Keys[key], "released"); end,
					class.GetKeyEmptyEventArgs
					);
			else
				return nil
			end
		end

	}),

	-- Keyboard key hold detection.  Used like Held["Left"], hence
	-- it is a table with the __index metamethod.
	Held = setmetatable({}, {

		__index = function(tbl, key)
			if (Engine.Events.Keyboard.Keys[key] ~= nil) then
				return Engine.Unmanaged.Core.Event(
					function(event) return class.DetectKeyEvent(event, class.Keyboard.Keys[key], "held"); end,
					class.GetKeyPressedEventArgs
					);
			else
				return nil
			end
		end

	}),

	-- A list of string values to internal engine codes.  For internal
	-- use only.
	Keys = {
		Left = 37,
		Up = 38,
		Right = 39,
		Down = 40
	}
}


