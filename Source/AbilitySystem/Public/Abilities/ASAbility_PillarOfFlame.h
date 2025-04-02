#pragma once

#include "CoreMinimal.h"
#include "ASAbility.h"
#include "ASAbilityProjectile.h"
#include "ASAbility_PillarOfFlame.generated.h"

UCLASS()
class ABILITYSYSTEM_API UASAbility_PillarOfFlame : public UASAbility
{
	GENERATED_BODY()

	FOnAbilityTrigger OnFlamePillarHitTarget;
	
protected:
	UPROPERTY(EditAnywhere)
	float FlamePillarRadius;

	UPROPERTY(EditAnywhere)
	float FlamePillarDelayBeforeDamage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> VisualZonePrefab;

	UPROPERTY()
	TObjectPtr<AActor> VisualZone;
	
	UPROPERTY()
	FVector MouseTarget;
	
public :
	virtual void InitializePersistant(AASCharacter* InOwner) override;

	virtual void StartCasting() override;
	
	virtual void OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;

	UFUNCTION()
	void ApplyFlamePillar();
};
