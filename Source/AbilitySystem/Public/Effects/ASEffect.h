#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASEffect.generated.h"


class UASAttributs;

UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEM_API UASEffect : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UASAttributs> AttributsAdditive;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UASAttributs> AttributsMultiplicative;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UASAttributs> CasterAttributs;

public:
	UFUNCTION()
	void Initialize(UASAttributs* InCasterAttributs);

	UFUNCTION()
	void ApplyEffect(UASAttributsManager* InTargetAttributsManager);
};
