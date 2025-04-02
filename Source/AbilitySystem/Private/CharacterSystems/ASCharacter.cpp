#include "AbilitySystem/Public/CharacterSystems/ASCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterSystems/ASAbilitySystem.h"
#include "CharacterSystems/ASAttributsManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/UnrealTypePrivate.h"



AASCharacter::AASCharacter(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	AttributsManagerComponent = CreateDefaultSubobject<UASAttributsManager>("Attributs Manager");
	AbilitySystemComponent = CreateDefaultSubobject<UASAbilitySystem>("Ability System Component");
}

void AASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	AttributsManagerComponent->OnStatusAdded.AddDynamic(this, &AASCharacter::OnAddStatus);
	AttributsManagerComponent->OnStatusRemoved.AddDynamic(this, &AASCharacter::OnRemoveStatus);
	
	if (!IsValid(PC)) return;
	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->AddMappingContext(IMC_Default, 0);
	}

}

void AASCharacter::OnTriggerAbilityOne()
{
	AbilitySystemComponent->CastAbility(1);
}

void AASCharacter::OnTriggerAbilityTwo()
{
	AbilitySystemComponent->CastAbility(2);
}

void AASCharacter::OnTriggerAbilityThree()
{
	AbilitySystemComponent->CastAbility(3);
}

void AASCharacter::OnTriggerUltimate()
{
	AbilitySystemComponent->CastAbility(4);
}

void AASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_AbilityOne, ETriggerEvent::Triggered, this, &AASCharacter::OnTriggerAbilityOne);
		EnhancedInputComponent->BindAction(IA_AbilityTwo, ETriggerEvent::Triggered, this, &AASCharacter::OnTriggerAbilityTwo);
		EnhancedInputComponent->BindAction(IA_AbilityThree, ETriggerEvent::Triggered, this, &AASCharacter::OnTriggerAbilityThree);
		EnhancedInputComponent->BindAction(IA_Ultimate, ETriggerEvent::Triggered, this, &AASCharacter::OnTriggerUltimate);
	}
}

float AASCharacter::GetHealthPercent()
{
	return AttributsManagerComponent->GetHealthPercent();
}

float AASCharacter::GetManaPercent()
{
	return AttributsManagerComponent->GetManaPercent();
}

int AASCharacter::GetAbilityOneCurrentCooldown()
{
	return FMath::CeilToInt(AbilitySystemComponent->GetAbilityOneCooldown());
}

int AASCharacter::GetAbilityTwoCurrentCooldown()
{
	return FMath::CeilToInt(AbilitySystemComponent->GetAbilityTwoCooldown());
}

int AASCharacter::GetAbilityThreeCurrentCooldown()
{
	return FMath::CeilToInt(AbilitySystemComponent->GetAbilityThreeCooldown());
}

int AASCharacter::GetUltimateCurrentCooldown()
{
	return FMath::CeilToInt(AbilitySystemComponent->GetUltimateCooldown());
}

void AASCharacter::OnAddStatus(EStatus NewStatus)
{
	BIE_OnAddStatus(NewStatus);
	switch (NewStatus)
	{
	case EStatus::Stunned:
		GetCharacterMovement()->Deactivate();
		break;
	case EStatus::Burning:
		break;
	default: ;
	}
}

void AASCharacter::OnRemoveStatus(EStatus OldStatus)
{
	BIE_OnRemoveStatus(OldStatus);

	switch (OldStatus)
	{
	case EStatus::Stunned:
		GetCharacterMovement()->Activate();
		break;
	case EStatus::Burning:
		break;
	default: ;
	}
}



