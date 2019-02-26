#include "Rendering.h"
#include <HAPISprites_Lib.h>

/*TODO
> Transform and Collision functions.
> Consider making SetModulatingColour().
> Consider incorperating a global for vec2 to replace the pair of ints returned by getFrameDimensions()
*/

class Sprite
{
private:
	std::unique_ptr<HAPISPACE::Sprite> m_data;

public:

	//This is for loading an already saved sprite (what we want to do in the final game)
	Sprite(const std::string& fileName, const std::string& textureRootPath = std::string())
	{
		m_data = HAPI_Sprites.LoadSprite(fileName, textureRootPath);
	}

	//This is for makeing a new sprite (necessary to build sprites we can load later)
	Sprite(const std::string& textureFileName, int numberOfFrames = 1, const std::string& frameSetName = std::string())
	{
		m_data = HAPI_Sprites.MakeSprite(textureFileName, false, numberOfFrames, frameSetName);
	}

	~Sprite() {}

	void renderSprite(std::shared_ptr<HAPISPACE::Surface>& screenSurface) 
	{
		m_data->Render(screenSurface);
	}

	void setFrameNumber(int frameNum, const std::string& setName = std::string())
	{
		m_data->SetFrameNumber(frameNum, setName);
	}

	bool advanceToNextFrame(bool loop = true)
	{
		m_data->AdvanceToNextFrame(loop);
	}

	void setAutoAnimate(float framesPerSecond, bool loop = true, const std::string& setName = std::string())
	{
		m_data->SetAutoAnimate(framesPerSecond, loop, setName);
	}

	bool save(const std::string fileName)
	{
		return m_data->Save(fileName);
	}

	int getNumberOfFrames()
	{
		return m_data->GetNumFrames();
	}

	int getFrameNumber()
	{
		return m_data->GetFrameNumber();
	}

	std::pair<int,int> getFrameDimensions()
	{
		int x = m_data->FrameWidth();
		int y = m_data->FrameHeight();
		std::pair<int, int> tmp{ x, y };
		return tmp;
	}

	void setPosition(std::pair<float, float> newPosition)
	{
		m_data->GetTransformComp().SetPosition(HAPISPACE::VectorF(newPosition.first, newPosition.second));
	}
};

Rendering::Rendering()
{
}


Rendering::~Rendering()
{
}

void Rendering::renderSprite(Sprite & sprite)
{
	sprite.renderSprite(SCREEN_SURFACE);
}
void Rendering::renderSprite(Sprite & sprite, std::pair<int, int> position)
{

	sprite.renderSprite(SCREEN_SURFACE);
}
//TODO
void Rendering::createNewSprite(std::string & fileName, std::string & textureFileName, int numberOfFrames)
{
}

void Rendering::loadSprite(std::string & fileName, int frame)
{
	
}
