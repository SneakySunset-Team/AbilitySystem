#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASEffect.generated.h"


class UASAttributs;

UENUM(BlueprintType)
enum class ETriggerType : uint8
{
	OnStartCasting,
	OnAnimationTriggerEvent,
	OnEndAnimation,
	OnHit,
	//Applies when the hit object has an UASAttributsManager
	OnHitTarget,
	OnProjectileReachMaxDistance
};

UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEM_API UASEffect : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ETriggerType TriggerType;
	
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
	
	UFUNCTION()
	ETriggerType GetTriggerType()
	{
		return TriggerType;
	}
};
