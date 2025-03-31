#pragma once

#include "CoreMinimal.h"
#include "ASAttributs.h"
#include "EditorCategoryUtils.h"
#include "Animation/AnimAttributes.h"
#include "Components/ActorComponent.h"
#include "Effects/ASLingeringEffect.h"
#include "ASAttributsManager.generated.h"

class UASLingeringEffect;

UENUM()
enum class ETeam : uint8
{
	Team1,
	Team2,
	Neutral
};


struct FASAttributs;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UASAttributsManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UASAttributsManager();

	UPROPERTY(EditAnywhere)
	FASAttributs Attributs;
	
protected:

	UPROPERTY(EditAnywhere)
	ETeam CurrentTeam;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void AddLingeringEffect(UASLingeringEffect* LingeringEffect);

	UFUNCTION()
	void RemoveLingeringEffect(UASLingeringEffect* LingeringEffect);

	UFUNCTION()
	void EditStat(EStat InStat, float InValue);

	UFUNCTION()
	TMap<EStat, float> GetAttributsStats()
	{
		return Attributs.Stats;
	}
	
	UFUNCTION()
	ETeam GetTeam()
	{
		return CurrentTeam;
	}

	UFUNCTION()
	bool GetHasStatus(EStatus InTargetStatus)
	{
		for (const auto& Effect : Attributs.ActiveEffects)
		{
			if (Effect->GetStatus() == InTargetStatus)
			{
				return true;
			}
		}
		return false;
	}

	UFUNCTION()
	float GetHealthPercent();

	UFUNCTION()
	float GetManaPercent();
};
