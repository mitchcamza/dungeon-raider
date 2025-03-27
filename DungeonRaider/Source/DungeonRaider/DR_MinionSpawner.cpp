// Copyright Mitch Campbell 2025. All Rights Reserved.


#include "DR_MinionSpawner.h"
#include "DR_Minion.h"
#include "Components/BoxComponent.h"


// Sets default values
ADR_MinionSpawner::ADR_MinionSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Area"));
	SpawnArea->SetupAttachment(RootComponent);
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));
}

// Called when the game starts or when spawned
void ADR_MinionSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Check that a spawnable minion type exists
	if (SpawnableMinions.IsEmpty())
	{
		return;
	}

	// Check that the actor has the authority to spawn other actors
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// Spawn initial enemies
	for (int32 i = 0; i < NumMinionsAtStart; i++)
	{
		Spawn();
	}
	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ADR_MinionSpawner::Spawn,
		SpawnDelay,
		true,
		SpawnDelay
	);
}

void ADR_MinionSpawner::Spawn()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	auto Minion = SpawnableMinions[FMath::RandRange(0, SpawnableMinions.Num() - 1)];
	const auto Rotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
	const auto Location = SpawnArea->GetComponentLocation() + FVector(
		FMath::RandRange(-SpawnArea->GetScaledBoxExtent().X, SpawnArea->GetScaledBoxExtent().X),
		FMath::RandRange(-SpawnArea->GetScaledBoxExtent().Y, SpawnArea->GetScaledBoxExtent().Y),
		0.0f
	);
	GetWorld()->SpawnActor<ADR_Minion>(Minion, Location, Rotation, SpawnParameters);
}

// Called every frame
void ADR_MinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

