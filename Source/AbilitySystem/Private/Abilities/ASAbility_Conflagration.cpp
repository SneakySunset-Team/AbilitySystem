#include "Abilities/ASAbility_Conflagration.h"

#include "Abilities/ASAbilityProjectile.h"
#include "CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Projectiles/ASProjectile_Targetted.h"

void UASAbility_Conflagration::OnTriggerAnimationEventCallback(FName NotifyName,
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
			if (OnCastTargetAttributesManager->GetHasStatus(EStatus::Burning))
			{
				TArray<UASAttributsManager*> NearbyAttributsManager =
					GetNearbyAttributsManagers(DetectionRadius, OnCastTargetAttributesManager->GetOwner()->GetActorLocation());
				if (NearbyAttributsManager.Num() != 0)
				{
					ETeam Team = CasterAttributsManager->GetTeam();
					for (const auto& NearbyAttribut : NearbyAttributsManager)
					{
						if (Team != NearbyAttribut->GetTeam())
						{
							FVector SpawnLocation = OnCastTargetAttributesManager->GetOwner()->GetActorLocation();
							FRotator SpawnRotation = FRotator::ZeroRotator;
							FActorSpawnParameters SpawnParameters;
							SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
							AASProjectile_Targetted* NewProjectile = GetWorld()->SpawnActor<AASProjectile_Targetted>(ProjectileSpreadFire, SpawnLocation, SpawnRotation, SpawnParameters);
							UASAbilityProjectile* VolatileAbility = Cast<UASAbilityProjectile>(CreateAbilityInstance(NewProjectile));
							NewProjectile->InitializeTargettedProjectile(NearbyAttribut, VolatileAbility->GetProjectileSpeed());
							VolatileAbility->SetOnCastTargetAttributsManager(NearbyAttribut);
							VolatileAbility->InitializeDuplicate(OwningCharacter);
							NewProjectile->OnHitDelegate.BindDynamic(VolatileAbility, &UASAbilityProjectile::OnHitTargetCallback);
						}
					}
					
				}
			}
			OnCastAnimationTrigger.Broadcast(OnCastTargetAttributesManager, EASActivationType::OnAnimationTriggerEvent);
		}
	}
}

UASAbility* UASAbility_Conflagration::CreateAbilityInstance(AActor* NewOwner)
{
	UASAbility* AbilityInstance = NewObject<UASAbility>(NewOwner, AbilitySpreadFire);
	AbilityInstance->InitializeDuplicate(OwningCharacter);
	AbilityInstance->SetOnCastTargetAttributsManager(OnCastTargetAttributesManager);
	return AbilityInstance;
}
