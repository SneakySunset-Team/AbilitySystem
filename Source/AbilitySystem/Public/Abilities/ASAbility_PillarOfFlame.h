#pragma once

#include "CoreMinimal.h"
#include "ASAbility.h"
#include "ASAbilityProjectile.h"
#include "ASAbility_PillarOfFlame.generated.h"

UCLASS()
class ABILITYSYSTEM_API UASAbility_PillarOfFlame : public UASAbility
{
	GENERATED_BODY()

    // Delegate called in persistant Ability. The associated callbacks create effects and
    // applie them to either the AttributsManagers hit by the ability
	FOnAbilityTrigger OnFlamePillarHitTarget;
	
protected:
    // Radius of the damage zone of the ability
	UPROPERTY(EditAnywhere)
	float FlamePillarRadius;

    // Time before the the ability applies its effects after the animation montage notify
	UPROPERTY(EditAnywhere)
	float FlamePillarDelayBeforeDamage;

    // The blueprint of the Actor that visualizes the Damage zone of the ability
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> VisualZonePrefab;

    // Cached in pointer to the actor that visualizes the Damage zone of the ability
	UPROPERTY()
	TObjectPtr<AActor> VisualZone;
	
    // The Position where the Damage zone will be applied
	UPROPERTY()
	FVector MouseTarget;
	
public :
    /**
     * Cache in reference to Owning Character for the persistant Ability
     * Add Function ApplyEffects to Flame Pillar Damage delegates
     * @param InOwner The reference to the Owning Character
     */
	virtual void InitializePersistant(AASCharacter* InOwner) override;

    /**
     * Bind Animation Callback Methods to the animInstance
     * Start Animation Montage and Play Sound
     * Rotate Character towards mouse position
     * Start Cooldown Timer and Edit Caster Mana with negative Mana Cost
     * Cache in the mouse position
     */
	virtual void StartCasting() override;
	
    /**
     * If the notify has the right name ("Cast") play OnCastAnimationTrigger Effects
     * Starts the timer to apply the effects in the damage zone after FlamePillarDelayBeforeDamage
     * The Callback is ApplyFlamePillar
     * @param NotifyName the name of the notify that triggered this Callback
     * @param Payload Tbh no idea cba
     */
	virtual void OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload) override;

    /**
     * Gets all Ennemy AttributsManagers in the FlamePillarRadius  around MouseTarget and apply effects on them
     */
	UFUNCTION()
	void ApplyFlamePillar();
};
