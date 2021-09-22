// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FastNoiseWrapper.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "TerrainGenerator.generated.h"

UCLASS()
class TANKPLAYGROUND_API ATerrainGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// construction script
	virtual void OnConstruction(const FTransform& Transform) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 SizeX = 10;
	UPROPERTY(EditAnywhere)
	int32 SizeY = 10;
	UPROPERTY(EditAnywhere)
	float TileSize = 100.0f;
	UPROPERTY(EditAnywhere)
	float MinHeight = -100;
	UPROPERTY(EditAnywhere)
	float MaxHeight = 100;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UFastNoiseWrapper* FNG;
	URuntimeMeshComponentStatic* RuntimeMesh;
};
