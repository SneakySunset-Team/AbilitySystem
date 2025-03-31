#include "Abilities/ASAbilityProjectile.h"
#include "ASProjectile.h"
#include "CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Effects/ASEffect.h"
#include "Kismet/GameplayStatics.h"

void UASAbilityProjectile::InitializeDuplicate(AASCharacter* InOwner)
{
	Super::InitializeDuplicate(InOwner);

	for (const auto EffectPrefab : EffectsPrefabs)
	{
		bool IsCastFromDuplicate = true;
		
		UASEffect* Effect = NewObject<UASEffect>(this, EffectPrefab);
		Effect->Initialize(OwningCharacter->GetAttributsManager());
		switch (Effect->GetActivationType())
		{
		case EASActivationType::OnHit:
			OnProjectileHit.AddDynamic(Effect, &UASEffect::ApplyEffect);
			break;
		case EASActivationType::OnHitTarget:
			OnProjectileHitAttributsManager.AddDynamic(Effect, &UASEffect::ApplyEffect);
			break;
		case EASActivationType::OnProjectileReachMaxDistance:
			OnProjectileReachMaxDistance.AddDynamic(Effect, &UASEffect::ApplyEffect);
			break;
		default:
			IsCastFromDuplicate = false;
			break;
		}

		if (IsCastFromDuplicate)
		{
			Effects.Add(Effect);
		}
	}
}

void UASAbilityProjectile::OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	Super::OnTriggerAnimationEventCallback(NotifyName, Payload);

	FVector SpawnLocation = OwningCharacter->GetMesh()->GetSocketLocation(BoneName);
	FRotator SpawnRotation = OwningCharacter->GetActorForwardVector().Rotation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AASProjectile* Projectile = GetWorld()->SpawnActor<AASProjectile>(ProjectilePrefab, SpawnLocation, SpawnRotation, SpawnParameters);
	Projectile->Initialize(ProjectileMaxDistance, ProjectileSpeed);
	UASAbilityProjectile* VolatileAbility = Cast<UASAbilityProjectile>(CreateAbilityInstance(Projectile));
	Projectile->OnHitDelegate.BindDynamic(VolatileAbility, &UASAbilityProjectile::OnHitTargetCallback);
	Projectile->OnMaxDistanceReachedDelegate.BindDynamic(VolatileAbility, &UASAbilityProjectile::OnProjectileMaxDistanceReachedCallback);
}

void UASAbilityProjectile::OnHitTargetCallback(AActor* HitActor, FVector NormalImpulse, const FHitResult& HitResult, AActor* Projectile)
{
	OnProjectileHit.Broadcast(OwningCharacter->GetAttributsManager());
	if (HitActor->FindComponentByClass<UASAttributsManager>())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHitTargetCallback, With name : %s"), *HitActor->GetName());
		if (OnHitParticle != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitParticle, HitResult.Location, NormalImpulse.Rotation(), true);
		}
		UASAttributsManager* AttributsManager = HitActor->GetComponentByClass<UASAttributsManager>();
		OnProjectileHitAttributsManager.Broadcast(AttributsManager);
		Projectile->Destroy();
	}
}

void UASAbilityProjectile::OnProjectileMaxDistanceReachedCallback()
{
	OnProjectileReachMaxDistance.Broadcast(OwningCharacter->GetAttributsManager());
}
