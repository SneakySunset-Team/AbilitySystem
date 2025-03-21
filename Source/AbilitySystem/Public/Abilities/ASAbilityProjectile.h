#pragma once

#include "CoreMinimal.h"
#include "ASAbility.h"
#include "ASAbilityProjectile.generated.h"

class AASProjectile;

UCLASS()
class ABILITYSYSTEM_API UASAbilityProjectile : public UASAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	TSubclassOf<AASProjectile> ProjectilePrefab;

	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<USoundBase> OnHitSound;

	virtual void Initialize(AASCharacter* InOwner, bool IsPersistantAbility) override;

	virtual void EndCasting(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;
};
