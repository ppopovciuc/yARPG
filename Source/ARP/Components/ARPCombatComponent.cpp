// Fill out your copyright notice in the Description page of Project Settings.

#include "ARP.h"
#include "Data/GameDataSingleton.h"
#include "ARPCombatComponent.h"

// Sets default values for this component's properties
UARPCombatComponent::UARPCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	int StatusEffectsCount = static_cast<int>(EStatusEffectsEnum::SEE_MAX);
	StatusEffects.SetNumZeroed(StatusEffectsCount, false);

	int CombatAttributesCount = static_cast<int>(ECombatAttributesEnum::CAE_MAX);
	BaseCombatAttributes.SetNumZeroed(CombatAttributesCount, false);
	ModCombatAttributes.SetNumZeroed(CombatAttributesCount, false);

	FCombatTextStruct CT;
	CombatTextQueue.Add(CT);
	// ...
}


// Called when the game starts
void UARPCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UARPCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TickType == LEVELTICK_All)
	{
		Server_Tick_CombatEffects(DeltaTime);
	}

	// ...
}

void UARPCombatComponent::Server_Tick_CombatEffects(float DeltaTime)
{
	if (GetOwner()->Role == ROLE_Authority)
	{
		TArray<int> ExpiredEffects;

		//USolusDataSingleton* DataManager = Cast<USolusDataSingleton>(GEngine->GameSingleton);
		CombatTextQueue.Empty();

		int CurrentTime = GetCurrentTime();
		for (int i = 0; i < ActiveCombatEffects.Num(); i++)
		{
			if (ActiveCombatEffects[i].ShouldTick(CurrentTime))
			{
				ActiveCombatEffects[i].LastTickWorldTime += ActiveCombatEffects[i].TickInterval;
				ActiveCombatEffects[i].DurationInTicks -= 1;

				//Apply stat effects
				FCombatTextStruct CT;
				CT.Amount = rand() % 100;
				CT.bCritical = rand() % 2 == 0;

				int type = rand() % static_cast<int>(ECombatTextTypeEnum::CTTE_MAX);
				CT.Type = static_cast<ECombatTextTypeEnum>(type);

				CombatTextQueue.Add(CT);
				//Add effect to combat text
			}

			if (ActiveCombatEffects[i].IsExpired())
			{
				ExpiredEffects.Add(ActiveCombatEffects[i].ID);
			}
		}

		Server_Expire_CombatEffects(ExpiredEffects);
	}
}

void UARPCombatComponent::ApplyEffect(int EffectID)
{
	if (GetOwner()->Role < ROLE_Authority)
	{
		Server_ApplyEffect(EffectID);
	}
	else {

		Client_ApplyEffect(EffectID);
	}
}

void UARPCombatComponent::Server_ApplyEffect_Implementation(int EffectID)
{
	// Do something here that modifies game state.
	UE_LOG(LogTemp, Warning, TEXT("Server_ApplyEffect %s %"), *AActor::GetDebugName(GetOwner()));
	UGameDataSingleton* DataManager = Cast<UGameDataSingleton>(GEngine->GameSingleton);
	FCombatEffectStruct Effect = DataManager->GetCombatEffect(EffectID);

	Effect.LastTickWorldTime = GetCurrentTime();


	// Basically refresh effect
	ActiveCombatEffects.Remove(Effect);
	ActiveCombatEffects.Add(Effect);

	// Then apply it on all clients
	ApplyEffect(EffectID);
}

bool UARPCombatComponent::Server_ApplyEffect_Validate(int EffectID)
{
	// Optionally validate the request and return false if the function should not be run.
	return true;
}

/*
* Note: spawn all visual effects (UI, particles, etc)
*/
void UARPCombatComponent::Client_ApplyEffect_Implementation(int EffectID)
{

	UGameDataSingleton* DataManager = Cast<UGameDataSingleton>(GEngine->GameSingleton);
	if (DataManager != NULL)
	{
		FCombatVisualEffectStruct Effect = DataManager->GetCombatVisualEffect(EffectID);
		//Basically refresh
		ActiveCombatVisualEffects.Remove(Effect);


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
		ActiveCombatVisualEffects.Add(Effect);

	}
	// Do something here to affect the client. This method was called by the server ONLY.
}

void UARPCombatComponent::ExpireEffect(int EffectID)
{
	if (GetOwner()->Role < ROLE_Authority)
	{
		Server_ExpireEffect(EffectID);
	}

	Client_ExpireEffect(EffectID);
}

void UARPCombatComponent::Server_ExpireEffect_Implementation(int EffectID)
{
	// Do something here that modifies game state.
	UE_LOG(LogTemp, Warning, TEXT("Server_ExpireEffect %s %"), *AActor::GetDebugName(GetOwner()));

	UGameDataSingleton* DataManager = Cast<UGameDataSingleton>(GEngine->GameSingleton);
	FCombatEffectStruct Effect = DataManager->GetCombatEffect(EffectID);

	ActiveCombatEffects.Remove(Effect);

	// Then apply it on all clients
	ExpireEffect(EffectID);
}

bool UARPCombatComponent::Server_ExpireEffect_Validate(int EffectID)
{
	// Optionally validate the request and return false if the function should not be run.
	return true;
}

void UARPCombatComponent::Client_ExpireEffect_Implementation(int EffectID)
{
	UGameDataSingleton* DataManager = Cast<UGameDataSingleton>(GEngine->GameSingleton);

	FCombatVisualEffectStruct Effect = DataManager->GetCombatVisualEffect(EffectID);

	int index = GetVisualEffectByID(EffectID);

	if (index >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Expire Effect - visual %d %"), EffectID);
		FCombatVisualEffectStruct EffectToRemove = ActiveCombatVisualEffects[index];
		ActiveCombatVisualEffects.Remove(Effect);

		if (EffectToRemove.VisualComponent != NULL)
		{
			EffectToRemove.VisualComponent->DeactivateSystem();
			EffectToRemove.VisualComponent->DestroyComponent(false);
		}
	}
	// Do something here to affect the client. This method was called by the server ONLY.
}

//===============================================
// Replication
//===============================================
void UARPCombatComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UARPCombatComponent, CombatTextQueue);
	DOREPLIFETIME(UARPCombatComponent, StatusEffects);
	DOREPLIFETIME(UARPCombatComponent, ModCombatAttributes);
	DOREPLIFETIME(UARPCombatComponent, BaseCombatAttributes);
}

void UARPCombatComponent::OnRep_CombatTextQueue()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep - Client %"));

	for (int i = 0; i < CombatTextQueue.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Text - amount: %d - critical: %s - type: na %"), CombatTextQueue[i].Amount, CombatTextQueue[i].bCritical ? "true" : "false");
	}
}

//===============================================
// Internal / helper stuff
//===============================================

void UARPCombatComponent::Server_Expire_CombatEffects(TArray<int> EffectIDs)
{
	if (GetOwner()->Role == ROLE_Authority)
	{
		for (int i = 0; i < EffectIDs.Num(); i++)
		{
			ExpireEffect(EffectIDs[i]);
		}
	}
}


int UARPCombatComponent::GetVisualEffectByID(int EffectID)
{
	for (int i = 0; i < ActiveCombatVisualEffects.Num(); i++)
	{
		if (ActiveCombatVisualEffects[i].ID == EffectID)
		{
			return i;
		}
	}

	return -1;
}

int UARPCombatComponent::GetEffectByID(int EffectID)
{
	for (int i = 0; i < ActiveCombatEffects.Num(); i++)
	{
		if (ActiveCombatEffects[i].ID == EffectID)
		{
			return i;
		}
	}

	return -1;
}
