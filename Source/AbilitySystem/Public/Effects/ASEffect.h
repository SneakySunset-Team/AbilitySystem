#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASEffect.generated.h"


enum class EStat : uint8;
class UASAttributs;

UENUM(BlueprintType)
enum class EASActivationType : uint8
{
	OnStartCasting,
	OnAnimationTriggerEvent,
	OnEndAnimation,
	OnHit,
	//Applies when the hit object has an UASAttributsManager
	OnHitTarget,
	OnProjectileReachMaxDistance
};

USTRUCT(BlueprintType)
struct FASSingleStat
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStat ScalingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsScalingOnCaster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditiveValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MultiplicativeValue;
};


UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEM_API UASEffect : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EStat TargetType;
	
	UPROPERTY(EditAnywhere)
	EASActivationType ActivationType;

	UPROPERTY(EditAnywhere)
	TArray<FASSingleStat> Stats;
	
	UPROPERTY()
	TObjectPtr<UASAttributs> CasterAttributs;

public:
	UFUNCTION()
	void Initialize(UASAttributs* InCasterAttributs);

	UFUNCTION()
	virtual void ApplyEffect(UASAttributsManager* InTargetAttributsManager);
	
	UFUNCTION()
	EASActivationType GetActivationType() const
	{
		return ActivationType;
	}
};
