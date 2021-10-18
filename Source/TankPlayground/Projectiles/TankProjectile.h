// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Pawns/Tank.h"
#include "TankProjectile.generated.h"

// forward declarations
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class TANKPLAYGROUND_API ATankProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	ATank* Tank;
	UProjectileMovementComponent* ProjectileMovement;
};
