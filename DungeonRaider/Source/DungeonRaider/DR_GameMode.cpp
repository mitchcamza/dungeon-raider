// Fill out your copyright notice in the Description page of Project Settings.


#include "DR_GameMode.h"
#include "DR_GameState.h"

ADR_GameMode::ADR_GameMode()
{
	GameStateClass = ADR_GameState::StaticClass();
}
