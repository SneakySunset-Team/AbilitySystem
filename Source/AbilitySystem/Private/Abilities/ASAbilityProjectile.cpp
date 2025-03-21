#include "Abilities/ASAbilityProjectile.h"
#include "ASProjectile.h"

void UASAbilityProjectile::Initialize(AASCharacter* InOwner, bool IsPersistantAbility)
{
	Super::Initialize(InOwner, IsPersistantAbility);
}

void UASAbilityProjectile::EndCasting(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	Super::EndCasting(NotifyName, Payload);

	//TODO: Set LOCATION and ROTATION of new Projectile on spawn
	FVector SpawnLocation;
	FRotator SpawnRotation;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AASProjectile* Projectile = GetWorld()->SpawnActor<AASProjectile>(ProjectilePrefab, SpawnLocation, SpawnRotation, SpawnParameters);

	UASAbility* VolatileAbility = CreateAbilityInstance(Projectile);
	Projectile->OnHitDelegate.BindDynamic(VolatileAbility, &UASAbility::TriggerAbilityEffects);
}
