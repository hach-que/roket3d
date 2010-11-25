class "Engine.Audio"

function Audio:__init(aud, path, id)
	self.id = id
	self.audio = aud:createFromFile(id, path, false)
	self.controller = aud
end

function Audio:getAudio()
	return self.audio
end

