// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DR_Character.generated.h"

UCLASS()
class DUNGEONRAIDER_API ADR_Character : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractAction;
	

public:
	// Sets default values for this character's properties
	ADR_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const struct FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprintEnd(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
