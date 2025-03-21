// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystems/ASAbilitySystem.h"

#include "NiagaraTypes.h"
#include "Abilities/ASAbility.h"
#include "CharacterSystems/ASCharacter.h"

void UASAbilitySystem::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AASCharacter>(GetOwner());

	AbilityOne = NewObject<UASAbility>(AbilityOnePrefab);
	AbilityOne->Initialize(OwningCharacter, true);
	
	AbilityTwo = NewObject<UASAbility>(AbilityTwoPrefab);
	AbilityTwo->Initialize(OwningCharacter, true);
	
	AbilityThree = NewObject<UASAbility>(AbilityThreePrefab);
	AbilityThree->Initialize(OwningCharacter, true);
	
	Ultimate = NewObject<UASAbility>(UltimatePrefab);
	Ultimate->Initialize(OwningCharacter, true);
}

void UASAbilitySystem::CastAbility(int AbilityIndex)
{
	UASAbility* TargetAbility = nullptr;
	switch (AbilityIndex)
	{
		case 1 :
			TargetAbility = AbilityOne;
			break;
		case 2 :
			TargetAbility = AbilityTwo;
			break;
		case 3 :
			TargetAbility = AbilityThree;
			break;
		case 4 :
			TargetAbility = Ultimate;
			break;
		default:
			checkNoEntry();
	}

	if (TargetAbility->CanCast())
	{
		TargetAbility->StartCasting();
	}
}
