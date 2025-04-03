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

    // After the effect is applied, how much time before it is removed
    // if -1 the effect should be infinite
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	float Duration;

    // Frequency of application of the effect's dot
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	EASTickRate TickRate;

    // The Status to add to the Target AttributsManager while this effect is active
	UPROPERTY(EditAnywhere, Category = "AS|Stats")
	EStatus Status;

    // Cached in Target AttributsManager
	UPROPERTY()
	TObjectPtr<UASAttributsManager> TargetAttributManager;

public:
    // The Handle of this effect Timer
	FTimerHandle Timer;
	
    /**
     * Add this effect to the Target AttributsManager ActiveEffects
     * Start Timer to remove this effect from the Target AttributsManager ActiveEffects
     * @param InTargetAttributsManager the target AttributsManager onto which this effect will be applied
     */
	virtual void ApplyEffect(UASAttributsManager* InTargetAttributsManager) override;

    /**
     * Edit the TargetAttributManager with this effects Stats
     * this is a callback from the timer set at tick rate frequency called from the GameMode
     */
	UFUNCTION()
	void ApplyDot();

    /**
     * Callback when the effect is removed from the target AttributsManager
     */
	UFUNCTION()
	void OnTimerEnded();

public:
    /**
     * Returns the Status this effect applies to its target AttributsManager
     */
	UFUNCTION()
	EStatus GetStatus() {return Status;}
	
};
