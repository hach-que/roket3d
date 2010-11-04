name "Ext.IDE.Model"
description [[Encapsulates a model resource defined in the IDE for use within game code.]]
author "Roket Enterprises"
lastmodified "21st May, 2010"

function Model:__init(smgr, path)
	self.mesh = smgr:getMesh(path)
	self.smgr = smgr
end

function Model:getMesh()
	return self.mesh
end
