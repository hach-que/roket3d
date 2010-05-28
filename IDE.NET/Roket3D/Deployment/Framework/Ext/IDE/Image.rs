name "Ext.IDE.Image"
description [[Encapsulates an image resource defined in the IDE for use within game code.]]
author "Roket Enterprises"
lastmodified "21st May, 2010"

function Image:__init(vid, path)
	self.tex = vid:getTexture(path)
	self.vid = vid
end

function Image:getTexture()
	return self.tex
end
