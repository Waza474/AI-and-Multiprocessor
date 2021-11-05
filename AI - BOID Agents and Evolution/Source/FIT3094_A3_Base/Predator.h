// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Predator.generated.h"

class ABoidAgent;

UCLASS()
class FIT3094_A3_BASE_API APredator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float speed;
	FVector targetLocation;
	FVector position;

	UStaticMeshComponent* AsteroidVisual;

	UMaterialInstanceDynamic* DynamicMaterialInst;

	void SetRandomTarget();

	float boidCheckInterval;
	float counter = 0.0f;

	ABoidAgent* target;

};
