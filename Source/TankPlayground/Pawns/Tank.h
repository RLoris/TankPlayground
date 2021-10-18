// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MatineeCameraShake.h"
#include "Templates/SubclassOf.h"
#include "Components/TimelineComponent.h"
#include "Tank.generated.h"

// forward declarations
class ATankPlayerController;
class UAudioComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class USplineComponent;
class UStaticMesh;
class USplineMeshComponent;
class UMaterialInterface;
class UMaterialParameterCollection;
class UTimelineComponent;

UCLASS(Blueprintable, BlueprintType)
class TANKPLAYGROUND_API ATank : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ATank();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
	// called when actor is damaged
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
private:
	// axis handler
	void MoveForward(float Axis);
	void TurnRight(float Axis);
	void LookUp(float Axis);
	void LookRight(float Axis);
	// action handler
	void Shoot();
	void View();
	// AI utils
	void RandomTurnSpeed();
	bool IsPlayerInRange();
	void ShootPlayer();
	// handle hit & score
	void HandleHitFrom(ATank* OtherTank);
	void HandleScore();
	// camera timeline
	UFUNCTION()
	void CameraTimelineProgress(float Value);
	// projectile path
	void PredictProjectilePath();
	void ClearProjectilePath();
	UFUNCTION()
	void HighlightProjectileZone();
	UFUNCTION()
	void DeHighlightProjectileZone();
public:
	// Velocity
	FVector CurrentVelocity;
	// Rotation
	FRotator TargetRotation;
	FRotator TurretRotation;
	// score & live
	UPROPERTY(BlueprintReadOnly)
	float Health = 100.0f;
	int32 HitScore = 0;
	bool IsPlayer = false;
	// speed
	float ForwardSpeed = 0;
	float TurnSpeed = 0;
	// AI timers
	FTimerHandle TurnAITimer;
	FTimerHandle ShootAITimer;
	// camera shake
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> HitCameraShake;
	// shoot
	float LastShot = 0.0f;
	// view
	bool bAimView = false;
private:
	// turret
	UStaticMeshComponent* TurretMesh;
	UAudioComponent* DrivingAudio;
	UAudioComponent* TurretAudio;
	UAudioComponent* CanonAudio;
	// controller
	ATankPlayerController* PC;
	// sounds
	USoundBase* ShootSound;
	USoundBase* HitSound;
	USoundBase* ExplosionSound;
	USoundBase* MovingSound;
	// timeline
	USpringArmComponent* SpringArm;
	UTimelineComponent* CameraTimeline;
	// projectile
	USceneComponent* ProjectileSpawn;
	UStaticMeshComponent* ProjectileZoneMesh;
	USplineComponent* ProjectileSpline;
	UStaticMesh* ProjectileSplineMesh;
	UMaterialInterface* ProjectileSplineMaterial;
	TArray<USplineMeshComponent*> ProjectileSplineComponents;
	UMaterialParameterCollection* ProjectileMPC;
};
