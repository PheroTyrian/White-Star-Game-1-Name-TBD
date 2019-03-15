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

public:
	void Initialise();
	void Update();
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void UI_RadioButtonChangeState(UIWindow& window, const std::string& buttonName, bool pressed, int* userId) override final;
	int CameraPositionX = 0;
	int CameraPositionY = 0;
	VectorF pendingCameraMovement{ 0 };
	std::shared_ptr<Sprite> BattleMapBackground = HAPI_Sprites.MakeSprite("Data\\TempBattleMapBackground.png");
	std::shared_ptr<Sprite> EnemyTerritoryHexSheet = HAPI_Sprites.MakeSprite("Data\\EnemyTerritoryHexSheet.png", 2);
};