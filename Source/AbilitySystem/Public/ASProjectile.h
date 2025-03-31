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

	UPROPERTY()
	FVector StartingLocation;
	
	UPROPERTY()
	float MaxDistance;

	UPROPERTY()
	float Speed;

	UPROPERTY()
	float DistanceCrossed;
	
public:
	AASProjectile();

	FOnHitDelegate OnHitDelegate;
	FOnMaxDistanceReached OnMaxDistanceReachedDelegate;

	UFUNCTION()
	virtual void Initialize(float InMaxDistance, float InSpeed);
	
protected:
	virtual void BeginPlay() override;

	
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
