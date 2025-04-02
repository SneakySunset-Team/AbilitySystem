#include "AbilitySystem/Public/CharacterSystems/ASAttributsManager.h"

UASAttributsManager::UASAttributsManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UASAttributsManager::BeginPlay()
{
	Super::BeginPlay();

	if (Attributs.StatsContains(EStat::MaxHealth))
	{
		Attributs.Stats.FindOrAdd(EStat::Health);
		Attributs.Stats[EStat::Health] = Attributs.Stats[EStat::MaxHealth];
	}

	if (Attributs.StatsContains(EStat::MaxMana))
	{
		Attributs.Stats.FindOrAdd(EStat::Mana);
		Attributs.Stats[EStat::Mana] = Attributs.Stats[EStat::MaxMana];
	}

	for (const auto& EffectPrefab : PassiveEffects)
	{
		UASLingeringEffect* Effect = NewObject<UASLingeringEffect>(this, EffectPrefab);
		Attributs.ActiveEffects.Add(Effect);
		Effect->ApplyEffect(this);
	}
	
}

void UASAttributsManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UASAttributsManager::AddLingeringEffect(UASLingeringEffect* LingeringEffect)
{
	if (LingeringEffect->GetStatus() != EStatus::None &&  !GetHasStatus(LingeringEffect->GetStatus()))
	{
		OnStatusAdded.Broadcast(LingeringEffect->GetStatus());
	}
	Attributs.ActiveEffects.Add(LingeringEffect);
}

void UASAttributsManager::RemoveLingeringEffect(UASLingeringEffect* LingeringEffect)
{
	if (Attributs.ActiveEffects.Contains(LingeringEffect))
	{
		OnStatusRemoved.Broadcast(LingeringEffect->GetStatus());
		Attributs.ActiveEffects.Remove(LingeringEffect);
	}
}

void UASAttributsManager::EditStat(EStat InStat, float InValue)
{
	if (!Attributs.StatsContains(InStat))
		return;
	
	Attributs.Stats[InStat] += InValue;
	
	if (InStat == EStat::Health)
	{
		if (Attributs.Stats[InStat] <= 0.0f)
		{
			GetOwner()->Destroy();
			return;
		}
		Attributs.Stats[InStat] = FMath::Clamp(Attributs.Stats[InStat], 0.0f, Attributs.Stats[EStat::MaxHealth]);
	}

	if (InStat == EStat::Mana)
	{
		Attributs.Stats[InStat] = FMath::Clamp(Attributs.Stats[InStat], 0.0f, Attributs.Stats[EStat::MaxMana]);
	}
}

float UASAttributsManager::GetHealthPercent()
{
	if (!Attributs.StatsContains(EStat::MaxHealth))
	{
		return 0.0f;	
	}
	else
	{
		return Attributs.GetStat(EStat::Health) / Attributs.GetStat(EStat::MaxHealth);
	}
}

float UASAttributsManager::GetManaPercent()
{
	if (!Attributs.StatsContains(EStat::MaxMana))
	{
		return 0.0f;	
	}
	else
	{
		return Attributs.GetStat(EStat::Mana) / Attributs.GetStat(EStat::MaxMana);
	}
}

