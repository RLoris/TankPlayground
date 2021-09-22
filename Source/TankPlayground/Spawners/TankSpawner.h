// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../Procedurals/TerrainGenerator.h"
#include "../Pawns/Tank.h"
#include "../TankPlaygroundGameModeBase.h"
#include "TankSpawner.generated.h"

UCLASS()
class TANKPLAYGROUND_API ATankSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// construction script
	virtual void OnConstruction(const FTransform& Transform) override;
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	FVector GetRandomLocation();
	UFUNCTION()
	void SpawnTanks(int32 Count);
	void DespawnTanks();
	void SetupZone();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called when a tank is destroyed
	void DestroyTank(ATank* Tank);

	UPROPERTY(EditAnywhere)
	ATerrainGenerator* Terrain;

	UPROPERTY(EditAnywhere)
	int32 TankCount = 5;
	// gamemode
	ATankPlaygroundGameModeBase* GameMode = nullptr;
	UBoxComponent* Zone;
	// Pawns spawned
	TArray<ATank*> Pawns;
};
