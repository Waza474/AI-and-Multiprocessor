// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidAgent.h"
#include "EvolutionController.generated.h"

UCLASS()
class FIT3094_A3_BASE_API AEvolutionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEvolutionController();

	static const int MIN_WORLD_SPAWN = 0;
	static const int MAX_WORLD_SPAWN_X = 5500;
	static const int MAX_WORLD_SPAWN_Y = 10500;

	UPROPERTY(EditAnywhere)
	//static const 
		int BOID_COUNT = 100;

	// Boids to be spawned
	UPROPERTY(EditAnywhere, Category = "Entities")
		TSubclassOf<ABoidAgent> BoidActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<ABoidAgent*> WorldBoids;
	TArray<ABoidAgent*> DeadBoids;
	TArray<ABoidAgent*> BabyBoids;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void boidDeath(ABoidAgent* killedBoid);

	void childGeneration();

	ABoidAgent* spawnNewBoid();
};
