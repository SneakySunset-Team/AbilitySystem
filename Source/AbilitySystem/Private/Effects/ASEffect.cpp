#include "AbilitySystem/Public/Effects/ASEffect.h"

#include "ASAttributs.h"
#include "CharacterSystems/ASAttributsManager.h"

void UASEffect::Initialize(UASAttributs* InCasterAttributs)
{
	CasterAttributs = InCasterAttributs;
}

void UASEffect::ApplyEffect(UASAttributsManager* InTargetAttributsManager)
{
	float TotalValue = 0;
	for (const auto Stat : Stats)
	{
		UASAttributs* BaseAttributs = Stat.IsScalingOnCaster ? CasterAttributs.Get() : InTargetAttributsManager->GetAttributs();
		float BaseValue = BaseAttributs->StatsContains(Stat.ScalingType) ? BaseAttributs->GetStat(Stat.ScalingType) : 0;
		TotalValue += BaseValue * Stat.MultiplicativeValue + Stat.AdditiveValue;
		
	}
	InTargetAttributsManager->EditStat(TargetType, TotalValue);
}
