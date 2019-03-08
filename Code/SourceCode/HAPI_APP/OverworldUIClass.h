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
	bool sideBoundary = false;
	bool floorBoundary = false;//if sides are on the left and right, what is on the top and bottom. I dont think a word exists for that. vertical, plane & floor arnt quite right.
	float cameraZoom = 1.0f;

public:
	void Update();
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void UI_RadioButtonChangeState(UIWindow& window, const std::string& buttonName, bool pressed, int* userId) override final;
	int CameraPositionX = 0;
	int CameraPositionY = 0;
	VectorF pendingCameraMovement{ 0 };
};