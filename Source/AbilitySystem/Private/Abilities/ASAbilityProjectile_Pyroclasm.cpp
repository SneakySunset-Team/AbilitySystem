#include "Abilities/ASAbilityProjectile_Pyroclasm.h"

#include "CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/ASProjectile_Targetted.h"

void UASAbilityProjectile_Pyroclasm::OnHitTargetCallback(AActor* HitActor, FVector NormalImpulse,
                                                         const FHitResult& HitResult, AActor* Projectile)
{
	if (HitActor != OwningCharacter)
	{
		OnProjectileHit.Broadcast(OwningCharacter->GetAttributsManager(), EASActivationType::OnHit);
		OnProjectileHitAttributsManager.Broadcast(OnCastTargetAttributesManager, EASActivationType::OnHitTarget);
	}
	if (OnHitParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitParticle, HitResult.Location, NormalImpulse.Rotation(), true);
	}

		
	if (IsValid(OnHitSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnHitSound, HitActor->GetActorLocation());	
	}
	Projectile->Destroy();

	if (CurrentBounceCount >= MaxBounceCount)
		return;
	
	TArray<UASAttributsManager*> NearbyAttributsManager = GetNearbyAttributsManagers(DetectionRadius, HitResult.Location);
	if (NearbyAttributsManager.Num() == 0)
		return;

	float ShortestDistance = DetectionRadius + 500;
	UASAttributsManager* TargetAttributsManager = nullptr;
	ETeam CasterTeam = CasterAttributsManager->GetTeam();
	bool IsCasterInRadius = false;
	for (auto AttributsManager : NearbyAttributsManager)
	{
		if (AttributsManager == nullptr || AttributsManager == OnCastTargetAttributesManager)
			continue;
		
		if (AttributsManager->GetTeam() == CasterTeam)
		{
			if (AttributsManager == CasterAttributsManager)
			{
				IsCasterInRadius = true;
			}
			continue;
		}

		float Distance = FVector::Distance(AttributsManager->GetOwner()->GetActorLocation(), HitActor->GetActorLocation());
		if (Distance < ShortestDistance)
		{
			ShortestDistance = Distance;
			TargetAttributsManager = AttributsManager;
		}
	}

	if (TargetAttributsManager == nullptr && IsCasterInRadius)
	{
		TargetAttributsManager = CasterAttributsManager;
	}
	else if (TargetAttributsManager == nullptr)
		return;

	CurrentBounceCount++;
	FVector SpawnLocation = HitResult.Location;
	FRotator SpawnRotation = NormalImpulse.Rotation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AASProjectile_Targetted* NewProjectile = GetWorld()->SpawnActor<AASProjectile_Targetted>(ProjectilePrefab, SpawnLocation, SpawnRotation, SpawnParameters);
	NewProjectile->InitializeTargettedProjectile(TargetAttributsManager, ProjectileSpeed);

	
	UASAbilityProjectile_Pyroclasm* VolatileAbility = Cast<UASAbilityProjectile_Pyroclasm>(CreateAbilityInstance(NewProjectile));
	VolatileAbility->SetCurrentBounceCount(CurrentBounceCount);
	VolatileAbility->SetOnCastTargetAttributsManager(TargetAttributsManager);
	VolatileAbility->InitializeDuplicate(OwningCharacter);
	NewProjectile->OnHitDelegate.BindDynamic(VolatileAbility, &UASAbilityProjectile::OnHitTargetCallback);
}
