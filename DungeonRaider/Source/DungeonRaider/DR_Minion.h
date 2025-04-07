// Copyright Mitch Campbell 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DR_Minion.generated.h"

class UPawnSensingComponent;
class USphereComponent;
class ADR_BasePickup;

UCLASS()
class DUNGEONRAIDER_API ADR_Minion : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADR_Minion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion AI")
	float PatrolSpeed = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion AI")
	float ChaseSpeed = 350.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion AI")
	float PatrolRadius = 1000000.0f;

	UFUNCTION(BlueprintCallable, Category = "Minion AI")
	void SetNextPatrolLocation();
	UFUNCTION(BlueprintCallable, Category = "Minion AI")
	void Chase(APawn* Pawn);
	virtual void PostInitializeComponents() override;
	FORCEINLINE UPawnSensingComponent* GetPawnSense() const { return PawnSense; }
	FORCEINLINE USphereComponent* GetCollision() const { return Collision; }

	UFUNCTION()
	void OnPawnDetected(APawn* Pawn);
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Minion AI")
	void GoToLocation(const FVector& Location);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPawnSensingComponent> PawnSense;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Collision;
	UPROPERTY()
	FVector PatrolLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion AI", meta = (AllowPrivateAccess = "true"))
	float AlertRadius = 10000.0f;
	float IdleTime = 1.0f;
	UPROPERTY()
	FTimerHandle IdleTimerHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADR_BasePickup> SpawnedPickup;
};
