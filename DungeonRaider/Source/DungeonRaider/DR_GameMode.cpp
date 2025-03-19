// Fill out your copyright notice in the Description page of Project Settings.


#include "DR_GameMode.h"
#include "DR_GameState.h"
#include "DR_PlayerController.h"
#include "DR_PlayerState.h"
#include "DR_Character.h"
#include "UObject/ConstructorHelpers.h"


ADR_GameMode::ADR_GameMode()
{
	GameStateClass = ADR_GameState::StaticClass();
	PlayerStateClass = ADR_PlayerState::StaticClass();
	PlayerControllerClass = ADR_PlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Character"));
	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
