// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilitySystemGameMode.h"
#include "ASGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerEvent);

UCLASS()
class ABILITYSYSTEM_API AASGameMode : public AAbilitySystemGameMode
{
	GENERATED_BODY()

public:
    // Delegates that are triggered on 250, 500 and 1000 ms intervals
	FOnTimerEvent OnTimerEvent_250ms;
	FOnTimerEvent OnTimerEvent_500ms;
	FOnTimerEvent OnTimerEvent_1000ms;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTimer_250ms();

	UFUNCTION()
	void OnTimer_500ms();

	UFUNCTION()
	void OnTimer_1000ms();
};
