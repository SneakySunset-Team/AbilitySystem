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
	UPROPERTY()
	TObjectPtr<UASAttributsManager> TargetAttributManager;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void InitializeTargettedProjectile(UASAttributsManager* InTargetAttributManager, float InSpeed);

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
