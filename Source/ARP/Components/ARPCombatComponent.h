// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Data/CombatData.h"
#include "ARPCombatComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARP_API UARPCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//===================================================
	// Replicated 
	//===================================================
	UPROPERTY(EditAnywhere, Replicated, Category = Combat)
		TArray<int> BaseCombatAttributes;

	UPROPERTY(EditAnywhere, Replicated, Category = Combat)
		TArray<int> ModCombatAttributes;

	//Store the ID of the owning combat effect, remove on expire of effect
	UPROPERTY(EditAnywhere, Replicated, Category = CombatEffects)
		TArray<int> StatusEffects;

	UPROPERTY(EditAnywhere, Category = CommbatEffect, ReplicatedUsing = OnRep_CombatTextQueue)
		TArray<FCombatTextStruct> CombatTextQueue;

	//===================================================
	// Not replicated
	//===================================================
	UPROPERTY(EditAnywhere, Category = CombatEffects)
		TArray<FCombatEffectStruct> ActiveCombatEffects;

	UPROPERTY(EditAnywhere, Category = CombatEffects)
		TArray<FCombatVisualEffectStruct> ActiveCombatVisualEffects;

public:
	// Sets default values for this component's properties
	UARPCombatComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ApplyEffect(int EffectID);

	virtual void ExpireEffect(int EffectID);

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_ApplyEffect(int EffectID);

	UFUNCTION(NetMulticast, Reliable)
		virtual void Client_ApplyEffect(int EffectID);


	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_ExpireEffect(int EffectID);

	UFUNCTION(NetMulticast, Reliable)
		virtual void Client_ExpireEffect(int EffectID);

	UFUNCTION()
		virtual void OnRep_CombatTextQueue();


protected:
	//UFUNCTION(Server, Reliable)
	virtual void Server_Tick_CombatEffects(float DeltaTime);
	virtual void Server_Expire_CombatEffects(TArray<int> EffectIDs);

	virtual int GetVisualEffectByID(int EffectID);
	virtual int GetEffectByID(int EffectID);

	//UFUNCTION(Server, Reliable, WithValidation)
	//	virtual void Server_ExpireEffects(const TArray<int> EffectIDs);

	//UFUNCTION(NetMulticast, Unreliable)
	//	virtual void Client_ExpireEffects(const TArray<int> EffectIDs);
};

