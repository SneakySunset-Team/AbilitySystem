#include "Projectiles/ASProjectile_Targetted.h"

#include "CharacterSystems/ASAttributsManager.h"

AASProjectile_Targetted::AASProjectile_Targetted()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AASProjectile_Targetted::BeginPlay()
{
	Super::BeginPlay();
	
}

void AASProjectile_Targetted::Tick(float DeltaTime)
{
	if (!IsValid(TargetAttributManager))
	{
		Destroy();
		return;
	}
	
	FVector TargetLocation = TargetAttributManager->GetOwner()->GetActorLocation();
	FVector Direction = TargetLocation - GetActorLocation();
	Direction.Z = 0; // Ignore height difference
	Direction = Direction.GetSafeNormal();
        
	// Create rotation from direction
	FRotator NewRotation = Direction.Rotation();
        
	// Apply rotation instantly
	SetActorRotation(NewRotation);

	SetActorLocation(GetActorLocation() + Direction * Speed * DeltaTime);

	if (FVector::Distance(GetActorLocation(), TargetLocation) < 50)
	{
		FHitResult HitResult;
		HitResult.Location = GetActorLocation();
		OnHitDelegate.ExecuteIfBound(
			TargetAttributManager->GetOwner(),
			TargetAttributManager->GetOwner()->GetActorLocation() - GetActorLocation(),
			HitResult,
			this);
	}
}

void AASProjectile_Targetted::InitializeTargettedProjectile(UASAttributsManager* InTargetAttributManager, float InSpeed)
{
	Speed = InSpeed;
	TargetAttributManager = InTargetAttributManager;
}

void AASProjectile_Targetted::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

