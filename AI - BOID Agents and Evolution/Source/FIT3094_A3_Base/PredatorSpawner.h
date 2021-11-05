// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PredatorSpawner.generated.h"

UCLASS()
class FIT3094_A3_BASE_API APredatorSpawner : public AActor
{
	GENERATED_BODY()
	
public:

	// World Size Min and Max
	static const int MIN_WORLD_SPAWN = 0;
	static const int MAX_WORLD_SPAWN_X = 5500;
	static const int MAX_WORLD_SPAWN_Y = 10500;

	static const int PREDATOR_COUNT = 7;

	// Actors for spawning into the world
	UPROPERTY(EditAnywhere, Category = "Entities")
		TSubclassOf<AActor> PredatorActor;
	
	// Sets default values for this actor's properties
	APredatorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
