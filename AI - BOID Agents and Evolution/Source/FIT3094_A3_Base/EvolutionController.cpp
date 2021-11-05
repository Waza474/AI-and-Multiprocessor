// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionController.h"

#include "Engine/World.h"

// Sets default values
AEvolutionController::AEvolutionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEvolutionController::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();

	if (BoidActor && World)
	{
		for (int i = 0; i < BOID_COUNT; i++)
		{
			float XPos = FMath::FRandRange(MIN_WORLD_SPAWN, MAX_WORLD_SPAWN_X);
			float YPos = FMath::FRandRange(MIN_WORLD_SPAWN, MAX_WORLD_SPAWN_Y);
			float ZPos = 100;
			FVector RandPosition = FVector(XPos, YPos, ZPos);
			// The random location may overlap something, this will make them slightly adjust or just spawncompletely.
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* Spawned = World->SpawnActor(BoidActor, &RandPosition, &FRotator::ZeroRotator, SpawnParam);

			ABoidAgent* BoidCast = Cast<ABoidAgent>(Spawned);
			if (BoidCast)
			{
				WorldBoids.Add(BoidCast);
				BoidCast->Spawner = this;
			}

			
			UE_LOG(LogTemp, Warning, TEXT("Boid Spawn | X: %f  Y: %f"), XPos, YPos);
		}
	}
}

// Called every frame
void AEvolutionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WorldBoids.Num() <= 40)
		childGeneration();
}

void AEvolutionController::boidDeath(ABoidAgent* killedBoid)
{
	if (killedBoid)
	{
		killedBoid->SetActorTickEnabled(false);
		killedBoid->SetActorLocation(FVector(-1000, -1000, 100));
		WorldBoids.Remove(killedBoid);
		DeadBoids.Add(killedBoid);
		//UE_LOG(LogTemp, Warning, TEXT("Evolution Controller: A Boid Agent has died. Remaing Boids: %d"), WorldBoids.Num());
	}
}

void AEvolutionController::childGeneration()
{
	UE_LOG(LogTemp, Warning, TEXT("Evolution Controller: NEW CHILD GENERATION"));

	// Fill a TArray of new boids
	while (BabyBoids.Num() <= 100)
	{
		int parentIndexA = FMath::RandRange(0, WorldBoids.Num() - 1);
		int parentIndexB = FMath::RandRange(0, WorldBoids.Num() - 1);

		ABoidAgent* parentA = WorldBoids[parentIndexA];
		ABoidAgent* parentB = WorldBoids[parentIndexB];

		// Crossover will use spawnNewBoid() which, adds a new boid to BabyBoids.
		parentA->Crossover(parentB);
		parentB->Crossover(parentA);
	}

	// Empty WorldBoids and DeadBOids Arrays
	for (auto* boid : WorldBoids)
	{
		boid->Destroy();
	}
	
	WorldBoids = BabyBoids;
	BabyBoids.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Evolution Controller: GENERATION Complete"));
}

ABoidAgent* AEvolutionController::spawnNewBoid()
{
	UWorld* World = GetWorld();

	if (BoidActor && World)
	{
	
		float XPos = FMath::FRandRange(MIN_WORLD_SPAWN, MAX_WORLD_SPAWN_X);
		float YPos = FMath::FRandRange(MIN_WORLD_SPAWN, MAX_WORLD_SPAWN_Y);
		float ZPos = 100;
		FVector RandPosition = FVector(XPos, YPos, ZPos);
		// The random location may overlap something, this will make them slightly adjust or just spawncompletely.
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* Spawned = World->SpawnActor(BoidActor, &RandPosition, &FRotator::ZeroRotator, SpawnParam);

		ABoidAgent* BoidCast = Cast<ABoidAgent>(Spawned);
		if (BoidCast)
		{
			BabyBoids.Add(BoidCast);
			BoidCast->Spawner = this;
			return BoidCast;
		}

	}

	return nullptr;
}
