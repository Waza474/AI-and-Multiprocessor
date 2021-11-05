// Fill out your copyright notice in the Description page of Project Settings.


#include "PredatorSpawner.h"

#include "Engine/World.h"

// Sets default values
APredatorSpawner::APredatorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APredatorSpawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if(PredatorActor)
	{
		for(int i = 0; i < PREDATOR_COUNT; i++)
		{
			float XPos = FMath::FRandRange(MIN_WORLD_SPAWN, MAX_WORLD_SPAWN_X);
			float YPos = FMath::FRandRange(MIN_WORLD_SPAWN, MAX_WORLD_SPAWN_Y);
			float ZPos = 100;
			FVector RandPosition = FVector(XPos, YPos, ZPos);
			World->SpawnActor(PredatorActor, &RandPosition, &FRotator::ZeroRotator);
		}
	}
}

// Called every frame
void APredatorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

