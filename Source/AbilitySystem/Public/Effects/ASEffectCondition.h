// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASEffectCondition.generated.h"

class UASAttributsManager;

UCLASS(Abstract)
class ABILITYSYSTEM_API UASEffectCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual bool GetConditionValidation(UASAttributsManager* InTargetAttributsManager);
};

