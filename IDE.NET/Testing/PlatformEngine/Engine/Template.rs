class "Engine.Template"

-- Mark our variables as per-instance.
context
{
	Position = instance,
	Speed = instance,
	Gravity = instance
}

-- Our movement and positioning variables
X = 0
Y = 0
Speed = Engine.Collections.Vector2D(0, 0)
Gravity = Engine.Collections.Vector2D(0, 0)

function Template:__init()
end

function Template:__step()
end

function Template:__draw()
end
