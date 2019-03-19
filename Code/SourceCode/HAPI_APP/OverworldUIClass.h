#pragma once
#include <HAPISprites_lib.h>
using namespace HAPISPACE;
#include <HAPISprites_UI.h>
using namespace HAPI_UI_SPACE;
class OverworldUiClass
{
public:
	OverworldUiClass();
	~OverworldUiClass();
};

class OverworldUIWIndowTest :public IHapiSpritesInputListener, public IHapiSpritesUIListener
{
private:
	float cameraZoom = 1.0f;

	RectangleI m_screenRect;
	Transform m_rectTransform;
	ColliderGroup m_rectCollider;
	bool test = false;

public:
	bool Initialise();
	void Update();
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final;
	void UI_RadioButtonChangeState(UIWindow& window, const std::string& buttonName, bool pressed, int* userId) override final;
	int CameraPositionX = 0;
	int CameraPositionY = 0;
	VectorF pendingCameraMovement{ 0 };
	std::shared_ptr<Sprite> BattleMapBackground = HAPI_Sprites.MakeSprite("Data\\TempBattleMapBackground.png");
	std::shared_ptr<Sprite> EnemyTerritoryHexSheet = HAPI_Sprites.MakeSprite("Data\\EnemyTerritoryHexSheet.png", 2);
	OverworldUIWIndowTest(RectangleI screenRect) : m_screenRect(screenRect), m_rectCollider({ 0,300,0,40 }) {}
	int playerFleetPower = 3.2;
	int testHexDifficulty = 5.7;
	int hard = 2;
	Colour255 difficultyColour;
};