#include "Effects/ASEffectCondition_HasStatus.h"
#include "CharacterSystems/ASAttributsManager.h"

bool UASEffectCondition_HasStatus::GetConditionValidation(UASAttributsManager* InTargetAttributsManager)
{
	return InTargetAttributsManager->GetHasStatus(TargetStatus);
}
