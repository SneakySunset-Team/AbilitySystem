// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilitySystemCharacter.h"
#include "ASCharacter.generated.h"

class UASAbilitySystem;
class UASAttributsManager;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ABILITYSYSTEM_API AASCharacter : public AAbilitySystemCharacter
{
	GENERATED_BODY()

public:
	AASCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere, Category="", meta=(AllowPrivateAccess))
	TObjectPtr<UASAttributsManager> AttributsManagerComponent;

	UPROPERTY(EditAnywhere, Category="", meta=(AllowPrivateAccess))
	TObjectPtr<UASAbilitySystem> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> IMC_Default;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> IA_AbilityOne;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> IA_AbilityTwo;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> IA_AbilityThree;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> IA_Ultimate;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> IA_AutoAttack;
	
	virtual void BeginPlay() override;
	void OnTriggerAutoAttack();

	virtual void OnTriggerAbilityOne();
	virtual void OnTriggerAbilityTwo();
	virtual void OnTriggerAbilityThree();
	virtual void OnTriggerUltimate();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	UFUNCTION(BlueprintCallable)
	float GetManaPercent();

	UFUNCTION(BlueprintCallable)
	int GetAbilityOneCurrentCooldown();

	UFUNCTION(BlueprintCallable)
	int GetAbilityTwoCurrentCooldown();

	UFUNCTION(BlueprintCallable)
	int GetAbilityThreeCurrentCooldown();

	UFUNCTION(BlueprintCallable)
	int GetUltimateCurrentCooldown();

	UFUNCTION()
	void OnAddStatus(EStatus NewStatus);

	UFUNCTION()
	void OnRemoveStatus(EStatus OldStatus);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BIE_OnAddStatus(EStatus NewStatus);

	UFUNCTION(BlueprintImplementableEvent)
	void BIE_OnRemoveStatus(EStatus OldStatus);
	
	UFUNCTION()
	UASAttributsManager* GetAttributsManager()
	{
		return AttributsManagerComponent;
	}
};
