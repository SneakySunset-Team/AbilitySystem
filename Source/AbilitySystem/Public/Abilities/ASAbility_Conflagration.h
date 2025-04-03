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

    // The Blueprint Ability that will be bound to the ProjectileSpreadFire
    // will be relevant if the target is already burning
	UPROPERTY(EditAnywhere,  Category = "AS|Stats")
	TSubclassOf<UASAbilityProjectile> AbilitySpreadFire;

    // The Blueprint of the Targetted projectiles that will go towards nearby targets
    // if the first hit ennemy is burning
	UPROPERTY(EditAnywhere,  Category = "AS|Stats")
	TSubclassOf<AASProjectile_Targetted> ProjectileSpreadFire;
	
    // The radius around the first hit target that will overlap the eligable nearby targets on hit
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float DetectionRadius;

    /**
     * If the notify has the right name ("Cast") play OnCastAnimationTrigger Effects
     * Applies Ability Effects to Target here (No duplicates)
     * If Target is burning throw projectiles towards nearby ennemy AttributsManagers
     * @param NotifyName the name of the notify that triggered this Callback
     * @param Payload Tbh no idea cba
     */
	virtual void OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;

    /**
     * Duplicate the current ability and bind it to NewOwner
     * @param NewOwner The owner of the duplicated ability, Most of the time is a projectile
     */
	virtual UASAbility* CreateAbilityInstance(AActor* NewOwner) override;
};
