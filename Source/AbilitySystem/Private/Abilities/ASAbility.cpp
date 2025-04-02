#include "Abilities/ASAbility.h"
#include "ASAttributs.h"
#include "SQCapture.h"
#include "AbilitySystem/Public/CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Effects/ASEffect.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UASAbility::InitializePersistant(AASCharacter* InOwner)
{
	OwningCharacter = InOwner;

	for (const auto EffectPrefab : EffectsPrefabs)
	{
		bool IsCastFromPersistant = true;
		
		UASEffect* Effect = NewObject<UASEffect>(this, EffectPrefab);
		Effect->Initialize(OwningCharacter->GetAttributsManager());
		switch (Effect->GetActivationType())
		{
		case EASActivationType::OnStartCasting:
			OnCastAnimationStart.AddDynamic(Effect, &UASEffect::ApplyEffect);
			break;
		case EASActivationType::OnAnimationTriggerEvent:
			OnCastAnimationTrigger.AddDynamic(Effect, &UASEffect::ApplyEffect);
			break;
		case EASActivationType::OnEndAnimation:
			OnCastAnimationEnd.AddDynamic(Effect, &UASEffect::ApplyEffect);
			break;
		default:
			IsCastFromPersistant = false;
			break;
		}

		if (IsCastFromPersistant)
		{
			Effects.Add(Effect);
		}
	}


}

void UASAbility::InitializeDuplicate(AASCharacter* InOwner)
{
	OwningCharacter = InOwner;
	CasterAttributsManager = OwningCharacter->GetAttributsManager();
}

void UASAbility::StartCasting()
{
	CurrentTimer = Cooldown;
	IsInCooldown = true;

	if (IsValid(OnStartCasting_Sound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, OnStartCasting_Sound, OwningCharacter->GetActorLocation());	
	}

	UAnimInstance* OwningCharacter_AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	OwningCharacter_AnimInstance->OnMontageStarted.AddDynamic(this, &UASAbility::OnAnimationStartCallback);
	OwningCharacter_AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UASAbility::OnTriggerAnimationEventCallback);
	OwningCharacter_AnimInstance->OnMontageEnded.AddDynamic(this, &UASAbility::OnAnimationEndCallback);
	OwningCharacter_AnimInstance->Montage_Play(OnStartCasting_AnimMontage);

	RotateTowardsMouse();


}

void UASAbility::Tick(float DelaTime)
{
	if (IsInCooldown)
	{
		CurrentTimer -= DelaTime;
		if (CurrentTimer <= 0)
		{
			IsInCooldown = false;
			OnCooldownEnd();
		}
	}
}

void UASAbility::OnAnimationStartCallback(UAnimMontage* Montage)
{
	if (Montage == OnStartCasting_AnimMontage)
	{
		OnCastAnimationStart.Broadcast(OwningCharacter->GetAttributsManager());
		IsInCooldown = true;
		CasterAttributsManager->EditStat(EStat::Mana, -ManaCost);
		OwningCharacter->GetCharacterMovement()->Deactivate();
	}
}

void UASAbility::OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;

	// Get the active montage instance
	FAnimMontageInstance* ActiveMontageInstance = AnimInstance->GetActiveMontageInstance();
    
	// Check if it's our montage
	if (ActiveMontageInstance && ActiveMontageInstance->Montage == OnStartCasting_AnimMontage)
	{
		if (NotifyName == "Cast")
		{
			OnCastAnimationTrigger.Broadcast(OwningCharacter->GetAttributsManager());
		}
	}
}

void UASAbility::OnAnimationEndCallback(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == OnStartCasting_AnimMontage)
	{
		if (!bInterrupted && Montage == OnStartCasting_AnimMontage)
		{
			OnCastAnimationEnd.Broadcast(OwningCharacter->GetAttributsManager());
			if (!OwningCharacter->GetAttributsManager()->GetHasStatus(EStatus::Stunned))
			{
				OwningCharacter->GetCharacterMovement()->Activate();
			}
		}
		UAnimInstance* OwningCharacter_AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
		OwningCharacter_AnimInstance->OnMontageStarted.RemoveDynamic(this, &UASAbility::OnAnimationStartCallback);
		OwningCharacter_AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UASAbility::OnTriggerAnimationEventCallback);
		OwningCharacter_AnimInstance->OnMontageEnded.RemoveDynamic(this, &UASAbility::OnAnimationEndCallback);
	}
}

bool UASAbility::CanCast()
{
	int CasterMana = CasterAttributsManager->Attributs.GetStat(EStat::Mana);
	if (CasterMana >= ManaCost && !IsInCooldown)
	{
		if (IsTargettedAbility && !IsTargetUnderMouse(OnCastTargetAttributesManager))
		{
			return false;
		}
		return true;
	}

	return false;
}

void UASAbility::OnCooldownEnd()
{
	IsInCooldown = false;
}

UASAbility* UASAbility::CreateAbilityInstance(AActor* NewOwner)
{
	UASAbility* AbilityInstance = NewObject<UASAbility>(NewOwner, GetClass());
	AbilityInstance->InitializeDuplicate(OwningCharacter);
	AbilityInstance->SetOnCastTargetAttributsManager(OnCastTargetAttributesManager);
	return AbilityInstance;
}

void UASAbility::RotateTowardsMouse()
{
	APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController());
	if (!PlayerController) return;

	// Get mouse position in screen space
	float MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);
    
	// Convert mouse position to world space using a line trace
	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);
    
	// Setup collision params for line trace
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwningCharacter);
    
	// Calculate start and end of line trace
	FVector TraceStart = WorldLocation;
	FVector TraceEnd = WorldLocation + WorldDirection * 10000.0f; // Far distance
    
	// Perform line trace to find where mouse ray intersects the ground
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
	{
		// The hit location is where the mouse points in the world
		FVector MouseWorldPosition = HitResult.Location;
        
		// Calculate the direction to face (ignoring Z component for a top-down game)
		FVector CharacterLocation = OwningCharacter->GetActorLocation();
		FVector DirectionToMouse = MouseWorldPosition - CharacterLocation;
		DirectionToMouse.Z = 0; // Ignore height difference
		DirectionToMouse = DirectionToMouse.GetSafeNormal();
        
		// Create rotation from direction
		FRotator NewRotation = DirectionToMouse.Rotation();
        
		// Apply rotation instantly
		OwningCharacter->SetActorRotation(NewRotation);
	}
}

bool UASAbility::IsTargetUnderMouse(UASAttributsManager*& OutTarget)
{
	APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController());
	if (!PlayerController) return false;

	// Get mouse position in screen space
	float MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);
    
	// Convert mouse position to world space using a line trace
	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);
    
	// Setup collision params for line trace
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwningCharacter);
    
	// Calculate start and end of line trace
	FVector TraceStart = WorldLocation;
	FVector TraceEnd = WorldLocation + WorldDirection * 10000.0f; // Far distance
    
	// Perform line trace to find where mouse ray intersects the ground
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
	{
		UASAttributsManager* TargetAttributManager = HitResult.GetActor()->GetComponentByClass<UASAttributsManager>();
		if (TargetAttributManager)
		{
			OutTarget = TargetAttributManager;
			UE_LOG(LogTemp, Display, TEXT("Target Hit by Mouse"));
			return true;
		}
	}

	return false;
}

TArray<UASAttributsManager*> UASAbility::GetNearbyAttributsManagers(float Radius, FVector Center)
{
	TArray<AActor*> Actors;
	TArray<UASAttributsManager*> NearbyAttributsManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
	
	for (const AActor* FoundActor : Actors)
	{
		if (FVector::Distance(Center, FoundActor->GetActorLocation()) <= Radius)
		{
			UASAttributsManager* AttributsManager =  FoundActor->GetComponentByClass<UASAttributsManager>();
			if (AttributsManager)
			{
				NearbyAttributsManagers.AddUnique(AttributsManager);
			}
		}
	}
    
	return NearbyAttributsManagers;
}
