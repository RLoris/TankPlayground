// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MatineeCameraShake.h"
#include "Templates/SubclassOf.h"
#include "Tank.generated.h"

UCLASS()
class TANKPLAYGROUND_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Inputs handler
	void MoveForward(float Axis);
	void TurnRight(float Axis);
	void Shoot();

	// handle hit & score
	void HandleHit(ATank* OtherTank);
	void HandleScore();

	// AI utils
	void RandomTurnSpeed();
	bool IsPlayerInRange();
	void ShootPlayer();

	USceneComponent* ProjectileSpawn;
	// Velocity
	FVector CurrentVelocity;
	// Rotation
	FRotator TargetRotation;
	// score & live
	int32 HitCount = 0;
	int32 HitScore = 0;
	bool IsPlayer = false;
	// speed
	float ForwardSpeed = 0;
	float TurnSpeed = 0;
	// AI timers
	FTimerHandle TurnAITimer;
	FTimerHandle ShootAITimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> HitCameraShake;
};
