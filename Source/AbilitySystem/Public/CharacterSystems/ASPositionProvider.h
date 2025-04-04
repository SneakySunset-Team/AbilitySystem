// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ASPositionProvider.generated.h"

UINTERFACE()
class UASPositionProvider : public UInterface
{
	GENERATED_BODY()
};

class ABILITYSYSTEM_API IASPositionProvider
{
	GENERATED_BODY()

public:
	virtual bool GetTargetPosition(FVector& InWorldLocation, FVector& InWorldDirection, FHitResult& HitResult, ECollisionChannel TraceChannel = ECC_Visibility) = 0;
};
