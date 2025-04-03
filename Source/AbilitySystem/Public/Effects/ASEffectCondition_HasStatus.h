#pragma once

#include "CoreMinimal.h"
#include "ASEffectCondition.h"
#include "ASAttributs.h"
#include "ASEffectCondition_HasStatus.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ABILITYSYSTEM_API UASEffectCondition_HasStatus : public UASEffectCondition
{
	GENERATED_BODY()
		
protected:
    // The Status that will be checked on the provided Target AttribtusManager
	UPROPERTY(EditAnywhere)
	EStatus TargetStatus;
	
public:
    /**
     * returns weither the Target AttributsManager has the TargetStatus in its active effects
     * @param InTargetAttributsManager The Target AttributsManager of the Effect that called this function
     */
	virtual bool GetConditionValidation(UASAttributsManager* InTargetAttributsManager) override;
};
