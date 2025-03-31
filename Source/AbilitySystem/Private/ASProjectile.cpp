#include "ASProjectile.h"

#include "CharacterSystems/ASAttributsManager.h"


AASProjectile::AASProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = MeshComponent;

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

	DistanceCrossed += Speed * DeltaTime;
	
	FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("%f"), DistanceCrossed);
	if (DistanceCrossed > MaxDistance)
	{
		Destroy();
	}
	SetActorLocation(NewLocation);
}

void AASProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnHitDelegate.ExecuteIfBound(OtherActor, NormalImpulse, Hit, this);
}

