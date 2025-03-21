#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASAttributs.generated.h"

UENUM(BlueprintType)
enum class EStat : uint8
{
	Health,
	Damage,
	Speed,
	Mana
};

UENUM(BlueprintType)
enum class EStatus : uint8
{
	Stunned,
	Burning
};

class UASEffect;

UCLASS()
class ABILITYSYSTEM_API UASAttributs : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TMap<EStat, float> Stats;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UASEffect>> ActiveEffects;

	UPROPERTY(EditAnywhere)
	TSet<EStatus> ActiveStatuses;

	friend class UASAttributsManager;

public:
	bool StatsContains(EStat Stat)
	{
		return Stats.Contains(Stat);
	}

	float GetStat(EStat Stat)
	{
		return Stats[Stat];
	}
};
