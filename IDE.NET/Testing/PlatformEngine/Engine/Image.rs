class "Engine.Image"

function Image:__init(vid, path)
	self.tex = vid:getTexture(path)
	self.vid = vid
end

function Image:getTexture()
	return self.tex
end

