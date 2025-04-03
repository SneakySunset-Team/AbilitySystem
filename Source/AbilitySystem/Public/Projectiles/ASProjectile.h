#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASProjectile.generated.h"

class UASAttributsManager;

DECLARE_DYNAMIC_DELEGATE_FourParams(FOnHitDelegate, AActor*, OtherActor, FVector, NormalImpulse, const FHitResult&, Hit, AActor*, Projectile);
DECLARE_DYNAMIC_DELEGATE(FOnMaxDistanceReached);

UCLASS()
class ABILITYSYSTEM_API AASProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="", meta=(AllowPrivateAccess))
	TObjectPtr<UMeshComponent> MeshComponent;

    // Cached in location of the Position where the projectile is Spawned
	UPROPERTY()
	FVector StartingLocation;
	
    // Cached in max distance before the projectile is destroyed
	UPROPERTY()
	float MaxDistance;

    // Cached in Speed of the projectile
	UPROPERTY()
	float Speed;

    // Counter of the current distance crossed by this projectile
	UPROPERTY()
	float DistanceCrossed;
	
public:
	AASProjectile();

    // Delegates triggered when the projectile reaches max distance or hits a
    // target
	FOnHitDelegate OnHitDelegate;
	FOnMaxDistanceReached OnMaxDistanceReachedDelegate;

    /**
     * Cache in Speed, StartingLocation and MaxDistance
     */
	UFUNCTION()
	virtual void Initialize(float InMaxDistance, float InSpeed);
	
protected:
	virtual void BeginPlay() override;

    /**
     * Moves projectile forward and if the max distance is reached call delegate
     * @param DeltaTime Time elapsed since last frame
     */
public:
	virtual void Tick(float DeltaTime) override;

    /**
     * Once the MeshComponent hits an object send OnHit delegate to related Ability through delegate
     */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
