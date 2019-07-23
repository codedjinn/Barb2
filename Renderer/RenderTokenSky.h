#ifndef RENDERTOKENSKY_H_INCLUDED
#define RENDERTOKENSKY_H_INCLUDED

#include "RenderToken.h"
#include <string>
#include <vector>

namespace RendererLib
{

class RenderTokenSky : 
	public RenderToken
{
	public:
		  
		RenderTokenSky(std::vector<std::string>& faceTextures)
		{
			this->faceTextures = faceTextures;
		}

		virtual ~RenderTokenSky() {}

		std::vector<std::string> faceTextures;
};

}


#endif // RENDERTOKENSKY_H_INCLUDED
