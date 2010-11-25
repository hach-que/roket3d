class "Engine.Area"

-- The context of variables.
context
{
	Device = static,
	CurrentArea = static,
	Active = instance,
	Objects = instance,
	BackgroundColor = instance
}

-- A variable which indicates whether this is the currently active area.
Device = nil
CurrentArea = nil
Active = false
Objects = {}
BackgroundColor = Engine.Collections.Color(255, 127, 127, 127)

function Area:__init()
end

function Area:Run()
	while (device.Running and self.Active) do
		device.VideoDriver:BeginScene(self.BackgroundColor, true, true);

		-- Call the step event for each of our objects.
		for k, v in pairs(self.Objects) do
			if (v is Engine.Template) then
				v.Area = self
				v:__step()
			end
		end

		-- Call the draw event for each of our objects.
		for k, v in pairs(self.Objects) do
			if (v is Engine.Template) then
				v.Area = self
				v:__draw(Engine.Area.Device)
			end
		end
			
		device.VideoDriver:EndScene();
	end
end

function Area.Start(area)
	area.Active = true
	class.CurrentArea = area
	area:Run(class.Device)
end

function Area.Goto(area)
	if (class.CurrentArea ~= nil) then
		class.CurrentArea.Active = false
	end
	area.Active = true
	class.CurrentArea = area
	area:Run(class.Device)
end
