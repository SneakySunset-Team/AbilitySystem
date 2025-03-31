#include "CharacterSystems/ASAbilitySystem.h"

#include "NiagaraTypes.h"
#include "Abilities/ASAbility.h"
#include "CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"

void UASAbilitySystem::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AASCharacter>(GetOwner());

	if (IsValid(AbilityOnePrefab))
	{
		AbilityOne = NewObject<UASAbility>(this, AbilityOnePrefab);
		AbilityOne->InitializePersistant(OwningCharacter);
		AbilityOne->SetCasterAttributs(OwningCharacter->GetAttributsManager()->GetAttributs());
	}

	if (IsValid(AbilityTwoPrefab))
	{
		AbilityTwo = NewObject<UASAbility>(this, AbilityTwoPrefab);
		AbilityTwo->InitializePersistant(OwningCharacter);
		AbilityTwo->SetCasterAttributs(OwningCharacter->GetAttributsManager()->GetAttributs());
	}

	if (IsValid(AbilityThreePrefab))
	{
		AbilityThree = NewObject<UASAbility>(this, AbilityThreePrefab);
		AbilityThree->InitializePersistant(OwningCharacter);
		AbilityThree->SetCasterAttributs(OwningCharacter->GetAttributsManager()->GetAttributs());
	}

	if (IsValid(UltimatePrefab))
	{
		Ultimate = NewObject<UASAbility>(this, UltimatePrefab);
		Ultimate->InitializePersistant(OwningCharacter);
		Ultimate->SetCasterAttributs(OwningCharacter->GetAttributsManager()->GetAttributs());
	}
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

	if (IsValid(TargetAbility) && TargetAbility->CanCast())
	{
		TargetAbility->StartCasting();
	}
}
