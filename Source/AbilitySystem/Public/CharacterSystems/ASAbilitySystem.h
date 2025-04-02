#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASAbilitySystem.generated.h"


class AASCharacter;
class UASAbility;

UCLASS()
class ABILITYSYSTEM_API UASAbilitySystem : public UActorComponent
{
	GENERATED_BODY()

	UASAbilitySystem();
	
	UPROPERTY()
	TObjectPtr<AASCharacter> OwningCharacter;


	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> AutoAttackPrefab;
	
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> AbilityOnePrefab;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> AbilityTwoPrefab;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> AbilityThreePrefab;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> UltimatePrefab;

	UPROPERTY()
	TObjectPtr<UASAbility> AutoAttack;
	
	UPROPERTY()
	TObjectPtr<UASAbility> AbilityOne;

	UPROPERTY()
	TObjectPtr<UASAbility> AbilityTwo;

	UPROPERTY()
	TObjectPtr<UASAbility> AbilityThree;

	UPROPERTY()
	TObjectPtr<UASAbility> Ultimate;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UFUNCTION()
	void CastAbility(int AbilityIndex);

	UFUNCTION()
	float GetAbilityOneCooldown();

	UFUNCTION()
	float GetAbilityTwoCooldown();

	UFUNCTION()
	float GetAbilityThreeCooldown();

	UFUNCTION()
	float GetUltimateCooldown();
};
