#pragma once

#include "CoreMinimal.h"
#include "ASEffectCondition_HasStatus.h"
#include "UObject/Object.h"
#include "ASEffect.generated.h"


class UASAttributsManager;
enum class EStat : uint8;
struct FASAttributs;
class UASEffectCondition;

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
	// The Stat type that will be multiplied by the Multiplicative value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStat ScalingType;

    // Weither the Stat multiplier should target the caster or the Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsScalingOnCaster;

    // The base value of the Stat Change
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditiveValue;

    // The multiplicative value of the Stat Change
    // Multiplied with the Caster's or Target's Attributs
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MultiplicativeValue;

};


UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEM_API UASEffect : public UObject
{
	GENERATED_BODY()

    // The Stat type that will be targetted by this effect on the Target Attributs
	UPROPERTY(EditAnywhere)
	EStat TargetType;
	
    // The Type of Event that will trigger this Effect on the target
	UPROPERTY(EditAnywhere)
	EASActivationType ActivationType;

    // The Data of the effects (ScalingType, Additive, Multipliers, Target or Caster attributs)
	UPROPERTY(EditAnywhere)
	TArray<FASSingleStat> Stats;
	
    // The cached pointer to Caster AttributsManager
	UPROPERTY()
	TObjectPtr<UASAttributsManager> CasterAttributsManager;

    // The Conditions to apply this effect on the target AttributsManager
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UASEffectCondition>> Conditions;
	
public:
    /**
     * Cache in the Caster AttributsManager
     * @param InCasterAttributsManager AttributsManager to cache in
     */
	UFUNCTION()
	void Initialize(UASAttributsManager* InCasterAttributsManager);

    /**
     * Edit the Stats of the InTargetAttributsManager
     * @param InTargetAttributsManager Target AttributsManager
     */
	UFUNCTION()
	virtual void ApplyEffect(UASAttributsManager* InTargetAttributsManager);

    /**
     * Weither the conditions are valid to apply this effect on the target attributs manager
     * @param InTargetAttributsManager Target AttributsManager
     */
	UFUNCTION()
	bool CanApplyEffect(UASAttributsManager* InTargetAttributsManager);
	
    /**
     * returns the ActivationType of this Effect
     */
	UFUNCTION()
	EASActivationType GetActivationType() const
	{
		return ActivationType;
	}
};
