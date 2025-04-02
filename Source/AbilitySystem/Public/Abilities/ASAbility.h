#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASAbility.generated.h"


class AASCharacter;
class UASEffect;
class UASAttributsManager;
struct FASAttributs;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTrigger, UASAttributsManager*, AttributsManager);

UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEM_API UASAbility : public UObject
{
	GENERATED_BODY()

protected:
	FOnAbilityTrigger OnCastAnimationStart;
	FOnAbilityTrigger OnCastAnimationTrigger;
	FOnAbilityTrigger OnCastAnimationEnd;
	
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	int ManaCost;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float Cooldown;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	FName BoneName;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	bool IsTargettedAbility;

	UPROPERTY()
	UASAttributsManager* OnCastTargetAttributesManager;
	
	UPROPERTY()
	bool IsInCooldown;

	// List of Effects that will be applied by the ability on Hit.
	// If I was implementing a level up system I would probably introduce a
	// TMap<int, TArray<TSubclassOf<UASEffect>> with each key beeing the level
	// at which the effect will be in use.
	// (this would also allow me to introduce additional effects on level up)
	// Going further would be implementing the TMap in the Ability System for the abilities.
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	TArray<TSubclassOf<UASEffect>> EffectsPrefabs;

	UPROPERTY()
	TArray<TObjectPtr<UASEffect>> Effects;
	
	UPROPERTY()
	TObjectPtr<UASAttributsManager> CasterAttributsManager;

	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<UAnimMontage> OnStartCasting_AnimMontage;

	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<USoundBase> OnStartCasting_Sound;

	UPROPERTY()
	TObjectPtr<AASCharacter> OwningCharacter;

	UPROPERTY()
	float CurrentTimer;
	
	FTimerHandle CooldownTimerHandle;

public:
	UFUNCTION()
	virtual void InitializePersistant(AASCharacter* InOwner);

	UFUNCTION()
	virtual void InitializeDuplicate(AASCharacter* InOwner);
	
	UFUNCTION()
	virtual void StartCasting();

	UFUNCTION()
	virtual void Tick(float DelaTime);
	
	UFUNCTION()
	virtual void OnAnimationStartCallback(UAnimMontage* Montage);

	// When Animation reachs Notify with name "Cast" Trigger ability
	// I identify the notify by name but it would probably be better to create a custom Notify class.
	UFUNCTION()
	virtual void OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

	UFUNCTION()
	virtual void OnAnimationEndCallback(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	bool CanCast();
	
protected:
	UFUNCTION()
	void OnCooldownEnd();

	UFUNCTION()
	virtual UASAbility* CreateAbilityInstance(AActor* NewOwner);

	UFUNCTION()
	void RotateTowardsMouse();

	UFUNCTION()
	bool IsTargetUnderMouse(UASAttributsManager*& OutTarget);

	UFUNCTION()
	TArray<UASAttributsManager*> GetNearbyAttributsManagers(float Radius, FVector Center);

public:
	//************ GETTERS ***************************************

	UFUNCTION()
	void SetCasterAttributs(UASAttributsManager* InCasterAttributs)
	{
		CasterAttributsManager = InCasterAttributs;
	}

	UFUNCTION()
	float GetCurrentTimer()
	{
		return CurrentTimer;
	}
	
	UFUNCTION()
	void SetOnCastTargetAttributsManager(UASAttributsManager* InOnCastTargetAttributsManager)
	{
		OnCastTargetAttributesManager = InOnCastTargetAttributsManager;
	}
};
