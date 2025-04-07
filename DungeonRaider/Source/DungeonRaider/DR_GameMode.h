// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DR_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONRAIDER_API ADR_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADR_GameMode();

	UFUNCTION(BlueprintCallable, Category = "Minions")
	void AlertMinions(AActor* AlertInstigator, const FVector& Location, float Radius);
};
