#include "AbilitySystem/Public/CharacterSystems/ASCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterSystems/ASAbilitySystem.h"
#include "CharacterSystems/ASAttributsManager.h"
#include "UObject/UnrealTypePrivate.h"

AASCharacter::AASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	AttributsManagerComponent = CreateDefaultSubobject<UASAttributsManager>("Attributs Manager");
	AbilitySystemComponent = CreateDefaultSubobject<UASAbilitySystem>("Ability System Component");
}

void AASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
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

