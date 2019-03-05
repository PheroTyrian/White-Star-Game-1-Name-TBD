#pragma once


class Sprite;

class Rendering
{
public:
	Rendering();
	~Rendering();

	//Renders the passed sprite to the screen buffer.
	void renderSprite(Sprite& sprite);
	void renderSprite(Sprite& sprite, std::pair<int, int> position);

	//For creating new sprites, saves as an XML under the name specified as "fileName".  They can then be loaded in future.
	void createNewSprite(std::string& fileName, std::string& textureFileName, int numberOfFrames);
	//For loading existing saved sprites.
	void loadSprite(std::string& fileName, int frame = 0);
};

 