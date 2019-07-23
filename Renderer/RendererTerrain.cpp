#include "RendererTerrain.h"
#include "lodepng.h"

namespace RendererLib
{

float* RendererTerrain::ReadHeightMapValuesFromFile(const std::string& fileName, unsigned int landscapeSize, float scaleFactor)
{
	std::vector<unsigned char> png;
	std::vector<unsigned char> image;

	unsigned int width = landscapeSize;
	unsigned int height = landscapeSize;

	lodepng::State state;

	lodepng::load_file(png, fileName);
	lodepng::decode(image, width, height, state, png);

	float* heightArray = new float[landscapeSize * landscapeSize];

	int currentOffset = 0;
	for (unsigned int i = 0; i < landscapeSize * landscapeSize; i++)
	{
        heightArray[i] = powf((image[currentOffset] / 255.0f), 2.2f) * scaleFactor;
		currentOffset += 4;
	}

	return heightArray;
}

}
