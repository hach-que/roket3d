#include "roket3d.h"
#include "rscenetexture.h"

namespace video
{
	const char *RTexture::className = TYPE_RVIDEOTEXTURE;
	const Luna < RTexture >::FunctionType RTexture::Functions[] = {
		//{"drawAll", &RTexture::drawAll},
		{0}
	};
	const Luna < RTexture >::PropertyType RTexture::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	void RTexture::setTexture(irr::video::ITexture* tex)
	{
		base_self = tex;
	}
	
	irr::video::ITexture* RTexture::getTexture()
	{
		if (textureExists)
			return base_self;
		else
			return 0;
	}
	
	RTexture::RTexture(lua_State* L, bool instantedByUser)
	{
		if (instantedByUser==true)
			debug("You cannot create RTextures.  You must collect one from an RVideoDriver.",LVL_ERR,L);
	}
}
