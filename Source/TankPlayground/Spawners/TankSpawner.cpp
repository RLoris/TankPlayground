// Copyright RLoris 2021

#include "TankSpawner.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "../TankPlaygroundGameModeBase.h"

// Sets default values
ATankSpawner::ATankSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Zone"));
	RootComponent = Zone;
	Zone->bHiddenInGame = false;
	Zone->SetCollisionProfileName(FName("Custom"), false);
	Zone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Zone->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Zone->OnComponentEndOverlap.AddDynamic(this, &ATankSpawner::OnEndOverlap);
}

// Called when the game starts or when spawned
void ATankSpawner::BeginPlay()
{
	Super::BeginPlay();
	SetupZone();
	GameMode = Cast<ATankPlaygroundGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->OnRoundStart.AddDynamic(this, &ATankSpawner::SpawnTanks);
		GameMode->SetupGame(this);
	}
}

void ATankSpawner::OnConstruction(const FTransform& Transform)
{
	SetupZone();
}

void ATankSpawner::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATank* Tank = Cast<ATank>(OtherActor);
	if (Tank)
	{
		Tank->TeleportTo(GetRandomLocation(), Tank->GetActorRotation());
	}
}

FVector ATankSpawner::GetRandomLocation()
{
	FVector RandomDest;
	RandomDest.X = FMath::RandRange(0.0f, (Terrain->SizeX - 1) * Terrain->TileSize);
	RandomDest.Y = FMath::RandRange(0.0f, (Terrain->SizeY - 1) * Terrain->TileSize);
	RandomDest.Z = Zone->Bounds.BoxExtent.Z;
	return Terrain->GetActorLocation() + RandomDest;
}

void ATankSpawner::SpawnTanks(int32 Count)
{
	DespawnTanks();
	for (int32 i = 0; i <= Count; i++)
	{
		FActorSpawnParameters Parameters;
		Parameters.Owner = this;
		Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector Location = GetRandomLocation();
		ATank* Tank = GetWorld()->SpawnActor<ATank>(Location, FRotator(0), Parameters);
		if (i == Count)
		{
			// Player
			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			PC = PC ? PC : GetWorld()->SpawnActor<APlayerController>(Location, FRotator(0), Parameters);
			PC->Possess(Tank);
		}
		else
		{
			// AI
			AAIController* AIC = GetWorld()->SpawnActor<AAIController>(Location, FRotator(0), Parameters);
			AIC->Possess(Tank);
		}
		Pawns.Add(Tank);
	}
}

void ATankSpawner::DespawnTanks()
{
	for (int32 i = Pawns.Num() - 1; i >= 0; i--)
	{
		ATank* T = Pawns[i];
		if (T && !T->IsActorBeingDestroyed())
		{
			T->Destroy();
		}
		Pawns.RemoveAt(i);
	}
	Pawns.Empty();
}

void ATankSpawner::SetupZone()
{
	if (Terrain)
	{
		// update zone extent
		FVector BoxExtent;
		BoxExtent.X = (Terrain->SizeX * Terrain->TileSize) / 2;
		BoxExtent.Y = (Terrain->SizeY * Terrain->TileSize) / 2;
		BoxExtent.Z = (FMath::Abs(Terrain->MinHeight) + FMath::Abs(Terrain->MaxHeight)) * 2;
		Zone->SetBoxExtent(BoxExtent, false);
		// update location to center of the terrain
		Zone->SetWorldLocation(Terrain->GetActorLocation() + FVector(0,0,FMath::Abs(Terrain->MinHeight) * -1) + BoxExtent);
	}
}

// Called every frame
void ATankSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATankSpawner::DestroyTank(ATank* Tank)
{
	if (Tank && (Pawns.Remove(Tank) != INDEX_NONE))
	{
		if (Pawns.Num() <= 1 || Tank->IsPlayer)
		{
			GameMode->EndGame((Pawns.Num() > 0 && Pawns[0]->IsPlayer));
		}
		Tank->Destroy();
	}
}

