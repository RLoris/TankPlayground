// Copyright RLoris 2021

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "../Pawns/Tank.h"
#include "../Projectiles/TankProjectile.h"

// Sets default values
ATankProjectile::ATankProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// sphere collision
	USphereComponent* Root = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// bind overlap
	Root->OnComponentBeginOverlap.AddDynamic(this, &ATankProjectile::OnBeginOverlap);
	RootComponent = Root;
	Root->SetSphereRadius(10);
	// projectile mesh
	UStaticMeshComponent* Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	Projectile->SetStaticMesh(Sphere.Object);
	Projectile->SetRelativeScale3D(FVector(0.2));
	Projectile->SetCollisionProfileName(FName("NoCollision"), false);
	// projectile material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereMat(TEXT("/Engine/EditorMaterials/Camera/CineMat.CineMat"));
	Projectile->SetMaterial(0, SphereMat.Object);
}

// Called when the game starts or when spawned
void ATankProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->Tank = Cast<ATank>(this->GetOwner());
	this->InitialVelocity = GetActorForwardVector() * 300;
	this->InitialLocation = GetActorLocation();
	this->InitialTime = GetWorld()->GetTimeSeconds();
}

void ATankProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		ATank* OtherTank = Cast<ATank>(OtherActor);
		// tank hit ?
		if (OtherTank && this->Tank)
		{
			OtherTank->HandleHit(this->Tank);
		}

		Destroy();
	}
}

// Called every frame
void ATankProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentTime = GetWorld()->GetTimeSeconds() - this->InitialTime;

	FVector NewLocation;
	NewLocation.X = this->InitialLocation.X + 5 * this->InitialVelocity.X * CurrentTime;
	NewLocation.Y = this->InitialLocation.Y + 5 * this->InitialVelocity.Y * CurrentTime;
	NewLocation.Z = this->InitialLocation.Z + 5 * this->InitialVelocity.Z * CurrentTime - (5 * CurrentTime) * (5 * CurrentTime) * (9.81 * 2) / 2;

	SetActorLocation(NewLocation, true);
}

