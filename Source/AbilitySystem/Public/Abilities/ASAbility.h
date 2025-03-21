#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASAbility.generated.h"


class AASCharacter;
class UASEffect;
class UASAttributsManager;
class UASAttributs;

UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEM_API UASAbility : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	int ManaCost;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float Cooldown;
	
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
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

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	TSubclassOf<UASEffect> ManaCostEffectPrefab;
	
	UPROPERTY()	
	TObjectPtr<UASEffect> ManaCostEffect;
	
	UPROPERTY()
	TObjectPtr<UASAttributs> CasterAttributs;

	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<UAnimMontage> OnStartCasting_AnimMontage;

	UPROPERTY(EditAnywhere, Category = "AS|Cosmetics")
	TObjectPtr<USoundBase> OnStartCasting_Sound;

	UPROPERTY()
	TObjectPtr<AASCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UAnimInstance> OwningCharacter_AnimInstance;

	
	FTimerHandle CooldownTimerHandle;

public:
	UFUNCTION()
	virtual void Initialize(AASCharacter* Owner, bool IsPersistantAbility);
	
	UFUNCTION()
	void StartCasting();

	UFUNCTION()
	virtual void EndCasting(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	
	UFUNCTION()
	bool CanCast();


	UFUNCTION()
	void TriggerAbilityEffects(UASAttributsManager* TargetAttributsManager);
	
protected:
	UFUNCTION()
	void OnCooldownEnd();

	UFUNCTION()
	UASAbility* CreateAbilityInstance(AActor* NewOwner);
	
	//************ GETTERS ***************************************
	
	UFUNCTION()
	void SetCasterAttributs(UASAttributs* InCasterAttributs)
	{
		CasterAttributs = InCasterAttributs;
	}
};
