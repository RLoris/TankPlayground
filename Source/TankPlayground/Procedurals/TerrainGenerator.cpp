// Copyright RLoris 2021

#include "TerrainGenerator.h"
#include "../Pawns/Tank.h"

// Sets default values
ATerrainGenerator::ATerrainGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FNG = CreateDefaultSubobject<UFastNoiseWrapper>(TEXT("FNG"));
	FNG->SetupFastNoise(EFastNoise_NoiseType::WhiteNoise);
	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponentStatic>(TEXT("RuntimeMesh"));
	RootComponent = RuntimeMesh;
	RuntimeMesh->SetCollisionProfileName(FName("BlockAll"), false);
	// RuntimeMesh->SetCollisionProfileName(FName("WorldStatic"), false);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> WorldMat(TEXT("/Engine/EngineMaterials/WorldGridMaterial.WorldGridMaterial"));
	Material = WorldMat.Object;
}

// Called when the game starts or when spawned
void ATerrainGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void ATerrainGenerator::OnConstruction(const FTransform& Transform)
{
	RuntimeMesh->RemoveAllSectionsForLOD(0);

	for (int32 Row = 0; Row < SizeY; Row++)
	{
		for (int32 Col = 0; Col < SizeX; Col++)
		{
			int32 Id = Row * SizeX + Col;
			FVector V1 = FVector(Col * TileSize, Row * TileSize, 0);
			FVector V2 = V1 + FVector(TileSize, TileSize, 0);
			FVector V3 = V1 + FVector(TileSize, 0, 0);
			FVector V4 = V1 + FVector(0, TileSize, 0);

			float Height = 0;
			Height = FMath::GetMappedRangeValueClamped(FVector2D(-1, 1), FVector2D(MinHeight, MaxHeight), FNG->GetNoise2D(V1.X, V1.Y));
			V1.Z = Height;
			Height = FMath::GetMappedRangeValueClamped(FVector2D(-1, 1), FVector2D(MinHeight, MaxHeight), FNG->GetNoise2D(V2.X, V2.Y));
			V2.Z = Height;
			Height = FMath::GetMappedRangeValueClamped(FVector2D(-1, 1), FVector2D(MinHeight, MaxHeight), FNG->GetNoise2D(V3.X, V3.Y));
			V3.Z = Height;
			Height = FMath::GetMappedRangeValueClamped(FVector2D(-1, 1), FVector2D(MinHeight, MaxHeight), FNG->GetNoise2D(V4.X, V4.Y));
			V4.Z = Height;

			TArray<FColor> Colors;
			TArray<FRuntimeMeshTangent> Tangents;
			TArray<FVector> Normals;
			TArray<FVector> Vertices{ V1, V2, V3, V4 };
			TArray<int32> Triangles{ 0, 1, 2, 1, 0, 3 };
			TArray<FVector2D> UVs{ FVector2D(1, 1), FVector2D(0, 0), FVector2D(1, 0), FVector2D(0, 1) };

			RuntimeMesh->CreateSectionFromComponents(0, Id, 0, Vertices, Triangles, Normals, UVs, Colors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, true);
		}
	}

	RuntimeMesh->SetMaterial(0, Material);
}

// Called every frame
void ATerrainGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

