// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ARPAbilitiesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARP_API UARPAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARPAbilitiesComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void UseAbility(int AbilityID);

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_UseAbility(int AbilityID);

	UFUNCTION(NetMulticast, Reliable)
		virtual void Client_UseAbility(int AbilityID);
};
