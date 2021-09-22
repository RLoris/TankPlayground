// Copyright RLoris 2021

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Projectiles/TankProjectile.h"
#include "../Spawners/TankSpawner.h"
#include "../CameraShakes/HitTankCameraShake.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Auto possess for test
	// AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Box collision
	UBoxComponent* Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Root;
	Root->SetBoxExtent(FVector(200, 100, 40), false);
	Root->SetSimulatePhysics(true);
	Root->SetLinearDamping(0);
	Root->SetCollisionProfileName(FName("Pawn"), false);
	// Camera
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(-20, 0, 0));
	// Spring Arm
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 800.f;
	SpringArm->SocketOffset = FVector(0, 0, 200);
	SpringArm->SetRelativeLocation(FVector(0, 0, 100));
	// Attach camera to spring arm
	Camera->SetupAttachment(SpringArm);
	// Attach springarm to root
	SpringArm->SetupAttachment(RootComponent);
	// Tank body
	UStaticMeshComponent* Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	Body->SetStaticMesh(Cube.Object);
	Body->SetRelativeScale3D(FVector(4, 2, 0.75));
	// body material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TankMat(TEXT("/Game/Materials/M_Tank.M_Tank"));
	Body->SetMaterial(0, TankMat.Object);
	// Tank Turret
	UStaticMeshComponent* Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	Turret->SetupAttachment(Body);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	Turret->SetStaticMesh(Sphere.Object);
	// turret material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TurretMat(TEXT("/Engine/EditorMaterials/Camera/CineMat.CineMat"));
	Turret->SetMaterial(0, TurretMat.Object);
	Turret->SetRelativeLocation(FVector(0, 0, 50));
	Turret->SetRelativeScale3D(FVector(0.25, 0.5, 1.33));
	// Tank Canon
	UStaticMeshComponent* Canon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Canon"));
	Canon->SetupAttachment(Turret);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	Canon->SetStaticMesh(Cylinder.Object);
	// Canon material
	Canon->SetMaterial(0, TurretMat.Object);
	Canon->SetRelativeLocation(FVector(110, 0, 30));
	Canon->SetRelativeScale3D(FVector(0.23, 0.23, 1.76));
	Canon->SetRelativeRotation(FRotator(105, 0, 0));
	// projectile spawn
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileSpawn"));
	this->ProjectileSpawn = SphereComponent;
	SphereComponent->bHiddenInGame = false;
	SphereComponent->SetSphereRadius(10);
	SphereComponent->SetRelativeRotation(FRotator(15, 0, 0));
	SphereComponent->SetupAttachment(Root);
	SphereComponent->SetRelativeLocation(FVector(210, 0, 95));

	HitCameraShake = UHitTankCameraShake::StaticClass();
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATank::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	// Clear timers
	GetWorld()->GetTimerManager().ClearTimer(this->ShootAITimer);
	GetWorld()->GetTimerManager().ClearTimer(this->TurnAITimer);
	// GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ATank::PossessedBy(AController* NewController)
{
	if (Cast<APlayerController>(NewController))
	{
		this->IsPlayer = true;
		this->ForwardSpeed = 500;
		this->TurnSpeed = 30;
	}
	else
	{
		this->IsPlayer = false;
		this->ForwardSpeed = 200;
		GetWorld()->GetTimerManager().SetTimer(this->TurnAITimer, [this]() {
			this->RandomTurnSpeed();
		}, 1.0f, true, 1.0f);
		GetWorld()->GetTimerManager().SetTimer(this->ShootAITimer, [this]() {
			this->ShootPlayer();
		}, 0.2f, true);
	}
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// AI
	if (!IsPlayer)
	{
		this->MoveForward(1);
		this->TurnRight(1);
	}

	FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	SetActorLocation(NewLocation);
	FRotator NewRotation = GetActorRotation() + (TargetRotation * DeltaTime);
	NewRotation.Roll = FMath::ClampAngle(NewRotation.Roll, -50, 50);
	NewRotation.Pitch = FMath::ClampAngle(NewRotation.Pitch, -50, 50);
	SetActorRotation(NewRotation);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ATank::MoveForward);
	PlayerInputComponent->BindAxis("TurnRight", this, &ATank::TurnRight);
	// Action
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ATank::Shoot);
}


void ATank::MoveForward(float Axis)
{
	CurrentVelocity = GetActorForwardVector() * (FMath::Clamp(Axis, -1.0f, 1.0f) * this->ForwardSpeed);
}

void ATank::TurnRight(float Axis)
{
	TargetRotation.Yaw = FMath::Clamp(Axis, -1.0f, 1.0f) * this->TurnSpeed;
}

void ATank::Shoot()
{
	FTransform Transform = this->ProjectileSpawn->GetComponentTransform();
	FActorSpawnParameters Parameters;
	Parameters.Owner = this;
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<ATankProjectile>(ATankProjectile::StaticClass(), Transform, Parameters);
}

void ATank::HandleHit(ATank* OtherTank)
{
	if (OtherTank)
	{
		this->HitCount++;
		if (IsPlayer)
		{
			// play camera shake
			UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(HitCameraShake, 1.0f);
		}
		if (this->HitCount >= 3)
		{
			OtherTank->HandleScore();
			if (ATankSpawner* Spawner = Cast<ATankSpawner>(GetOwner()))
			{
				Spawner->DestroyTank(this);
			}
		}
	}
}

void ATank::HandleScore()
{
	this->HitScore++;
}

void ATank::RandomTurnSpeed()
{
	this->TurnSpeed = FMath::RandRange(-50, 50);
}

bool ATank::IsPlayerInRange()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn)
	{
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		FVector Distance = PlayerLocation - GetActorLocation();
		Distance.Normalize();
		// angle from dot product in degrees
		float FOVAngleDeg = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Distance, GetActorForwardVector())));
		// is player in fov of tank
		return FOVAngleDeg <= 45 && FOVAngleDeg >= -45;
	}
	return false;
}

void ATank::ShootPlayer()
{
	if (this->IsPlayerInRange())
	{
		this->Shoot();
	}
}