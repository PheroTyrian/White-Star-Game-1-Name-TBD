#include "UIClass.h"
//needs splitting out really into function wrapper

void UIWindowTest::OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode)
{
}

void UIWindowTest::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	//moves the sprites when the mouse is on the edge of the screen
	//only checks when mouse moves. if mouse doesnt move, it knows its still in the same spot and will keep scrolling without checking

	mouseX = mouseData.x;
	mouseY = mouseData.y;
	for (int x = 0; x < storage.size(); x++)
	{
		HandleCollision(*storage[storage.size() - 1], *storage[x]);
	}
}

void UIWindowTest::HandleCollision(Sprite & sprite, Sprite & collideWith)
{
	//sprite is the mouse cursor sprite
	// collideWith is the tile

	CollisionInfo info;
	if (sprite.CheckCollision(collideWith, &info) && trigger == true)
	{
		//auto duck = collideWith.GetTransformComp().GetPosition();

		collideWith.AdvanceToNextFrame();
		trigger = false;
	}
}


void UIWindowTest::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		mouseX = mouseData.x;
		mouseY = mouseData.y;
		trigger = true;
	}
}

bool UIWindowTest::Initialise()
{
	storage.push_back(HAPI_Sprites.LoadSprite("Data\\mouseCrossHair.xml"));//temp mouse cursor sprite
	storage[storage.size() - 1]->GetColliderComp().EnablePixelPerfectCollisions(true);

	frameHeight = storage[0]->FrameHeight();
	frameWidth = storage[0]->FrameWidth();

	return true;
}

void UIWindowTest::Update()
{
	//HAPI_Sprites.SetShowCursor(false);

	storage[storage.size() - 1]->GetTransformComp().SetPosition({ (float)mouseX - 5,(float)mouseY - 5 });//this is the mouse cursor

	for (int x = 0; x < storage.size(); x++)
	{
		storage[x]->Render(SCREEN_SURFACE);
	}
}