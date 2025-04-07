// Fill out your copyright notice in the Description page of Project Settings.


#include "DR_GameMode.h"
#include "DR_GameState.h"
#include "DR_PlayerController.h"
#include "DR_PlayerState.h"
#include "DR_Character.h"
#include "UObject/ConstructorHelpers.h"
#include "DR_Minion.h"
#include "Kismet/GameplayStatics.h"


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

void ADR_GameMode::AlertMinions(AActor* AlertInstigator, const FVector& Location, float Radius)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "AlertMinions");
	
	// Store all minion types in an array
	TArray<AActor*> FoundMinions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADR_Minion::StaticClass(), FoundMinions);

	// Calculate the distance between each minion and the alerting one
	for (const auto Minion : FoundMinions)
	{
		if (AlertInstigator == Minion)
		{
			continue;
		}

		// Move all minions to the specified location, if in range
		if (const auto Distance = FVector::Distance(AlertInstigator->GetActorLocation(), Minion->GetActorLocation()); Distance < Radius)
		{
			if (const auto MinionCharacter = Cast<ADR_Minion>(Minion))
			{
				MinionCharacter->GoToLocation(Location);
			}
		}
	}
}
