// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASAbility.h"
#include "ASAbility_Conflagration.generated.h"


class AASProjectile_Targetted;
class UASAbilityProjectile;

UCLASS()
class ABILITYSYSTEM_API UASAbility_Conflagration : public UASAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,  Category = "AS|Stats")
	TSubclassOf<UASAbilityProjectile> AbilitySpreadFire;

	UPROPERTY(EditAnywhere,  Category = "AS|Stats")
	TSubclassOf<AASProjectile_Targetted> ProjectileSpreadFire;
	
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float DetectionRadius;

	virtual void OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;

	virtual UASAbility* CreateAbilityInstance(AActor* NewOwner) override;
};
