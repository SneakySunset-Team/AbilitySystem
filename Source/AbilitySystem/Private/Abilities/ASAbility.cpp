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

	OnCastAnimationStart.AddDynamic(this, &UASAbility::ApplyEffects);
	OnCastAnimationTrigger.AddDynamic(this, &UASAbility::ApplyEffects);
	OnCastAnimationEnd.AddDynamic(this, &UASAbility::ApplyEffects);

	if (IsSelfStunnedDuringCast && IsValid(SelfStunPrefab))
	{
		SelfStunEffect = NewObject<UASLingeringEffect>(this, SelfStunPrefab);
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
    CasterAttributsManager->EditStat(EStat::Mana, -ManaCost);
    
	if (IsValid(OnStartCasting_Sound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, OnStartCasting_Sound, OwningCharacter->GetActorLocation());	
	}

	UAnimInstance* OwningCharacter_AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	OwningCharacter_AnimInstance->OnMontageStarted.AddDynamic(this, &UASAbility::OnAnimationStartCallback);
	OwningCharacter_AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UASAbility::OnTriggerAnimationEventCallback);
	OwningCharacter_AnimInstance->OnMontageEnded.AddDynamic(this, &UASAbility::OnAnimationEndCallback);
	OwningCharacter_AnimInstance->Montage_Play(OnStartCasting_AnimMontage);

	RotateTowardsTargetPosition();


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
		OnCastAnimationStart.Broadcast(OwningCharacter->GetAttributsManager(), EASActivationType::OnStartCasting);

		if (IsSelfStunnedDuringCast && IsValid(SelfStunPrefab))
		{
			CasterAttributsManager->AddLingeringEffect(SelfStunEffect);
		}
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
			OnCastAnimationTrigger.Broadcast(OwningCharacter->GetAttributsManager(), EASActivationType::OnAnimationTriggerEvent);
		}
	}
}

void UASAbility::OnAnimationEndCallback(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == OnStartCasting_AnimMontage)
	{
		if (IsSelfStunnedDuringCast && IsValid(SelfStunPrefab))
		{
			CasterAttributsManager->RemoveLingeringEffect(SelfStunEffect);
		}

		if (!bInterrupted)
		{
			OnCastAnimationEnd.Broadcast(OwningCharacter->GetAttributsManager(), EASActivationType::OnEndAnimation);
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
		FVector WorldLocation, WorldDirection;
		FHitResult HitResult;
		if (!OwningCharacter->GetTargetPosition(WorldLocation, WorldDirection, HitResult))
		{
			return false;
		}
		
		if (IsTargettedAbility &&
			!IsTargetPositionPointingToAttributsManager(OnCastTargetAttributesManager))
		{
			return false;
		}
		
		
		if (IsTargettedAbility && !IsTargetCloseEnough(WorldLocation)){
			FVector Origin = OwningCharacter->GetActorLocation();
			Origin.Z = 0;
			DrawCircle(
				GetWorld(),
				Origin,
				FVector::RightVector,
				FVector::ForwardVector,
				FColor::Red,
				MaxCastDistance,
				64,
				false,
				2,
				3);
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

void UASAbility::RotateTowardsTargetPosition()
{
	FVector WorldLocation, WorldDirection;
	FHitResult HitResult;
	if (!OwningCharacter->GetTargetPosition(WorldLocation, WorldDirection, HitResult))
	{
	}
	// Calculate the direction to face (ignoring Z component for a top-down game)
	FVector CharacterLocation = OwningCharacter->GetActorLocation();
	FVector DirectionToTarget = WorldLocation - CharacterLocation;
	DirectionToTarget.Z = 0; // Ignore height difference
	DirectionToTarget = DirectionToTarget.GetSafeNormal();
        
	// Create rotation from direction
	FRotator NewRotation = DirectionToTarget.Rotation();
    
	// Apply rotation instantly
	OwningCharacter->SetActorRotation(NewRotation);
}

bool UASAbility::IsTargetPositionPointingToAttributsManager(UASAttributsManager*& OutTarget)
{
	FVector WorldLocation, WorldDirection;
	FHitResult HitResult;
	if (!OwningCharacter->GetTargetPosition(WorldLocation, WorldDirection, HitResult, ECC_PhysicsBody))
	{
		return false;
	}
	UASAttributsManager* TargetAttributManager = HitResult.GetActor()->GetComponentByClass<UASAttributsManager>();
	if (TargetAttributManager)
	{
		OutTarget = TargetAttributManager;
		return true;
	}
	return false;
}

bool UASAbility::IsTargetCloseEnough(FVector TargetPosition)
{
	float Distance = FVector::Distance(OwningCharacter->GetActorLocation(), TargetPosition);
	return  Distance < MaxCastDistance;
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

void UASAbility::ApplyEffects(UASAttributsManager* TargetAttributManager, EASActivationType ActivationType)
{
	for (auto EffectPrefab : EffectsPrefabs)
	{
		if (EffectPrefab->GetDefaultObject<UASEffect>()->GetActivationType() == ActivationType)
		{
			UASEffect* Effect = NewObject<UASEffect>(this, EffectPrefab);
			Effect->Initialize(CasterAttributsManager);
			Effect->ApplyEffect(TargetAttributManager);
		}
	}
}


	
