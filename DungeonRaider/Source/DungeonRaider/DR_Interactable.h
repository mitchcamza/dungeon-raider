// Copyright Mitch Campbell 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DR_Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UDR_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONRAIDER_API IDR_Interactable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Interact"))
	void Interact(class ADR_Character* CharacterInstigator);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Can Interact"))
	bool bCanInteract(ADR_Character* CharacterInstigator) const;
};
