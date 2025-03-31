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
	UPROPERTY(EditAnywhere)
	EStatus TargetStatus;
	
public:
	virtual bool GetConditionValidation(UASAttributsManager* InTargetAttributsManager) override;
};
