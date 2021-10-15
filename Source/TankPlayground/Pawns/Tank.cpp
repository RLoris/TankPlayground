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
#include "../Controllers/TankPlayerController.h"
#include "../Widgets/WidgetHUD.h"
#include "../Widgets/WidgetBase.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SetCanBeDamaged(true);
	// Auto possess for test
	// AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Box collision
	UBoxComponent* Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Root;
	Root->SetBoxExtent(FVector(200, 100, 40), false);
	Root->SetSimulatePhysics(true);
	Root->SetLinearDamping(0);
	Root->SetCollisionProfileName(FName("Pawn"), false);
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
	TurretMesh = Turret;
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
	SphereComponent->SetSphereRadius(50);
	SphereComponent->SetRelativeRotation(FRotator(-90, 0, 0));
	SphereComponent->SetupAttachment(Canon);
	SphereComponent->SetWorldLocation(FVector(0, 0, -55));
	SphereComponent->SetWorldScale3D(FVector(1, 1, 1));
	
	// Camera third person
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPCamera"));
	Camera->SetRelativeRotation(FRotator(-20, 0, 0));
	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSpringArm"));
	// only yaw movement for camera
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 800.f;
	SpringArm->SocketOffset = FVector(0, 0, 200);
	SpringArm->SetRelativeLocation(FVector(0, 0, 100));
	// Attach camera to spring arm
	Camera->SetupAttachment(SpringArm);
	// Attach springarm to turret to follow movement
	SpringArm->SetupAttachment(Turret);

	// camera shake
	HitCameraShake = UHitTankCameraShake::StaticClass();

	// projectile sound
	static ConstructorHelpers::FObjectFinder<USoundBase> ProjectileShootSound(TEXT("/Game/Sounds/S_TankProjectileShoot.S_TankProjectileShoot"));
	ShootSound = ProjectileShootSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> ProjectileHitSound(TEXT("/Game/Sounds/S_TankProjectileHit.S_TankProjectileHit"));
	HitSound = ProjectileHitSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> TankExplosionSound(TEXT("/Game/Sounds/S_TankExplosion.S_TankExplosion"));
	ExplosionSound = TankExplosionSound.Object;
	
	static ConstructorHelpers::FObjectFinder<USoundBase> DrivingSound(TEXT("/Game/Sounds/S_TankDriving.S_TankDriving"));
	DrivingAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("S_TankDriving"));
	DrivingAudio->SetSound(DrivingSound.Object);
	DrivingAudio->SetAutoActivate(false);
	DrivingAudio->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<USoundBase> TurretSound(TEXT("/Game/Sounds/S_TurretRotation.S_TurretRotation"));
	TurretAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("S_TurretRotating"));
	TurretAudio->SetSound(TurretSound.Object);
	TurretAudio->SetAutoActivate(false);
	TurretAudio->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<USoundBase> CanonSound(TEXT("/Game/Sounds/S_CanonRotation.S_CanonRotation"));
	CanonAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("S_CanonRotating"));
	CanonAudio->SetSound(CanonSound.Object);
	CanonAudio->SetAutoActivate(false);
	CanonAudio->SetupAttachment(Root);

	// curve timeline for camera switch
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CameraCurve(TEXT("/Game/Curve/CameraCurve.CameraCurve"));
	FOnTimelineFloat ProgressEvent;
	ProgressEvent.BindDynamic(this, &ATank::CameraTimelineProgress);
	CameraTimeline.AddInterpFloat(CameraCurve.Object, ProgressEvent);
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
	if (ATankPlayerController* PlayerController = Cast<ATankPlayerController>(NewController))
	{
		// show HUD
		PC = PlayerController;
		PC->ShowHUD();
		if (PC->WidgetHUD) 
		{
			IWidgetHUD::Execute_UpdateHealth(PC->WidgetHUD, this->Health);
		}
		
		this->IsPlayer = true;
		this->ForwardSpeed = 500;
		this->TurnSpeed = 30;
	}
	else
	{
		PC = nullptr;
		this->IsPlayer = false;
		this->ForwardSpeed = 200;
		GetWorld()->GetTimerManager().SetTimer(this->TurnAITimer, [this]() {
			this->RandomTurnSpeed();
		}, 1.0f, true, 1.0f);
		GetWorld()->GetTimerManager().SetTimer(this->ShootAITimer, [this]() {
			this->ShootPlayer();
		}, 0.20f, true);
	}
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// deal damage
	this->Health = FMath::Clamp<float>(Health - DamageAmount, 0, 100.0f);
	// other tank hit
	ATank* OtherTank = Cast<ATank>(DamageCauser);
	if (OtherTank)
	{
		this->HandleHitFrom(OtherTank);
	}
	return 0.0f;
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraTimeline.TickTimeline(DeltaTime);

	// AI
	if (!IsPlayer)
	{
		this->MoveForward(1);
		this->TurnRight(1);
	}

	// playing moving sound
	if (CurrentVelocity.IsZero())
	{
		if (DrivingAudio->IsPlaying())
		{
			DrivingAudio->Stop();
		}
	}
	else
	{
		if (!DrivingAudio->IsPlaying())
		{
			DrivingAudio->Play();
		}
	}

	// pawn location
	FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	SetActorLocation(NewLocation);
	// pawn rotation
	FRotator NewRotation = GetActorRotation() + (TargetRotation * DeltaTime);
	NewRotation.Roll = FMath::ClampAngle(NewRotation.Roll, -50, 50);
	NewRotation.Pitch = FMath::ClampAngle(NewRotation.Pitch, -50, 50);
	SetActorRotation(NewRotation);
	// turret & canon rotation
	FRotator NewTurretRotation = TurretMesh->GetRelativeRotation() + (TurretRotation * DeltaTime);
	NewTurretRotation.Pitch = FMath::Clamp(NewTurretRotation.Pitch, -10.0f, 15.0f);
	TurretMesh->SetRelativeRotation(NewTurretRotation);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ATank::MoveForward);
	PlayerInputComponent->BindAxis("TurnRight", this, &ATank::TurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ATank::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ATank::LookRight);
	// Action
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ATank::Shoot);
	PlayerInputComponent->BindAction("View", EInputEvent::IE_Pressed, this, &ATank::View);
}

void ATank::CameraTimelineProgress(float Value)
{
	SpringArm->TargetArmLength = FMath::Lerp(800.f, 50.f, Value);
	SpringArm->SocketOffset = FMath::Lerp(FVector(0, 0, 200), FVector(0, 0, 50), Value);
	SpringArm->SetRelativeLocation(FMath::Lerp(FVector(0, 0, 100), FVector(0, 0, 50), Value));
	SpringArm->SetRelativeRotation(FMath::Lerp(FRotator(0), FRotator(20, 0, 0), Value));
}

void ATank::MoveForward(float Axis)
{
	CurrentVelocity = GetActorForwardVector() * (FMath::Clamp(Axis, -1.0f, 1.0f) * this->ForwardSpeed);
}

void ATank::TurnRight(float Axis)
{
	TargetRotation.Yaw = FMath::Clamp(Axis, -1.0f, 1.0f) * this->TurnSpeed;
}

void ATank::LookUp(float Axis)
{
	if (Axis == 0)
	{
		if (CanonAudio->IsPlaying())
		{
			CanonAudio->Stop();
		}
	}
	else
	{
		if (!CanonAudio->IsPlaying())
		{
			CanonAudio->Play();
		}
	}
	TurretRotation.Pitch = FMath::Clamp(Axis, -1.0f, 1.0f) * 30;
}

void ATank::LookRight(float Axis)
{
	if (Axis == 0)
	{
		if (TurretAudio->IsPlaying())
		{
			TurretAudio->Stop();
		}
	}
	else
	{
		if (!TurretAudio->IsPlaying())
		{
			TurretAudio->Play();
		}
	}
	TurretRotation.Yaw = FMath::Clamp(Axis, -1.0f, 1.0f) * 40;
}

void ATank::Shoot()
{
	if ((GetWorld()->GetTimeSeconds() - this->LastShot) >= 0.5f) 
	{
		// spawn projectile
		FTransform Transform = this->ProjectileSpawn->GetComponentTransform();
		Transform.SetScale3D(FVector(1));
		FActorSpawnParameters Parameters;
		Parameters.Owner = this;
		Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ATankProjectile>(ATankProjectile::StaticClass(), Transform, Parameters);
		// play shoot sound
		UGameplayStatics::PlaySoundAtLocation(this, ShootSound, this->ProjectileSpawn->GetComponentLocation());
		// save current time for shoot delay
		this->bCanShoot = false;
		this->LastShot = GetWorld()->GetTimeSeconds();
	}
}

void ATank::View()
{
	if (!bAimView)
	{
		bAimView = true;
		SpringArm->bInheritPitch = true;
		CameraTimeline.Play();
	}
	else
	{
		bAimView = false;
		SpringArm->bInheritPitch = false;
		CameraTimeline.Reverse();
	}
}

void ATank::HandleHitFrom(ATank* OtherTank)
{
	if (OtherTank)
	{
		// play hit sound
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		if (IsPlayer)
		{
			if (PC->WidgetHUD)
			{
				IWidgetHUD::Execute_UpdateHealth(PC->WidgetHUD, this->Health);
			}
			// play camera shake
			PC->ClientStartCameraShake(HitCameraShake, 1.0f);
		}
		if (this->Health <= 0)
		{
			// increment score other tank
			OtherTank->HandleScore();
			// play explosion sound
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
			// destroy current
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