#include "ASProjectile.h"

#include "CharacterSystems/ASAttributsManager.h"


AASProjectile::AASProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);
}

void AASProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->OnComponentHit.AddDynamic(this, &AASProjectile::OnHit);
}

void AASProjectile::Initialize(float InMaxDistance, float InSpeed)
{
	MaxDistance = InMaxDistance;
	Speed = InSpeed;
	StartingLocation = GetActorLocation();
}

void AASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime);
	if (FVector::Distance(StartingLocation, NewLocation) > MaxDistance)
	{
		NewLocation = StartingLocation + GetActorForwardVector() * MaxDistance;
		Destroy();
	}
	SetActorLocation(NewLocation);
}

void AASProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->FindComponentByClass<UASAttributsManager>())
	{
		UASAttributsManager* AttributesManager = OtherActor->GetComponentByClass<UASAttributsManager>();		
		OnHitDelegate.ExecuteIfBound(AttributesManager);
	}
}

