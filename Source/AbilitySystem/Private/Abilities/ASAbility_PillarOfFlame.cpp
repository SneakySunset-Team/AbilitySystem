#include "Abilities/ASAbility_PillarOfFlame.h"

#include "CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Effects/ASEffect.h"

enum class ETeam : uint8;

void UASAbility_PillarOfFlame::InitializePersistant(AASCharacter* InOwner)
{
	Super::InitializePersistant(InOwner);
	OnFlamePillarHitTarget.AddDynamic(this, &UASAbility_PillarOfFlame::ApplyEffects);
}

void UASAbility_PillarOfFlame::StartCasting()
{
	Super::StartCasting();
	MouseTarget = GetMousePosition();
}

void UASAbility_PillarOfFlame::OnTriggerAnimationEventCallback(FName NotifyName,
                                                               const FBranchingPointNotifyPayload& Payload)
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

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UASAbility_PillarOfFlame::ApplyFlamePillar, FlamePillarDelayBeforeDamage, false, -1);
			VisualZone = GetWorld()->SpawnActor<AActor>(VisualZonePrefab, MouseTarget, FRotator::ZeroRotator);
			VisualZone->SetActorScale3D(FVector(FlamePillarRadius / 150.0f, FlamePillarRadius / 150.0f, 1));
		}
	}
}

void UASAbility_PillarOfFlame::ApplyFlamePillar()
{
	TArray<UASAttributsManager*> AttributsManagerInRadius = GetNearbyAttributsManagers(FlamePillarRadius, MouseTarget);

	VisualZone->Destroy();
	ETeam CasterTeam = CasterAttributsManager->GetTeam();
	for (auto AttributManager : AttributsManagerInRadius)
	{
		if (CasterTeam != AttributManager->GetTeam())
		{
			OnFlamePillarHitTarget.Broadcast(AttributManager, EASActivationType::OnHitTarget);
		}
	}
}
