// Copyright Mitch Campbell 2025


#include "DR_BasePickup.h"
#include "DR_Character.h"
#include "Components/SphereComponent.h"


// Sets default values
ADR_BasePickup::ADR_BasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = SphereCollision;
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(200.0f);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADR_BasePickup::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADR_BasePickup::OnBeginOverlap);
}

void ADR_BasePickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto Character = Cast<ADR_Character>(OtherActor))
	{
		Pickup(Character);
	}
}

void ADR_BasePickup::Pickup_Implementation(class ADR_Character* OwningCharacter)
{
	SetOwner(OwningCharacter);
}


// Called every frame
void ADR_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

