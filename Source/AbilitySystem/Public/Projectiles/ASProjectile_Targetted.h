// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASProjectile.h"
#include "ASProjectile_Targetted.generated.h"

UCLASS()
class ABILITYSYSTEM_API AASProjectile_Targetted : public AASProjectile
{
	GENERATED_BODY()

public:
	AASProjectile_Targetted();

protected:
    // Cached in Target AttributsManager
	UPROPERTY()
	TObjectPtr<UASAttributsManager> TargetAttributManager;
	
protected:
	virtual void BeginPlay() override;

public:
    /**
     * Rotate towards cached in Target Attributsmanager and then move forward
     * If the distance between the projectile and the target is < 50 call the on hit delegate
     * @param DeltaTime Elapsed Time since last frame
     */
	virtual void Tick(float DeltaTime) override;

    /**
     * cached in Target AttributsManager and speed
     */
	UFUNCTION()
	virtual void InitializeTargettedProjectile(UASAttributsManager* InTargetAttributManager, float InSpeed);

    /**
     * DO NOTHING
     */
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
