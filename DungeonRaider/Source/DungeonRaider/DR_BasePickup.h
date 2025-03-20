// Copyright Mitch Campbell 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DR_BasePickup.generated.h"

UCLASS()
class DUNGEONRAIDER_API ADR_BasePickup : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	TObjectPtr<class USphereComponent> SphereCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	TObjectPtr<class UStaticMeshComponent> Mesh;
	
public:	
	// Sets default values for this actor's properties
	ADR_BasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup", meta = (DisplayName = "Pickup"))
	void Pickup(class ADR_Character* OwningCharacter);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
};
