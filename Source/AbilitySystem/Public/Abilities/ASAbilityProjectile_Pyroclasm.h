#pragma once

#include "CoreMinimal.h"
#include "ASAbilityProjectile.h"
#include "ASAbilityProjectile_Pyroclasm.generated.h"


UCLASS()
class ABILITYSYSTEM_API UASAbilityProjectile_Pyroclasm : public UASAbilityProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float DetectionRadius;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	int MaxBounceCount;

	UPROPERTY()
	int CurrentBounceCount;

	virtual void OnHitTargetCallback(AActor* HitActor, FVector NormalImpulse, const FHitResult& HitResult, AActor* Projectile) override;
	
public:
	UFUNCTION()
	void SetCurrentBounceCount(int NewBounceCount)
	{
		CurrentBounceCount = NewBounceCount;
	}

};
