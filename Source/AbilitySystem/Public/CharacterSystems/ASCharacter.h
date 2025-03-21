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
	AASCharacter();

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
	
	virtual void BeginPlay() override;

	virtual void OnTriggerAbilityOne();
	virtual void OnTriggerAbilityTwo();
	virtual void OnTriggerAbilityThree();
	virtual void OnTriggerUltimate();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	UASAttributsManager* GetAttributsManager()
	{
		return AttributsManagerComponent;
	}
};
