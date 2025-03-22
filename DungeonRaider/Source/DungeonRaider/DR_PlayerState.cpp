// Fill out your copyright notice in the Description page of Project Settings.


#include "DR_PlayerState.h"
#include "DR_Character.h"
#include "DR_CharacterStats.h"
#include "Net/UnrealNetwork.h"

void ADR_PlayerState::AddXp(int32 Value)
{
	Xp += Value;
	OnXpChanged.Broadcast(Xp);
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString::Printf(TEXT("Total Xp: %d"), Value));
	if (const auto Character = Cast<ADR_Character>(GetPawn()))
	{
		if (Character->GetCharacterStats()->NextLevelXp < Xp)
		{
			GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("Level Up!"));
			CharacterLevel++;
			Character->UpdateCharacterStats(CharacterLevel);
			OnCharacterLevelUp.Broadcast(CharacterLevel);
		}
	}
}

void ADR_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ADR_PlayerState, Xp, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ADR_PlayerState, CharacterLevel, COND_OwnerOnly);
}

void ADR_PlayerState::OnRep_Xp(int32 OldValue) const
{
	OnXpChanged.Broadcast(Xp);
}

void ADR_PlayerState::OnRep_CharacterLevelUp(int32 OldValue) const
{
	OnCharacterLevelUp.Broadcast(CharacterLevel);
}
