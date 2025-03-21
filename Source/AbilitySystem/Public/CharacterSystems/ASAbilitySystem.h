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

	UPROPERTY()
	TObjectPtr<AASCharacter> OwningCharacter;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> AbilityOnePrefab;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> AbilityTwoPrefab;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> AbilityThreePrefab;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<UASAbility> UltimatePrefab;

	UPROPERTY()
	TObjectPtr<UASAbility> AbilityOne;

	UPROPERTY()
	TObjectPtr<UASAbility> AbilityTwo;

	UPROPERTY()
	TObjectPtr<UASAbility> AbilityThree;

	UPROPERTY()
	TObjectPtr<UASAbility> Ultimate;

	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void CastAbility(int AbilityIndex);
};
