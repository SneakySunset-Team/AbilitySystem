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
	
    // The Stats of the body Changed by effects and used by the Owning Character
	UPROPERTY(EditAnywhere)
	TMap<EStat, float> Stats;
	
    // The currently active effects on this Attributs
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UASLingeringEffect>> ActiveEffects;

	friend class UASAttributsManager;

public:
    /**
     * Weither the Stats contains the provided type of Stat
     * @param Stat The stat that needs to be present in Stats to return true
     */
	bool StatsContains(EStat Stat)
	{
		return Stats.Contains(Stat);
	}

    /**
     * Returns the Value associated with the provided stat
     * If the Stat is not present return 0
     * @param Stat the Key Stat that gives the value that is requested
     */
	float GetStat(EStat Stat)
	{
		if (Stats.Contains(Stat))
		{
			return Stats[Stat];
		}
		return 0.f;
	}

};
