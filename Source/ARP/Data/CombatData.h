// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "UObject/NoExportTypes.h"
#include "CombatData.generated.h"

///**
// * 
// */
//UCLASS()
//class ARP_API UCombatData : public UObject
//{
//	GENERATED_BODY()
//	
//	
//	
//	
//};


USTRUCT()
struct FHasID
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		int ID;

	FHasID()
	{
		ID = 0;
	}
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EStatusEffectsEnum : uint8
{
	SEE_Slowed		UMETA(DisplayName = "Slowed"),
	SEE_Silenced	UMETA(DisplayName = "Silenced"),
	SEE_Rooted 		UMETA(DisplayName = "Rooted"),
	SEE_Stunned		UMETA(DisplayName = "Stunned"),
	SEE_MAX
};

UENUM(BlueprintType)
enum class ECombatAttributesEnum : uint8
{
	CAE_AttackDamage	UMETA(DisplayName = "AttackDamage"),
	CAE_AbilityPower	UMETA(DisplayName = "AbilityPower"),
	CAE_Health			UMETA(DisplayName = "Health"),
	CAE_Resource		UMETA(DisplayName = "Resource"),
	CAE_CriticalChance	UMETA(DisplayName = "CriticalChance"),
	CAE_AttackSpeed		UMETA(DisplayName = "AttackSpeed"),
	CAE_MagicResistance	UMETA(DisplayName = "MagicResistance"),
	CAE_Armor			UMETA(DisplayName = "Armor"),
	CAE_MAX
};

USTRUCT()
struct FCombatEffectStruct : public FHasID
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TickInterval;

	UPROPERTY(EditAnywhere)
		int DurationInTicks;

	UPROPERTY(EditAnywhere)
		ECombatAttributesEnum AffectedAttribute;

	UPROPERTY(EditAnywhere)
		int AffectedAmount;

	UPROPERTY(EditAnywhere)
		int LastTickWorldTime;

	FCombatEffectStruct()
	{
		//ID = 0;
		TickInterval = .0f;
		DurationInTicks = 0;
		AffectedAttribute = ECombatAttributesEnum::CAE_MAX;
		AffectedAmount = 0;
	}

	bool operator==(const FCombatEffectStruct& other) const
	{
		return ID == other.ID;
	}

	bool IsExpired() const
	{
		return DurationInTicks < 1;
	}

	bool ShouldTick(int currentTime) const
	{
		return !IsExpired() && (LastTickWorldTime + TickInterval < currentTime);
	}

};

USTRUCT()
struct FCombatVisualEffectStruct : public FHasID
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString Name;

	UPROPERTY(EditAnywhere)
		FString Description;

	UPROPERTY(EditAnywhere)
		UParticleSystem* Particle;

	UPROPERTY(EditAnywhere)
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* VisualComponent;

	FCombatVisualEffectStruct()
	{
		//ID = 0;
		Name = FString::FString();
		Description = FString::FString();
		Particle = NULL;
		Icon = NULL;
		VisualComponent = NULL;
	}

	bool operator==(const FCombatVisualEffectStruct& other) const
	{
		return ID == other.ID;
	}
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECombatTextTypeEnum : uint8
{
	CTTE_Heal		UMETA(DisplayName = "healed"),
	CTTE_Damage		UMETA(DisplayName = "damaged"),
	CTTE_Status		UMETA(DisplayName = "status"),
	CTTE_MAX
};


USTRUCT()
struct FCombatTextStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		ECombatTextTypeEnum Type;

	UPROPERTY(EditAnywhere)
		bool bCritical;

	UPROPERTY(EditAnywhere)
		int Amount;

	FCombatTextStruct()
	{
		Type = ECombatTextTypeEnum::CTTE_MAX;
		Amount = 0;
		bCritical = false;
	}
};

USTRUCT()
struct FAbilityStruct : public FHasID
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TickInterval;

	UPROPERTY(EditAnywhere)
		int DurationInTicks;

	UPROPERTY(EditAnywhere)
		ECombatAttributesEnum AffectedAttribute;

	UPROPERTY(EditAnywhere)
		int AffectedAmount;

	UPROPERTY(EditAnywhere)
		int LastTickWorldTime;

	FAbilityStruct()
	{
		//ID = 0;
		TickInterval = .0f;
		DurationInTicks = 0;
		AffectedAttribute = ECombatAttributesEnum::CAE_MAX;
		AffectedAmount = 0;
	}

	bool operator==(const FCombatEffectStruct& other) const
	{
		return ID == other.ID;
	}

	bool IsExpired() const
	{
		return DurationInTicks < 1;
	}

	bool ShouldTick(int currentTime) const
	{
		return !IsExpired() && (LastTickWorldTime + TickInterval < currentTime);
	}

};