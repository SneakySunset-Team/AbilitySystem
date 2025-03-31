#pragma once

#include "CoreMinimal.h"
#include "ASEffect.h"
#include "ASLingeringEffect.generated.h"

enum class EStatus : uint8;

UENUM(Blueprintable)
enum class EASTickRate : uint8
{
	Quarter_s,
	Half_s,
	Full_s
};

UCLASS()
class ABILITYSYSTEM_API UASLingeringEffect : public UASEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float Duration;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	EASTickRate TickRate;

	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	EStatus Status;

	UPROPERTY()
	TObjectPtr<UASAttributsManager> TargetAttributManager;

public:
	FTimerHandle Timer;
	
	virtual void ApplyEffect(UASAttributsManager* InTargetAttributsManager) override;

	UFUNCTION()
	void ApplyDot();

	UFUNCTION()
	void OnTimerEnded();

public:
	UFUNCTION()
	EStatus GetStatus() {return Status;}
	
};
