#include "CharacterSystems/ASAbilitySystem.h"

#include "NiagaraTypes.h"
#include "Abilities/ASAbility.h"
#include "CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"

UASAbilitySystem::UASAbilitySystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UASAbilitySystem::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AASCharacter>(GetOwner());

	if (IsValid(AbilityOnePrefab))
	{
		AbilityOne = NewObject<UASAbility>(this, AbilityOnePrefab);
		AbilityOne->InitializePersistant(OwningCharacter);
		AbilityOne->SetCasterAttributs(OwningCharacter->GetAttributsManager());
	}

	if (IsValid(AbilityTwoPrefab))
	{
		AbilityTwo = NewObject<UASAbility>(this, AbilityTwoPrefab);
		AbilityTwo->InitializePersistant(OwningCharacter);
		AbilityTwo->SetCasterAttributs(OwningCharacter->GetAttributsManager());
	}

	if (IsValid(AbilityThreePrefab))
	{
		AbilityThree = NewObject<UASAbility>(this, AbilityThreePrefab);
		AbilityThree->InitializePersistant(OwningCharacter);
		AbilityThree->SetCasterAttributs(OwningCharacter->GetAttributsManager());
	}

	if (IsValid(UltimatePrefab))
	{
		Ultimate = NewObject<UASAbility>(this, UltimatePrefab);
		Ultimate->InitializePersistant(OwningCharacter);
		Ultimate->SetCasterAttributs(OwningCharacter->GetAttributsManager());
	}
}

void UASAbilitySystem::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(AbilityOne))
	{
		AbilityOne->Tick(DeltaTime);
	}
	if (IsValid(AbilityTwo))
	{
		AbilityTwo->Tick(DeltaTime);
	}
	if (IsValid(AbilityThree))
	{
		AbilityThree->Tick(DeltaTime);
	}
	if (IsValid(Ultimate))
	{
		Ultimate->Tick(DeltaTime);
	}
}

void UASAbilitySystem::CastAbility(int AbilityIndex)
{
	if (OwningCharacter->GetAttributsManager()->GetHasStatus(EStatus::Stunned))
		return;
	
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

	if (IsValid(TargetAbility) && TargetAbility->CanCast())
	{
		TargetAbility->StartCasting();
	}
}

float UASAbilitySystem::GetAbilityOneCooldown()
{
	return IsValid(AbilityOne) ? AbilityOne->GetCurrentTimer() : 0;
}

float UASAbilitySystem::GetAbilityTwoCooldown()
{
	return IsValid(AbilityTwo) ? AbilityTwo->GetCurrentTimer() : 0;
}

float UASAbilitySystem::GetAbilityThreeCooldown()
{
	return IsValid(AbilityThree) ? AbilityThree->GetCurrentTimer() : 0;
}

float UASAbilitySystem::GetUltimateCooldown()
{
	return IsValid(Ultimate) ? Ultimate->GetCurrentTimer() : 0;
}
