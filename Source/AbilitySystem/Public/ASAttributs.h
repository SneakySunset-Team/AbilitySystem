#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ASAttributs.generated.h"


UENUM(BlueprintType)
enum class EStat : uint8
{
	MaxHealth,
	Health,
	Damage,
	MaxMana,
	Mana
};

UENUM(BlueprintType)
enum class EStatus : uint8
{
	Stunned,
	Burning,
	None
};



class UASLingeringEffect;
class UASEffect;

USTRUCT(BlueprintType)
struct ABILITYSYSTEM_API FASAttributs
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TMap<EStat, float> Stats;
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UASLingeringEffect>> ActiveEffects;

	friend class UASAttributsManager;

public:
	bool StatsContains(EStat Stat)
	{
		return Stats.Contains(Stat);
	}

	float GetStat(EStat Stat)
	{
		if (Stats.Contains(Stat))
		{
			return Stats[Stat];
		}
		return 0.f;
	}

};
