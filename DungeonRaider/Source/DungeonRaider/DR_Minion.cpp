// Copyright Mitch Campbell 2025. All Rights Reserved.


#include "DR_Minion.h"
#include "DR_Character.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "DR_GameMode.h"


// Sets default values
ADR_Minion::ADR_Minion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	PawnSense->SensingInterval = 1.0f;
	PawnSense->SetPeripheralVisionAngle(45.f);
	PawnSense->SightRadius = 5000.f;
	PawnSense->HearingThreshold = 500.f;
	PawnSense->LOSHearingThreshold = 1000.f;
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(100);
	Collision->SetupAttachment(RootComponent);
	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/KayKit/Skeletons/skeleton_minion"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

// Called when the game starts or when spawned
void ADR_Minion::BeginPlay()
{
	Super::BeginPlay();
	
	// Set initial patrol location and then go to next patrol location if idle
	GetWorldTimerManager().SetTimer(
	IdleTimerHandle,
	this,
	&ADR_Minion::SetNextPatrolLocation,
	IdleTime,
	true,
	IdleTime
);
}

void ADR_Minion::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Noise detected!"));
	GoToLocation(Location);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
}

// Called every frame
void ADR_Minion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// Check whether the enemy character is patrolling
	if (GetCharacterMovement()->GetMaxSpeed() == ChaseSpeed)
	{
		return;
	}

	// Look for another reachable location once close enough to current patrol location
	if ((GetActorLocation() - PatrolLocation).Size() < 500.f)
	{
		SetNextPatrolLocation();
	}
}

// Called to bind functionality to input
void ADR_Minion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADR_Minion::SetNextPatrolLocation()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	const auto bIsLocationFound = UNavigationSystemV1::K2_GetRandomReachablePointInRadius(
		this,
		GetActorLocation(),
		PatrolLocation,
		PatrolRadius
	);
	if (bIsLocationFound)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
	}
}

void ADR_Minion::Chase(APawn* Pawn)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), Pawn);
	DrawDebugSphere(
		GetWorld(),
		Pawn->GetActorLocation(),
		25.f,
		12,
		FColor::Red,
		false,
		2.f,
		0,
		2.f
	);
}

void ADR_Minion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	OnActorBeginOverlap.AddDynamic(this, &ADR_Minion::OnBeginOverlap);
	GetPawnSense()->OnSeePawn.AddDynamic(this, &ADR_Minion::OnPawnDetected);
	GetPawnSense()->OnHearNoise.AddDynamic(this, &ADR_Minion::OnHearNoise);
}

void ADR_Minion::OnPawnDetected(APawn* Pawn)
{
	if (!Pawn->IsA<ADR_Character>())
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character detected!"));
	if (GetCharacterMovement()->MaxWalkSpeed != ChaseSpeed)
	{
		Chase(Pawn);
	}

	// Retrieve the game mode and send alert to nearby minions
	if (const auto GameMode = Cast<ADR_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->AlertMinions(this, Pawn->GetActorLocation(), AlertRadius);
	}
}

void ADR_Minion::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor->IsA<ADR_Character>())
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Character captured!"));
}

void ADR_Minion::GoToLocation(const FVector& Location)
{
	PatrolLocation = Location;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
}

