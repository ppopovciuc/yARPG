// Fill out your copyright notice in the Description page of Project Settings.

#include "ARP.h"
#include "GameDataSingleton.h"


UGameDataSingleton::UGameDataSingleton()
{
}

FCombatVisualEffectStruct UGameDataSingleton::GetCombatVisualEffect(int EffectID)
{
	for (int i = 0; i < CombatVisualEffects.Num(); i++)
	{
		if (CombatVisualEffects[i].ID == EffectID)
		{
			return CombatVisualEffects[i];
		}
	}

	return FCombatVisualEffectStruct();
}

FCombatEffectStruct UGameDataSingleton::GetCombatEffect(int EffectID)
{
	for (int i = 0; i < CombatEffects.Num(); i++)
	{
		if (CombatEffects[i].ID == EffectID)
		{
			return CombatEffects[i];
		}
	}

	return FCombatEffectStruct();
}

FAbilityStruct UGameDataSingleton::GetAbility(int AbilityID)
{
	for (int i = 0; i < Abilities.Num(); i++)
	{
		if (Abilities[i].ID == AbilityID)
		{
			return Abilities[i];
		}
	}

	return FAbilityStruct();
}