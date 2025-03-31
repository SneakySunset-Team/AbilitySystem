#include "Abilities/ASAbility.h"
#include "ASAttributs.h"
#include "AbilitySystem/Public/CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Effects/ASEffect.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UASAbility::InitializePersistant(AASCharacter* InOwner)
{
	OwningCharacter = InOwner;
	
	for (const auto EffectPrefab : EffectsPrefabs)
	{
		FOnAbilityTrigger& EffectDelegate = OnCastAnimationStart; 

		const UASEffect* DefaultEffect = EffectPrefab->GetDefaultObject<UASEffect>();
		
		bool IsCastFromPersistant = true;
		switch (DefaultEffect->GetActivationType())
		{
			case EASActivationType::OnStartCasting:
				break;
			case EASActivationType::OnAnimationTriggerEvent:
				EffectDelegate = OnCastAnimationTrigger;
				break;
			case EASActivationType::OnEndAnimation:
				EffectDelegate = OnCastAnimationEnd;
				break;
			default:
				IsCastFromPersistant = false;
				break;
		}

		if (IsCastFromPersistant)
		{
			UASEffect* Effect = NewObject<UASEffect>(InOwner, EffectPrefab);
			Effect->Initialize(OwningCharacter->GetAttributsManager()->GetAttributs());
			EffectDelegate.AddDynamic(Effect, &UASEffect::ApplyEffect);
		}
	}

	UAnimInstance* OwningCharacter_AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();

	OwningCharacter_AnimInstance->OnMontageStarted.AddDynamic(this, &UASAbility::OnAnimationStartCallback);
	OwningCharacter_AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UASAbility::OnTriggerAnimationEventCallback);
	OwningCharacter_AnimInstance->OnMontageEnded.AddDynamic(this, &UASAbility::OnAnimationEndCallback);
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
	OwningCharacter_AnimInstance->Montage_Play(OnStartCasting_AnimMontage);
}

void UASAbility::OnAnimationStartCallback(UAnimMontage* Montage)
{
	if (Montage == OnStartCasting_AnimMontage)
	{
		OnCastAnimationStart.Broadcast(OwningCharacter->GetAttributsManager());
		IsInCooldown = true;
		OwningCharacter->GetCharacterMovement()->Deactivate();
	}
}

void UASAbility::OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;

	// Get the active montage instance
	FAnimMontageInstance* ActiveMontageInstance = AnimInstance->GetActiveMontageInstance();
    
	// Check if it's our montage
	if (ActiveMontageInstance && ActiveMontageInstance->Montage == OnStartCasting_AnimMontage)
	{
		if (NotifyName == "Cast")
		{
			UE_LOG(LogTemp, Display, TEXT("Cast triggered"));
			OnCastAnimationTrigger.Broadcast(OwningCharacter->GetAttributsManager());
		}
	}
}

void UASAbility::OnAnimationEndCallback(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == OnStartCasting_AnimMontage)
	{
		if (!bInterrupted && Montage == OnStartCasting_AnimMontage)
		{
			OnCastAnimationEnd.Broadcast(OwningCharacter->GetAttributsManager());
			OwningCharacter->GetCharacterMovement()->Activate();
		}
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
