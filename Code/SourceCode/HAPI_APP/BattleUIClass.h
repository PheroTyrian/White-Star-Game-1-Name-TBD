#pragma once
#include <HAPISprites_lib.h>
using namespace HAPISPACE;
#include <HAPISprites_UI.h>
using namespace HAPI_UI_SPACE;

class BattleUIClass
{
public:
	BattleUIClass();
	~BattleUIClass();
	
private:
};

class UIWIndowTest :public IHapiSpritesInputListener, public IHapiSpritesUIListener
{
private:

	enum tileState
	{
		notSelected = 0, 
		player = 1,
		selected = 2
	};

	bool phase1{ false };	
	//temp value set for phases  phase 1 select ship	
	RectangleI m_screenRect;	
	Transform m_rectTransform;
	ColliderGroup m_rectCollider;
	std::vector<std::unique_ptr <Sprite>>storage;
	
	//will be in a vector 
	bool trigger = false;//used for switching
	int frameHeight;
	int frameWidth;

	bool sideBoundary = false;
	bool floorBoundary = false;//if sides are on the left and right, what is on the top and bottom. I dont think a word exists for that. vertical, plane & floor arnt quite right.
	//int sideBoundary;
	VectorF pendingCameraMovement{ 0 };
	std::pair<int, int> CameraPositionOffset;

public:
	UIWIndowTest(RectangleI screenRect) : m_screenRect(screenRect), m_rectCollider({ 0,300,0,40 }) {}
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final;
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void HandleCollision(Sprite& sprite, Sprite& collideWith);
	bool Initialise();
	void Update();
	int mouseX, mouseY;
};