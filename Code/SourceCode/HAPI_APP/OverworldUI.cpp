#include "OverworldUI.h"

OverworldUI::OverworldUI()
{
}

OverworldUI::~OverworldUI()
{
}

bool OverworldUIWIndowTest::Initialise()
{
	//UI.AddWindow("testWindow", m_screenRect);
	//UI.OpenWindow("testWindow");

	testBattleMapWindow = true;

	EnemyTerritoryHexSheet->GetTransformComp().SetPosition({ 100, 600 });
	PlayButton->GetTransformComp().SetPosition({ 1150, 722 });
	BackButton->GetTransformComp().SetPosition({ 185, 747 });

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

	Entity newEntity("Data//thing.png");
	//newEntity.setTileLocation(HAPISPACE::VectorI(50, 50));
	newEntity.setHealth(100);
	newEntity.setMovementPoints(6);
	weapon newWeapon;
	newWeapon.damage = 10;
	newWeapon.range = 5;
	newWeapon.type = weaponType::eWeaponType1;
	newEntity.addWeapon(newWeapon);
	m_entityVector.push_back(newEntity);

	Entity secondEntity("Data//HAPI Sprites Logo.png");
	//secondEntity.setTileLocation(HAPISPACE::VectorI(50, 100));
	secondEntity.setHealth(600);
	secondEntity.setMovementPoints(3);
	secondEntity.addWeapon(newWeapon);
	m_entityVector.push_back(secondEntity);

	return true;
}

void OverworldUIWIndowTest::Update()
{
	SCREEN_SURFACE->Clear();

	BattleMapBackground->Render(SCREEN_SURFACE);
	EnemyTerritoryHexSheet->Render(SCREEN_SURFACE);

	if (EnemyTerritoryHexSheet->GetFrameNumber() == 0)
	{
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(EnemyTerritoryHexSheet->GetTransformComp().GetPosition().x + EnemyTerritoryHexSheet->GetCurrentFrame().rect.right / 2.5, EnemyTerritoryHexSheet->GetTransformComp().GetPosition().y + EnemyTerritoryHexSheet->GetCurrentFrame().rect.bottom / 4), difficultyColour, std::to_string(testHexDifficulty), 90);
	}

	if (testPrebattleWindow)
	{
		PrebattleUIBackground->Render(SCREEN_SURFACE);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 270), HAPISPACE::Colour255::BLACK, "45/55", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 355), HAPISPACE::Colour255::BLACK, "3", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 445), HAPISPACE::Colour255::BLACK, "4", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 535), HAPISPACE::Colour255::BLACK, "5", 50);
		PlayButton->Render(SCREEN_SURFACE);
		BackButton->Render(SCREEN_SURFACE);
	}

	//render current ship sprite 1200,300
}

void OverworldUIWIndowTest::Run()
{
	while (HAPI_Sprites.Update())
	{
		Update();
	}
}


void OverworldUIWIndowTest::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		if (testBattleMapWindow && !testPrebattleWindow)
		{
			if (EnemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(EnemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				testPrebattleWindow = true;
			}
		}
		else if (testPrebattleWindow)
		{
			if (PlayButton->GetSpritesheet()->GetFrameRect(0).Translated(PlayButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				BattleSystem world;
				world.run();
			}
			else if (BackButton->GetSpritesheet()->GetFrameRect(0).Translated(BackButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				testPrebattleWindow = false;
			}
		}
	}

	if (mouseEvent == EMouseEvent::eLeftButtonUp)
	{
	}
}

void OverworldUIWIndowTest::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	if (testBattleMapWindow && !testPrebattleWindow)
	{
		if (EnemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(EnemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			EnemyTerritoryHexSheet->SetFrameNumber(1);
		}
		else if (EnemyTerritoryHexSheet->GetFrameNumber() != 0)
		{
			EnemyTerritoryHexSheet->SetFrameNumber(0);
		}
	}
	else if (testPrebattleWindow)
	{
		if (PlayButton->GetSpritesheet()->GetFrameRect(0).Translated(PlayButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			PlayButton->SetFrameNumber(1);
		}
		else if (PlayButton->GetFrameNumber() != 0)
		{
			PlayButton->SetFrameNumber(0);
		}

		if (BackButton->GetSpritesheet()->GetFrameRect(0).Translated(BackButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			BackButton->SetFrameNumber(1);
		}
		else if (BackButton->GetFrameNumber() != 0)
		{
			BackButton->SetFrameNumber(0);
		}
	}
}

void OverworldUIWIndowTest::HandleCollision(Sprite & sprite, Sprite & collideWith)
{

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