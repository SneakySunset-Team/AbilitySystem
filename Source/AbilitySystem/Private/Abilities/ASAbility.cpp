#include "Abilities/ASAbility.h"
#include "ASAttributs.h"
#include "AbilitySystem/Public/CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Effects/ASEffect.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UASAbility::InitializePersistant(AASCharacter* InOwner)
{
	OwningCharacter = InOwner;
	
	for (const auto EffectPrefab : EffectsPrefabs)
	{
		FOnAbilityTrigger& EffectDelegate = OnCastAnimationStart; 
		
		bool IsCastFromPersistant = true;
		switch (Effects.Last()->GetTriggerType())
		{
			case ETriggerType::OnStartCasting:
				break;
			case ETriggerType::OnAnimationTriggerEvent:
				EffectDelegate = OnCastAnimationTrigger;
				break;
			case ETriggerType::OnEndAnimation:
				EffectDelegate = OnCastAnimationEnd;
				break;
			default:
				IsCastFromPersistant = false;
				break;
		}

		if (IsCastFromPersistant)
		{
			UASEffect* Effect = NewObject<UASEffect>(InOwner, EffectPrefab));
			Effect->Initialize(OwningCharacter->GetAttributsManager()->GetAttributs());
			EffectDelegate.AddDynamic(Effect, UASEffect::ApplyEffect);
		}
	}
}

void UASAbility::InitializeDuplicate(AASCharacter* InOwner)
{
	OwningCharacter = InOwner;
}

void UASAbility::StartCasting()
{
	// In case of wanting to implement a cooldown reduction system I would change
	// this timer to be in a Tick Function with a multiplier affected by a haste parameter in the attributs
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UASAbility::OnCooldownEnd, Cooldown, false, -1);

	UGameplayStatics::PlaySoundAtLocation(this, OnStartCasting_Sound, OwningCharacter->GetActorLocation());	

	UAnimInstance* OwningCharacter_AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();

	OwningCharacter_AnimInstance->OnMontageStarted.AddDynamic(this, &UASAbility::OnAnimationStartCallback);
	OwningCharacter_AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UASAbility::OnTriggerAnimationEventCallback);
	OwningCharacter_AnimInstance->OnMontageEnded.AddDynamic(this, &UASAbility::OnAnimationEndCallback);
	OwningCharacter_AnimInstance->Montage_Play(OnStartCasting_AnimMontage);
}

void UASAbility::OnAnimationStartCallback(UAnimMontage* Montage)
{
	OnCastAnimationStart.Broadcast(OwningCharacter->GetAttributsManager());
	IsInCooldown = true;
}

void UASAbility::OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	OnCastAnimationTrigger.Broadcast(OwningCharacter->GetAttributsManager());
}

void UASAbility::OnAnimationEndCallback(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted && Montage == OnStartCasting_AnimMontage)
	{
		OnCastAnimationEnd.Broadcast(OwningCharacter->GetAttributsManager());
	}
}

bool UASAbility::CanCast()
{
	int CasterMana = CasterAttributs->GetStat(EStat::Mana);
	if (CasterMana >= ManaCost && !IsInCooldown)
	{
		return true;
	}
	return false;
}

void UASAbility::OnCooldownEnd()
{
	IsInCooldown = false;
}

UASAbility* UASAbility::CreateAbilityInstance(AActor* NewOwner)
{
	UASAbility* AbilityInstance = NewObject<UASAbility>(NewOwner, GetClass());
	AbilityInstance->InitializeDuplicate(OwningCharacter);
	return AbilityInstance;
}
