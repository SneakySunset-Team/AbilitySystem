#pragma once

#include "CoreMinimal.h"
#include "ASAttributs.h"
#include "Components/ActorComponent.h"
#include "ASAttributsManager.generated.h"

UENUM()
enum class ETeam : uint8
{
	Team1,
	Team2,
	Neutral
};


class UASAttributs;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UASAttributsManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UASAttributsManager();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UASAttributs> Attributs;

	UPROPERTY(EditAnywhere)
	ETeam CurrentTeam;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void EditStat(EStat InStat, float InValue);

	UFUNCTION()
	UASAttributs* GetAttributs()
	{
		return Attributs;
	}

	UFUNCTION()
	TMap<EStat, float> GetAttributsStats()
	{
		return Attributs->Stats;
	}
	
	UFUNCTION()
	ETeam GetTeam()
	{
		return CurrentTeam;
	}
};
