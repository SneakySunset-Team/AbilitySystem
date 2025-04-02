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

	UPROPERTY(EditAnywhere, Category = "AS|Stats", meta = (EditConditionHides = "!IsTargettedAbility"))
	float MaxCastDistance;
	
	UPROPERTY()
	UASAttributsManager* OnCastTargetAttributesManager;
	
	UPROPERTY()
	bool IsInCooldown;
	
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

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	bool IsSelfStunnedDuringCast;
	
	UPROPERTY(EditAnywhere, Category = "AS|Stats", meta = (EditConditionHides = "IsSelfStunnedDuringCast"))
	TSubclassOf<UASLingeringEffect> SelfStunPrefab;
	
	UPROPERTY()
	TObjectPtr<UASLingeringEffect> SelfStunEffect;

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
	bool IsTargetCloseEnough(FVector TargetPosition);
	
	UFUNCTION()
	TArray<UASAttributsManager*> GetNearbyAttributsManagers(float Radius, FVector Center);

	UFUNCTION()
	virtual void ApplyEffects(UASAttributsManager* TargetAttributManager, EASActivationType ActivationType);

	UFUNCTION()
	FVector GetMousePosition();
	
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
