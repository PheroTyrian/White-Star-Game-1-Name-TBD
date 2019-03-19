#include "OverworldUiClass.h"

OverworldUiClass::OverworldUiClass()
{
}

OverworldUiClass::~OverworldUiClass()
{
}

bool OverworldUIWIndowTest::Initialise()
{
	EnemyTerritoryHexSheet->GetTransformComp().SetPosition({ 100, 600 });
	if (playerFleetPower > testHexDifficulty)
	{
		difficultyColour = HAPISPACE::Colour255::GREEN;
	}
	else if (playerFleetPower < testHexDifficulty + hard && playerFleetPower > testHexDifficulty)
	{
		difficultyColour = HAPISPACE::Colour255::RED;
	}
	else
	{
		difficultyColour = HAPISPACE::Colour255::YELLOW;
	}


	return true;
}

void OverworldUIWIndowTest::Update()
{
	BattleMapBackground->Render(SCREEN_SURFACE);
	EnemyTerritoryHexSheet->Render(SCREEN_SURFACE);
	if (test)//mouse colides with hex
	{
		EnemyTerritoryHexSheet->SetFrameNumber(1);
	}
	else
	{
		EnemyTerritoryHexSheet->SetFrameNumber(0);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(EnemyTerritoryHexSheet->GetTransformComp().GetPosition().x + EnemyTerritoryHexSheet->GetCurrentFrame().rect.right / 2.5, EnemyTerritoryHexSheet->GetTransformComp().GetPosition().y + EnemyTerritoryHexSheet->GetCurrentFrame().rect.bottom / 4), difficultyColour, std::to_string(testHexDifficulty), 90);
	}

	PrebattleUIBackground->Render(SCREEN_SURFACE);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 270), HAPISPACE::Colour255::BLACK, "45/55", 50);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 355), HAPISPACE::Colour255::BLACK, "3", 50);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 445), HAPISPACE::Colour255::BLACK, "4", 50);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 535), HAPISPACE::Colour255::BLACK, "5", 50);
	//render current ship sprite 1200,300

	//play button
	//back button
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
		test= !test;
	}
}

void OverworldUIWIndowTest::OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode)
{

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