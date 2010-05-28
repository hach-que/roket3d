name "Ext.IDE.Audio"
description [[Encapsulates an audio resource defined in the IDE for use within game code.]]
author "Roket Enterprises"
lastmodified "21st May, 2010"

function Audio:__init(aud, path, id)
	self.id = id
	self.audio = aud:createFromFile(id, path, false)
	self.controller = aud
end

function Audio:getAudio()
	return self.audio
end
