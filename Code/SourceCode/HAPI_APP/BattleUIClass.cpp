#include "BattleUIClass.h"
//needs splitting out really into function wrapper

BattleUIClass::BattleUIClass()
{
}

BattleUIClass::~BattleUIClass()
{
}

void UIWIndowTest::OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode)
{
}

void UIWIndowTest::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	mouseX = mouseData.x;
	mouseY = mouseData.y;

	//moves the sprites when the mouse is on the edge of the screen
	//only checks when mouse moves. if mouse doesnt move, it knows its still in the same spot and will keep scrolling without checking
	pendingCameraMovement = VectorF{ 0,0 };

	if (mouseData.x < 100)
	{
		pendingCameraMovement = VectorF{ 1,0 };
	}
	else if (mouseData.x > 1500)
	{
		pendingCameraMovement = VectorF{ -1,0 };
	}

	if (mouseData.y < 100)
	{
		pendingCameraMovement = VectorF{ 0,1 };
	}
	else if (mouseData.y > 800)
	{
		pendingCameraMovement = VectorF{ 0,-1 };
	}
}

void UIWIndowTest::HandleCollision(Sprite & sprite, Sprite & collideWith)
{
	//sprite is the mouse cursor sprite
	// collideWith is the tile
	CollisionInfo info;
	if (sprite.CheckCollision(collideWith, &info) && trigger == true)
	{		
		if (collideWith.GetFrameNumber() == tileState::notSelected)
		{
			collideWith.SetFrameNumber(tileState::selected);
			trigger = false;
		}
		else
		{
			collideWith.SetFrameNumber(tileState::notSelected);
			trigger = false;
		}	
	}
}

void UIWIndowTest::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		mouseX = mouseData.x;
		mouseY = mouseData.y;
		trigger = true;

	}

	if (mouseEvent == EMouseEvent::eWheelForward)
	{
		cameraZoom += 0.1f;
	}
	else if (mouseEvent == EMouseEvent::eWheelBack)
	{
		cameraZoom -= 0.1f;
	}
}

bool UIWIndowTest::Initialise()
{	
	for (int i = 0; i < 16; i++)
	{
		storage.push_back(HAPI_Sprites.LoadSprite("Data\\hexTiles.xml"));// populating the storage 
		storage[i]->GetColliderComp().EnablePixelPerfectCollisions(true);
	}

	storage.push_back(HAPI_Sprites.LoadSprite("Data\\mouseCrossHair.xml"));//temp mouse cursor 
	storage[storage.size()-1]->GetColliderComp().EnablePixelPerfectCollisions(true);

	frameHeight = storage[0]->FrameHeight();
	frameWidth = storage[0]->FrameWidth();

	for (int i = 0; i < storage.size(); i++) // position hexs demo
	{	
		if (i < 8)
		{
			storage[i]->GetTransformComp().SetPosition({ (float)frameWidth * i, 0 });// this wont be set here
		}
		else
		{
			storage[i]->GetTransformComp().SetPosition({ (float)frameWidth * (i - storage.size()/2), 200 });
		}
	}
	return true;
}

void UIWIndowTest::Update()
{
	//HAPI_Sprites.SetShowCursor(false);
	storage[storage.size() - 1]->GetTransformComp().SetPosition({ (float)mouseX - 5,(float)mouseY - 5 });//this is the mouse cursor
	for (int x = 0; x < storage.size(); x++)
	{
		HandleCollision(*storage[storage.size()-1], *storage[x]);
	}

	for (int x = 0; x < storage.size(); x++)
	{
			
		storage[x]->Render(SCREEN_SURFACE);
	}

	if (!pendingCameraMovement.IsZero())													
	{
		
		CameraPositionOffset.first += pendingCameraMovement.x;//translates the camera position
		CameraPositionOffset.second += pendingCameraMovement.y;

		if (CameraPositionOffset.first < -500)//checks for if its reached any of the 4 boundries, need to change it to a variable
		{
			CameraPositionOffset.first = -500;
			sideBoundary = true;
		}
		else if (CameraPositionOffset.first > 500)
		{
			CameraPositionOffset.first = 500;
			sideBoundary = true;
		}
		else
		{
			sideBoundary = false;
		}
		if (CameraPositionOffset.second < -400)
		{
			CameraPositionOffset.second = -400;
			floorBoundary = true;
		}
		else if (CameraPositionOffset.second > 400)
		{
			CameraPositionOffset.second = 400;
			floorBoundary = true;
		}
		else
		{
			floorBoundary = false;
		}

		if (!sideBoundary && !floorBoundary)
		{
			//m_heliSprite->GetTransformComp().Translate(pendingCameraMovement);//translates the sprites indevidually, temp solution. Will eventually just transform camera position which will be taken into account in the sprite position
			//m_logoSprite->GetTransformComp().Translate(pendingCameraMovement);
			//m_stickySprite->GetTransformComp().Translate(pendingCameraMovement);
		}
	}
}