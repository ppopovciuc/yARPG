// Fill out your copyright notice in the Description page of Project Settings.

#include "ARP.h"
#include "Data/GameDataSingleton.h"
#include "ARPAbilitiesComponent.h"


// Sets default values for this component's properties
UARPAbilitiesComponent::UARPAbilitiesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UARPAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARPAbilitiesComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}



void UARPAbilitiesComponent::UseAbility(int AbilityID)
{
	if (GetOwner()->Role < ROLE_Authority)
	{
		Server_UseAbility(AbilityID);
	}
	else
	{
		Client_UseAbility(AbilityID);
	}
}

void UARPAbilitiesComponent::Server_UseAbility_Implementation(int AbilityID)
{
	// Do something here that modifies game state.
	UE_LOG(LogTemp, Warning, TEXT("Server_ApplyEffect %s %"), *AActor::GetDebugName(GetOwner()));
	UGameDataSingleton* DataManager = Cast<UGameDataSingleton>(GEngine->GameSingleton);
	FAbilityStruct Effect = DataManager->GetAbility(AbilityID);

	Effect.LastTickWorldTime = GetCurrentTime();


	// Then apply it on all clients
	UseAbility(AbilityID);
}

bool UARPAbilitiesComponent::Server_UseAbility_Validate(int AbilityID)
{
	// Optionally validate the request and return false if the function should not be run.
	return true;
}

/*
* Note: spawn all visual effects (UI, particles, etc)
*/
void UARPAbilitiesComponent::Client_UseAbility_Implementation(int AbilityID)
{
	UGameDataSingleton* DataManager = Cast<UGameDataSingleton>(GEngine->GameSingleton);
	if (DataManager != NULL)
	{
		FCombatVisualEffectStruct Effect = DataManager->GetCombatVisualEffect(AbilityID);
		//Basically refresh
		//ActiveCombatVisualEffects.Remove(Effect);


		UE_LOG(LogTemp, Warning, TEXT("Apply Effect - visual %d %"), Effect.ID);
		USceneComponent* RootComponent = GetOwner()->GetRootComponent();

		UParticleSystemComponent* ParticleTemp = UGameplayStatics::SpawnEmitterAttached(
			Effect.Particle,
			RootComponent,
			NAME_None,
			GetOwner()->GetActorLocation(),
			GetOwner()->GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			false);

		Effect.VisualComponent = ParticleTemp;
		//ActiveCombatVisualEffects.Add(Effect);

	}
	// Do something here to affect the client. This method was called by the server ONLY.
}

