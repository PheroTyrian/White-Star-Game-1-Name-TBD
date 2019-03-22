#pragma once
#include <HAPISprites_lib.h>
using namespace HAPISPACE;
#include <HAPISprites_UI.h>
using namespace HAPI_UI_SPACE;
#include <string>
#include "Entity.h"
#include "BattleSystem.h"
class OverworldUI
{
public:
	OverworldUI();
	~OverworldUI();
};

class OverworldUIWIndowTest :public IHapiSpritesInputListener, public IHapiSpritesUIListener
{
private:
	RectangleI m_screenRect;
	Transform m_rectTransform;
	ColliderGroup m_rectCollider;

	std::vector<Entity> m_entityVector;
	std::vector<Entity> m_SelectedEntities;

	std::shared_ptr<Sprite> sprite = HAPI_Sprites.LoadSprite("Data\\thing.png");

public:
	bool Initialise();
	void Update();
	void Run();
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void HandleCollision(Sprite& sprite, Sprite& collideWith);
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final;
	void UI_RadioButtonChangeState(UIWindow& window, const std::string& buttonName, bool pressed, int* userId) override final;

	std::shared_ptr<Sprite> BattleMapBackground = HAPI_Sprites.MakeSprite("Data\\TempBattleMapBackground.png");
	std::shared_ptr<Sprite> EnemyTerritoryHexSheet = HAPI_Sprites.MakeSprite("Data\\EnemyTerritoryHexSheet.png", 2);
	std::shared_ptr<Sprite> PrebattleUIBackground = HAPI_Sprites.MakeSprite("Data\\PrebattleUI.png");
	std::shared_ptr<Sprite> PlayButton = HAPI_Sprites.MakeSprite("Data\\PrebattleUIPlayButton.png", 2);
	std::shared_ptr<Sprite> BackButton = HAPI_Sprites.MakeSprite("Data\\PrebattleUIBackButton.png", 2);

	int CameraPositionX = 0;
	int CameraPositionY = 0;
	VectorF pendingCameraMovement{ 0 };

	OverworldUIWIndowTest(RectangleI screenRect) : m_screenRect(screenRect), m_rectCollider({ 0,300,0,40 }) {}
	float playerFleetPower = 3.2;
	int testHexDifficulty = 1.7;//this needs to be changed back to a float once ive got it displaying the string to 1 decimal place
	int hard = 2;//how many difficulty levels above the players power a level needs to be to be considered hard
	Colour255 difficultyColour;

	bool testBattleMapWindow = false;
	bool testPrebattleWindow = false;
};