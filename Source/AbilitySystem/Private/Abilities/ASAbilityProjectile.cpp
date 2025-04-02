#include "Abilities/ASAbilityProjectile.h"
#include "Projectiles/ASProjectile.h"
#include "CharacterSystems/ASAttributsManager.h"
#include "CharacterSystems/ASCharacter.h"
#include "Effects/ASEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/ASProjectile_Targetted.h"

void UASAbilityProjectile::StartCasting()
{

	Super::StartCasting();

}

void UASAbilityProjectile::InitializePersistant(AASCharacter* InOwner)
{
	if (ProjectilePrefab->IsChildOf(AActor::StaticClass()) && !IsTargettedAbility)
	{
		UE_LOG(LogTemp, Error, TEXT("Has a Targetted Projectile but is not Targetted"));
	}
	Super::InitializePersistant(InOwner);
}

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
	
	AASProjectile_Targetted* TargettedProjectile = Cast<AASProjectile_Targetted>(Projectile);
	if (TargettedProjectile)
	{
		TargettedProjectile->InitializeTargettedProjectile(OnCastTargetAttributesManager, ProjectileSpeed);
	}
	else
	{
		Projectile->Initialize(ProjectileMaxDistance, ProjectileSpeed);
	}
	
	UASAbilityProjectile* VolatileAbility = Cast<UASAbilityProjectile>(CreateAbilityInstance(Projectile));
	Projectile->OnHitDelegate.BindDynamic(VolatileAbility, &UASAbilityProjectile::OnHitTargetCallback);
	Projectile->OnMaxDistanceReachedDelegate.BindDynamic(VolatileAbility, &UASAbilityProjectile::OnProjectileMaxDistanceReachedCallback);
}

void UASAbilityProjectile::OnHitTargetCallback(AActor* HitActor, FVector NormalImpulse, const FHitResult& HitResult, AActor* Projectile)
{
	OnProjectileHit.Broadcast(OwningCharacter->GetAttributsManager());
	if (HitActor->FindComponentByClass<UASAttributsManager>())
	{
		if (OnHitParticle != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitParticle, HitResult.Location, NormalImpulse.Rotation(), true);
		}


		if (IsValid(OnHitSound))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnHitSound, HitActor->GetActorLocation());	
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
