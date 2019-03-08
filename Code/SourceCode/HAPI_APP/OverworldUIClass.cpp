#include "OverworldUiClass.h"

OverworldUiClass::OverworldUiClass()
{
}

OverworldUiClass::~OverworldUiClass()
{
}


void OverworldUIWIndowTest::Update()
{
	if (!pendingCameraMovement.IsZero())													//change this to move the offset on the map.cpp
	{
		CameraPositionX += pendingCameraMovement.x;//translates the camera position
		CameraPositionY += pendingCameraMovement.y;

		if (CameraPositionX < -500)//checks for if its reached any of the 4 boundries
		{
			CameraPositionX = -500;
			sideBoundary = true;
		}
		else if (CameraPositionX > 500)
		{
			CameraPositionX = 500;
			sideBoundary = true;
		}
		else
		{
			sideBoundary = false;
		}
		if (CameraPositionY < -400)
		{
			CameraPositionY = -400;
			floorBoundary = true;
		}
		else if (CameraPositionY > 400)
		{
			CameraPositionY = 400;
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

void OverworldUIWIndowTest::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	//moves the sprites when the mouse is on the edge of the screen
	//only checks when mouse moves. if mouse doesnt move, it knows its still in the same spot and will keep scrolling without checking
	pendingCameraMovement = VectorF{ 0,0 };

	if (mouseData.x < 100)
	{
		pendingCameraMovement = VectorF{ 1,0 };
	}
	else if (mouseData.x > 1180)
	{
		pendingCameraMovement = VectorF{ -1,0 };
	}

	if (mouseData.y < 100)
	{
		pendingCameraMovement = VectorF{ 0,1 };
	}
	else if (mouseData.y > 700)
	{
		pendingCameraMovement = VectorF{ 0,-1 };
	}
}

void OverworldUIWIndowTest::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eWheelForward)
	{
		cameraZoom += 0.1f;
		//m_heliSprite->GetTransformComp().SetScaling(m_heliSprite->GetTransformComp().GetScale() + 0.1f);//scales the sprites indevidually, temp solution. Will eventually scale camera. Also need to ask kieth if i was alowed to change his vector operator.
		//m_logoSprite->GetTransformComp().SetScaling(m_logoSprite->GetTransformComp().GetScale() + 0.1f);
		//m_stickySprite->GetTransformComp().SetScaling(m_stickySprite->GetTransformComp().GetScale() + 0.1f);
	}
	else if (mouseEvent == EMouseEvent::eWheelBack)
	{
		cameraZoom -= 0.1f;
		//m_heliSprite->GetTransformComp().SetScaling(m_heliSprite->GetTransformComp().GetScale() + -0.1f);//scales the sprites indevidually, temp solution
		//m_logoSprite->GetTransformComp().SetScaling(m_logoSprite->GetTransformComp().GetScale() + -0.1f);
		//m_stickySprite->GetTransformComp().SetScaling(m_stickySprite->GetTransformComp().GetScale() + -0.1f);
	}

	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		//mouseData.x//how do you check if a mouse click collided with a collider?
	}
}

/*
	This function is called from the UI when a radio button in the window changes state (pressed or unpressed)
	The userId is something that can be provided on creation but not used in this demo
*/
void OverworldUIWIndowTest::UI_RadioButtonChangeState(UIWindow& window, const std::string& buttonName, bool pressed, int* userId)
{
	/*if (buttonName == "EnableCollisionInfo")
	{
		m_stickySprite->GetColliderComp().CalculateCollisionData(pressed);
	}

	if (buttonName == "EnablePixelPerfect")
	{
		m_stickySprite->GetColliderComp().EnablePixelPerfectCollisions(pressed);
	}

	if (buttonName == "ShowBounds")
	{
		m_showBounds = pressed;
	}

	if (buttonName == "ShowNormals")
	{
		m_showNormals = pressed;
	}*/
}