// Copyright Mitch Campbell 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DR_MinionSpawner.generated.h"

class UBoxComponent;
class ADR_Minion;

UCLASS()
class DUNGEONRAIDER_API ADR_MinionSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADR_MinionSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Spawn();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn System")
	TArray<TSubclassOf<ADR_Minion>> SpawnableMinions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn System")
	float SpawnDelay = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn System")
	int32 NumMinionsAtStart = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn System")
	int32 MaxMinionsToBeSpawned = 4;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn System", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> SpawnArea;
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

	int32 CurrentMinionCount;
};
