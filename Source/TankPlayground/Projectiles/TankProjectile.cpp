// Copyright RLoris 2021

#include "TankProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "../Pawns/Tank.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/PointLightComponent.h"

// Sets default values
ATankProjectile::ATankProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// sphere collision
	USphereComponent* Root = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// bind overlap
	RootComponent = Root;
	Root->OnComponentBeginOverlap.AddDynamic(this, &ATankProjectile::OnBeginOverlap);
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
	// projectile movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	// auto destroy after 10s
	InitialLifeSpan = 10.0f;
	// niagara system for smoke trail
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SmokeTrailNS(TEXT("/Game/Particles/NS_SmokeTrail.NS_SmokeTrail"));
	UNiagaraComponent* SmokeParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SmokeParticleSystem"));
	SmokeParticleSystem->SetupAttachment(Root);
	SmokeParticleSystem->SetAsset(SmokeTrailNS.Object);
	// point light
	UPointLightComponent* PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetIntensity(10000);
	PointLight->SetLightColor(FLinearColor::Red);
	PointLight->SetCastShadows(false);
	PointLight->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ATankProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->Tank = Cast<ATank>(this->GetOwner());
}

void ATankProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		// apply damage to other actor
		UGameplayStatics::ApplyDamage(OtherActor, 34.0f, this->Tank->GetController(), this->Tank, UDamageType::StaticClass());
		// play explosion
		static UParticleSystem* ExplosionSystem = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Particles/P_Explosion.P_Explosion"), nullptr, LOAD_None, nullptr);
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionSystem, SweepResult.Location, SweepResult.ImpactNormal.Rotation(), FVector(3));
		
		Destroy();
	}
}

// Called every frame
void ATankProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

