// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Object.h"
#include "CombatData.h"
#include "GameDataSingleton.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ARP_API UGameDataSingleton : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TArray<FCombatEffectStruct> CombatEffects;

	UPROPERTY(EditAnywhere)
		TArray<FCombatVisualEffectStruct> CombatVisualEffects;

	UPROPERTY(EditAnywhere)
		TArray<FAbilityStruct> Abilities;

public:
	UGameDataSingleton();

	FCombatVisualEffectStruct	GetCombatVisualEffect(int EffectID);
	FCombatEffectStruct		GetCombatEffect(int EffectID);
	FAbilityStruct			GetAbility(int AbilityID);
};
