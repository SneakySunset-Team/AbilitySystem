#include "AbilitySystem/Public/Effects/ASEffect.h"

#include "ASAttributs.h"
#include "CharacterSystems/ASAttributsManager.h"

void UASEffect::Initialize(UASAttributsManager* InCasterAttributsManager)
{
	CasterAttributsManager = InCasterAttributsManager;
}

void UASEffect::ApplyEffect(UASAttributsManager* InTargetAttributsManager)
{
	
	if (!CanApplyEffect(InTargetAttributsManager))
	{
		return;
	}
	
	float TotalValue = 0;
	for (const auto Stat : Stats)
	{
		FASAttributs& BaseAttributs = Stat.IsScalingOnCaster ? CasterAttributsManager->Attributs : InTargetAttributsManager->Attributs;
		float BaseValue = BaseAttributs.StatsContains(Stat.ScalingType) ? BaseAttributs.GetStat(Stat.ScalingType) : 0;
		TotalValue += BaseValue * Stat.MultiplicativeValue + Stat.AdditiveValue;
		
	}
	InTargetAttributsManager->EditStat(TargetType, TotalValue);
}

bool UASEffect::CanApplyEffect(UASAttributsManager* InTargetAttributsManager)
{
	for (const auto& Condition : Conditions)
	{
		if (!Condition.GetDefaultObject()->GetConditionValidation(InTargetAttributsManager))
		{
			return false;
		}
	}
	return true;
}
