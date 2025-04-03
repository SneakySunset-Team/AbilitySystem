#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASAbility.generated.h"


class UASLingeringEffect;
enum class EASActivationType : uint8;
class AASCharacter;
class UASEffect;
class UASAttributsManager;
struct FASAttributs;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityTrigger, UASAttributsManager*, AttributsManager, EASActivationType, ActivationType);

UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEM_API UASAbility : public UObject
{
	GENERATED_BODY()

protected:
    // Delegates called in persistant Ability. The associated callbacks create effects and
    // applies them to either the target or the player
	FOnAbilityTrigger OnCastAnimationStart;
	FOnAbilityTrigger OnCastAnimationTrigger;
	FOnAbilityTrigger OnCastAnimationEnd;
	
    // The mana cost required to cast the spell. Once Cast
    // it will be edit the mana of the Caster Attributs Manager
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	int ManaCost;

    // How long before the ability can be cast again
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float Cooldown;

    // The name of the bone that is spawn point of the ability's projectile
	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	FName BoneName;

    // Weither the ability needs an Attributs Manager Target to be cast
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	bool IsTargettedAbility;

    // The maximum range at which the ability can be cast when Targetted
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float MaxCastDistance;
	
    // Weither the Ability is currently in cooldown
	UPROPERTY()
	bool IsInCooldown;
	
    // The List of Effects blueprints that can be applied by the ability
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	TArray<TSubclassOf<UASEffect>> EffectsPrefabs;
	
    // The reference to the AttributsManager of the Owning Character
	UPROPERTY()
	TObjectPtr<UASAttributsManager> CasterAttributsManager;

    // The Cached referene to the Target Attributs Manager
    // Mostly used when the ability is Targetted
    UPROPERTY()
    UASAttributsManager* OnCastTargetAttributesManager;
    
    // The animation montage that will be played On Cast
    // It should have a notify with the name "Cast"
	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<UAnimMontage> OnStartCasting_AnimMontage;

    // Sound Played on Cast
	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<USoundBase> OnStartCasting_Sound;

    // Cached reference to the Owning Character of this Ability
	UPROPERTY()
	TObjectPtr<AASCharacter> OwningCharacter;

    // Progression of the cooldown
	UPROPERTY()
	float CurrentTimer;

    // Weither the Played should be stunned while performing the casting animation
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	bool IsSelfStunnedDuringCast;
	
    // The blueprint of the self stun that is applied during the animation
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	TSubclassOf<UASLingeringEffect> SelfStunPrefab;
	
    // Cached reference to the instance of the applied
    // stun effect active during the casting animation
	UPROPERTY()
	TObjectPtr<UASLingeringEffect> SelfStunEffect;

public:
    /**
     * Cache in reference to Owning Character for the persistant Ability
     * Add Function ApplyEffects to animation delegates
     * @param InOwner The reference to the Owning Character
     */
	UFUNCTION()
	virtual void InitializePersistant(AASCharacter* InOwner);

    /**
     * Cache in reference to Owning Character for the duplicate Ability
     * @param InOwner The reference to the Owning Character
     */
	UFUNCTION()
	virtual void InitializeDuplicate(AASCharacter* InOwner);
	
    /**
     * Bind Animation Callback Methods to the animInstance
     * Start Animation Montage and Play Sound
     * Rotate Character towards mouse position
     * Start Cooldown Timer and Edit Caster Mana with negative Mana Cost
     */
	UFUNCTION()
	virtual void StartCasting();

    /**
     * Progress the cooldown timer if IsInCooldown is true
     * @param DelaTime Time elapsed since last frame
     */
	UFUNCTION()
	virtual void Tick(float DelaTime);
	
    /**
     * If the running montage originates from this ability play OnAnimationStartCallback Effects
     * If this IsSelfStunnedDuringCast is true apply the self stun effect to the Caster
     * @param Montage Reference to the montage that triggered this callback
     */
	UFUNCTION()
	virtual void OnAnimationStartCallback(UAnimMontage* Montage);

    /**
     * If the notify has the right name ("Cast") play OnCastAnimationTrigger Effects
     * @param NotifyName the name of the notify that triggered this Callback
     * @param Payload Tbh no idea cba
     */
	UFUNCTION()
	virtual void OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

    /**
     * If the running montage originates from this ability play OnAnimationStartCallback Effects
     * If this IsSelfStunnedDuringCast is true remove the self stun effect from the Caster
     * @param Montage Reference to the montage that triggered this Callback
     * @param bInterrupted Weither this callback originates from the montage beeing interrupted or finished
     */
	UFUNCTION()
	virtual void OnAnimationEndCallback(UAnimMontage* Montage, bool bInterrupted);
	
    /**
     * Checks if the ability is under a cooldown
     * Checks if the Caster has enough mana
     * Checks if the Caster is too far away from the mouse position
     * If IsTargettedAbility, Checks if there is a Character with an AttributsManager under the mouse
     * if there is one cache it for other applications later
     */
	UFUNCTION()
	bool CanCast();
	
protected:
    /**
     * Called when the cooldown timer reaches 0
     * Sets the cooldown as finished
     */
	UFUNCTION()
	void OnCooldownEnd();

    /**
     * Duplicate the current ability and bind it to NewOwner
     * @param NewOwner The owner of the duplicated ability, Most of the time is a projectile
     */
	UFUNCTION()
	virtual UASAbility* CreateAbilityInstance(AActor* NewOwner);

    /**
     * Gets mouse position and then move the owning character towards it
     */
	UFUNCTION()
	void RotateTowardsMouse();

    /**
     * Checks if there is a target under the mouse
     * @param OutTarget Cache in the target if there is one
     */
	UFUNCTION()
	bool IsTargetUnderMouse(UASAttributsManager*& OutTarget);

    /**
     * Check if the ability target position is within the max range of the ability
     * @param TargetPosition the position that needs to be within range (for player the mouse position)
     */
	UFUNCTION()
	bool IsTargetCloseEnough(FVector TargetPosition);
	
    /**
     * Retrieves all the AttributsManagers that are within a radius around a position
     * @param Radius the range around Center where the AttributsManagers will be retrived
     * @param Center the center of the radius of detection
     */
	UFUNCTION()
	TArray<UASAttributsManager*> GetNearbyAttributsManagers(float Radius, FVector Center);

    /**
     * Callback of the FOnAbilityTrigger delegates that creates related effects and applies them on a target AttributsManager
     * @param TargetAttributManager The AttributsManager on which the effects will be applied
     * @param ActivationType The type of effects that will be created and applied by this callback
     */
	UFUNCTION()
	virtual void ApplyEffects(UASAttributsManager* TargetAttributManager, EASActivationType ActivationType);

    /**
     * Retrives the current position of the mouse of the player controller of the Owning Character
     * This shouldn't be here and this should be retrieved from the caster as a position (that could be the mouse or another input from an AI)
     */
	UFUNCTION()
	FVector GetMousePosition();
	
public:
	//************ GETTERS ***************************************

    /**
     * Sets the pointer of the CasterAttributsManager
     * @param InCasterAttributs The pointer to the Caster AttributsManager that will be cached
     */
	UFUNCTION()
	void SetCasterAttributs(UASAttributsManager* InCasterAttributs)
	{
		CasterAttributsManager = InCasterAttributs;
	}

    /**
     * Retrieves the current value of the timer
     */
	UFUNCTION()
	float GetCurrentTimer()
	{
		return CurrentTimer;
	}
	
    /**
     * Sets the target AttributsManager of this ability
     * @param InOnCastTargetAttributsManager The pointer to the Target of this ability
     */
	UFUNCTION()
	void SetOnCastTargetAttributsManager(UASAttributsManager* InOnCastTargetAttributsManager)
	{
		OnCastTargetAttributesManager = InOnCastTargetAttributsManager;
	}
};
