#include "AbilitySystem/Public/Effects/ASEffect.h"

#include "ASAttributs.h"
#include "CharacterSystems/ASAttributsManager.h"

void UASEffect::Initialize(UASAttributs* InCasterAttributs)
{
	CasterAttributs = InCasterAttributs;
}

void UASEffect::ApplyEffect(UASAttributsManager* InTargetAttributsManager)
{
	TMap<EStat, float> AppliedAttributs;
	for (const auto Stat : InTargetAttributsManager->GetAttributsStats())
	{
		float AdditiveValue = AttributsAdditive->StatsContains(Stat.Key) ? AttributsAdditive->GetStat(Stat.Key) : 0;
		float BaseValue = CasterAttributs->StatsContains(Stat.Key) ? CasterAttributs->GetStat(Stat.Key) : 0;
		float MultiplicativeValue = AttributsMultiplicative->StatsContains(Stat.Key) ? AttributsMultiplicative->GetStat(Stat.Key) : 0;

		float AppliedDamage = (BaseValue + AdditiveValue) * MultiplicativeValue;
		InTargetAttributsManager->EditStat(Stat.Key,AppliedDamage);
	}
}
