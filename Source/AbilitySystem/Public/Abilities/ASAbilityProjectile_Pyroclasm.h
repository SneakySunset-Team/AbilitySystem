#pragma once

#include "CoreMinimal.h"
#include "ASAbilityProjectile.h"
#include "ASAbilityProjectile_Pyroclasm.generated.h"


UCLASS()
class ABILITYSYSTEM_API UASAbilityProjectile_Pyroclasm : public UASAbilityProjectile
{
	GENERATED_BODY()
	
protected:
    
    // The radius around hit target that helps find other potential target to bounce towards
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float DetectionRadius;

    // The Max number of bounces before the ability stops bouncing
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	int MaxBounceCount;

    // The current number of bounces
	UPROPERTY()
	int CurrentBounceCount;

    /**
     * Callback when the spawned projectile hits something
     * Will trigger delegates and check if the target has a AttributsManager component
     * If the Max bounce number is not reach retrieve all nearby AttributsManager
     * Then duplicate this ability and make it go towards the nearest Ennemy AttributsManager or the Player if there is no good target
     * @param HitActor the Actor that has been hit by the projectile
     * @param NormalImpulse the Normal of the collision between the hit actor and the projectile
     * @param HitResult The RayTraceHit data of the collision between the hit actor and the projectile
     * @param Projectile A pointer to the Projectile
     */
	virtual void OnHitTargetCallback(AActor* HitActor, FVector NormalImpulse, const FHitResult& HitResult, AActor* Projectile) override;
	
public:
    /**
     * Sets the current Number of bounce
     * Called after it bounces on a target and needs to bounce again
     * @param NewBounceCount
     */
	UFUNCTION()
	void SetCurrentBounceCount(int NewBounceCount)
	{
		CurrentBounceCount = NewBounceCount;
	}

};
