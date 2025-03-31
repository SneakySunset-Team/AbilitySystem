#pragma once

#include "CoreMinimal.h"
#include "ASAbility.h"
#include "ASAbilityProjectile.generated.h"

class AASProjectile;

UCLASS()
class ABILITYSYSTEM_API UASAbilityProjectile : public UASAbility
{
	GENERATED_BODY()

	FOnAbilityTrigger OnProjectileHit;
	FOnAbilityTrigger OnProjectileHitAttributsManager;
	FOnAbilityTrigger OnProjectileReachMaxDistance;
	
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	TSubclassOf<AASProjectile> ProjectilePrefab;

	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<USoundBase> OnHitSound;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float ProjectileMaxDistance;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<UParticleSystem> OnHitParticle;
	
	virtual void InitializeDuplicate(AASCharacter* InOwner) override;
	
	virtual void OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;

	UFUNCTION()
	virtual void OnHitTargetCallback(AActor* HitActor, FVector NormalImpulse, const FHitResult& HitResult, AActor* Projectile);

	UFUNCTION()
	virtual void OnProjectileMaxDistanceReachedCallback();
};
