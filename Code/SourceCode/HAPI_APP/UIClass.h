#pragma once
#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
using namespace HAPISPACE;
using namespace HAPI_UI_SPACE;
/*
class UIClass
{
public:
	UIClass();
	~UIClass();

private:
};
*/
class UIWindowTest :public IHapiSpritesInputListener, public IHapiSpritesUIListener
{
private:
	enum tileState
	{
		notSelected = 0,
		player = 1,
		selected = 2
	};
	//temp value set for phases  phase 1 select ship

	RectangleI m_screenRect;

	Transform m_rectTransform;
	ColliderGroup m_rectCollider;

	//will be in a vector 
	bool trigger = false;//used for switching
	int frameHeight;
	int frameWidth;

	bool sideBoundary = false;
	bool floorBoundary = false;//if sides are on the left and right, what is on the top and bottom. I dont think a word exists for that. vertical, plane & floor arnt quite right.
	VectorF pendingCameraMovement{ 0 };
	std::pair<int, int> CameraPositionOffset;//camera offset that will be used by the map
	float cameraZoom = 1.0f;//variable to multiply scale by

public:
	UIWindowTest();

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final;
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void HandleCollision(Sprite& sprite, Sprite& collideWith);
	void Update();
	int mouseX, mouseY;
	std::pair<float,float> tilePos;// this is to get center of sprite
	std::vector<std::unique_ptr <Sprite>>storage;
};