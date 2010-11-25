class "Engine.Model"

function Model:__init(smgr, path)
	self.mesh = smgr:getMesh(path)
	self.smgr = smgr
end

function Model:getMesh()
	return self.mesh
end

