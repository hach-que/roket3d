class "Game.Models.D3DModel"
inherits "Engine.Model"

function D3DModel:__init(device, texture)
	base.__init(self, device, texture)
end

function D3DModel:AddFrame(path)
	local f = IO.Open(path)
	local unknown, count
	local frame = self.Mesh:AddFrame()
	local meshbuf = nil
	local white = Engine.Collections.Color(255, 255, 255, 255)

	unknown = f:Read()
	count = tonumber(f:Read())

	for line in f:Lines(path) do
		-- Split the string up
		local l = {}
		local buffer = ""
		for i = 1, line:Length() do
			if (line:Substring(i, i) == " ") then
				l[#l + 1] = buffer
				buffer = ""
			else
				buffer = buffer .. line:Substring(i, i)
			end
		end
		if (#buffer > 0) then
			l[#l + 1] = tonumber(buffer)
			buffer = ""
		end
		
		-- Read what to do
		if (l[0] == 0) then
			-- Start primitive
			meshbuf = Engine.Unmanaged.Scene.MeshBuffer()
		elseif (l[1] == 1) then
			-- End primitive
			frame:AddMeshBuffer(meshbuf)
		elseif (l[8] == 8) then
			-- Vertex
			meshbuf:Append(
			     -- x,    y,    z,    nx,   ny,   nz,   col,   tu,   tv,   tu2,  tv2
				    l[1], l[2], l[3], l[6], l[7], l[8], white, l[4], l[5], l[9], l[10]
			);
		end
	end
	
	print("Loaded model: " .. path)
	
	return frame
end








