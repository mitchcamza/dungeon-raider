// Fill out your copyright notice in the Description page of Project Settings.


#include "DR_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
ADR_Character::ADR_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Default component properties
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/KayKit/Characters/rogue"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

// Called when the game starts or when spawned
void ADR_Character::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADR_Character::Move(const struct FInputActionValue& Value)
{
	const auto MovementVector = Value.Get<FVector2d>();
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString::Printf(TEXT("MovementVector: %s"), *MovementVector.ToString()));
	if (Controller)
	{
		const auto Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADR_Character::Look(const FInputActionValue& Value)
{
	const auto LookAxisVector = Value.Get<FVector2d>();
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("LookAxisVector: %s"), *LookAxisVector.ToString()));
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADR_Character::SprintStart(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Blue, TEXT("SprintStart"));
	GetCharacterMovement()->MaxWalkSpeed = 3000.f;
}

void ADR_Character::SprintEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Blue, TEXT("SprintEnd"));
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ADR_Character::Interact(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("Interact"));
}

// Called every frame
void ADR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADR_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADR_Character::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADR_Character::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ADR_Character::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADR_Character::SprintEnd);
	}
}

