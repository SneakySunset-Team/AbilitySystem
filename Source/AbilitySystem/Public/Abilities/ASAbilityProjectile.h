#pragma once

#include "CoreMinimal.h"
#include "ASAbility.h"
#include "ASAbilityProjectile.generated.h"

class AASProjectile;

UCLASS()
class ABILITYSYSTEM_API UASAbilityProjectile : public UASAbility
{
	GENERATED_BODY()

public:
    // Delegates called in duplicated Ability. The associated callbacks create effects and
    // applies them to either the target or the player
	FOnAbilityTrigger OnProjectileHit;
	FOnAbilityTrigger OnProjectileHitAttributsManager;
	FOnAbilityTrigger OnProjectileReachMaxDistance;

protected:
    // The Projectile that will be spawned when the Animation Notify is triggered
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	TSubclassOf<AASProjectile> ProjectilePrefab;

    // The sound that will be played once the Projectile hits a target
	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<USoundBase> OnHitSound;

    // The Distance at which the projectile will be destroyed
    // This is only useful if the ability is not targetted
	UPROPERTY(EditAnywhere, Category = "AS|Stats", meta = (EditConditionHides = "!IsTargettedAbility"))
	float ProjectileMaxDistance;

    // The speed of the projectile
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float ProjectileSpeed;

    // The particles that will be played once the Projectile hits a target
	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<UParticleSystem> OnHitParticle;

public:
    /**
     * Cache in reference to Owning Character for the persistant Ability
     * Add Function ApplyEffects to animation delegates
     * @param InOwner The reference to the Owning Character
     */
	virtual void InitializePersistant(AASCharacter* InOwner) override;
	
    /**
     * Cache in reference to Owning Character for the duplicate Ability
     * Add Function ApplyEffects to projectile delegates
     * @param InOwner The reference to the Owning Character
     */
	virtual void InitializeDuplicate(AASCharacter* InOwner) override;
	
    /**
     * Bind Animation Callback Methods to the animInstance
     * Start Animation Montage and Play Sound
     * Rotate Character towards mouse position
     * Start Cooldown Timer and Edit Caster Mana with negative Mana Cost
     */
	virtual void StartCasting() override;
	
    /**
     * If the notify has the right name ("Cast") play OnCastAnimationTrigger Effects
     * Spawns the projectile and creates the Ability Duplicate instance
     * @param NotifyName the name of the notify that triggered this Callback
     * @param Payload Tbh no idea cba
     */
	virtual void OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;

    /**
     * Callback when the spawned projectile hits something
     * Will trigger delegates and check if the target has a AttributsManager component
     * @param HitActor the Actor that has been hit by the projectile
     * @param NormalImpulse the Normal of the collision between the hit actor and the projectile
     * @param HitResult The RayTraceHit data of the collision between the hit actor and the projectile
     * @param Projectile A pointer to the Projectile
     */
	UFUNCTION()
	virtual void OnHitTargetCallback(AActor* HitActor, FVector NormalImpulse, const FHitResult& HitResult, AActor* Projectile);

    /**
     * Callback when the projectile has reached its max distance
     */
	UFUNCTION()
	virtual void OnProjectileMaxDistanceReachedCallback();

    /**
     * Returns the projectile speed value
     */
	UFUNCTION()
	float GetProjectileSpeed()
	{
		return ProjectileSpeed;
	}
};
