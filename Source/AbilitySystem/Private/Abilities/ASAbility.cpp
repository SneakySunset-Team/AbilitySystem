#include "Abilities/ASAbility.h"
#include "ASAttributs.h"
#include "AbilitySystem/Public/CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Effects/ASEffect.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UASAbility::Initialize(AASCharacter* Owner, bool IsPersistantAbility)
{
	OwningCharacter = Owner;
	if (IsPersistantAbility)
	{
		OwningCharacter_AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
		ManaCostEffect = NewObject<UASEffect>(Owner, ManaCostEffectPrefab);
		ManaCostEffect->Initialize(OwningCharacter->GetAttributsManager()->GetAttributs());
	}
	else
	{
		for (const auto EffectPrefab : EffectsPrefabs)
		{
			Effects.Add(NewObject<UASEffect>(Owner, EffectPrefab));
			Effects.Last()->Initialize(OwningCharacter->GetAttributsManager()->GetAttributs());
		}
	}
}

void UASAbility::StartCasting()
{
	// In case of wanting to implement a cooldown reduction system I would change
	// this timer to be in a Tick Function with a multiplier affected by a haste parameter in the attributs
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UASAbility::OnCooldownEnd, Cooldown, false, -1);
	IsInCooldown = true;

	UGameplayStatics::PlaySoundAtLocation(this, OnStartCasting_Sound, OwningCharacter->GetActorLocation());	
	
	OwningCharacter_AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UASAbility::EndCasting);
	OwningCharacter_AnimInstance->Montage_Play(OnStartCasting_AnimMontage);
	
	ManaCostEffect->ApplyEffect(OwningCharacter->GetAttributsManager());
}

void UASAbility::EndCasting(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
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
	AbilityInstance->Initialize(OwningCharacter, false);
	return AbilityInstance;
}

void UASAbility::TriggerAbilityEffects(UASAttributsManager* TargetAttributsManager)
{
	for (const auto Effect : Effects)
	{
		Effect->ApplyEffect(TargetAttributsManager);
	}
}
