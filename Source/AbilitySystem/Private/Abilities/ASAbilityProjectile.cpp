#include "Abilities/ASAbilityProjectile.h"
#include "ASProjectile.h"
#include "CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Effects/ASEffect.h"

void UASAbilityProjectile::InitializeDuplicate(AASCharacter* InOwner)
{
	Super::InitializeDuplicate(InOwner);
	
	for (const auto EffectPrefab : EffectsPrefabs)
	{
		FOnAbilityTrigger& EffectDelegate = OnCastAnimationStart; 
		
		bool IsCastFromPersistant = true;
		switch (Effects.Last()->GetTriggerType())
		{
		case ETriggerType::OnHit:
			break;
		case ETriggerType::OnHitTarget:
			EffectDelegate = OnCastAnimationTrigger;
			break;
		case ETriggerType::OnProjectileReachMaxDistance:
			EffectDelegate = OnCastAnimationEnd;
			break;
		default:
			IsCastFromPersistant = false;
			break;
		}

		if (IsCastFromPersistant)
		{
			Effects.Add(NewObject<UASEffect>(this, EffectPrefab));
			Effects.Last()->Initialize(OwningCharacter->GetAttributsManager()->GetAttributs());
			EffectDelegate.AddDynamic(Effects.Last(), UASEffect::ApplyEffect);
		}
	}
}

void UASAbilityProjectile::OnTriggerAnimationEventCallback(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	Super::OnTriggerAnimationEventCallback(NotifyName, Payload);

	//TODO: Set LOCATION and ROTATION of new Projectile on spawn
	FVector SpawnLocation;
	FRotator SpawnRotation;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AASProjectile* Projectile = GetWorld()->SpawnActor<AASProjectile>(ProjectilePrefab, SpawnLocation, SpawnRotation, SpawnParameters);

	UASAbilityProjectile* VolatileAbility = Cast<UASAbilityProjectile>(CreateAbilityInstance(Projectile));
	Projectile->OnHitDelegate.BindDynamic(VolatileAbility, &UASAbilityProjectile::OnHitTargetCallback);
	Projectile->OnMaxDistanceReachedDelegate.BindDynamic(VolatileAbility, &UASAbilityProjectile::OnProjectileMaxDistanceReachedCallback);
}

void UASAbilityProjectile::OnHitTargetCallback(AActor* HitActor, FVector NormalImpulse, const FHitResult& HitResult)
{
	OnAbilityHit.Broadcast(OwningCharacter->GetAttributsManager());
	if (HitActor->FindComponentByClass<UASAttributsManager>())
	{
		UASAttributsManager* AttributesManager = HitActor->GetComponentByClass<UASAttributsManager>();
		OnAbilityHitAttributsManager.Broadcast(AttributesManager);
	}
}

void UASAbilityProjectile::OnProjectileMaxDistanceReachedCallback()
{
	OnAbilityReachMaxDistance.Broadcast(OwningCharacter->GetAttributsManager());
}
