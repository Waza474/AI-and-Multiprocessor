// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BoidAgent.generated.h"

class AEvolutionController;

UCLASS()
class FIT3094_A3_BASE_API ABoidAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidAgent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float speed;
	FVector targetLocation;
	FVector position;
	float comfortZoneRadius;
	float predatorRunModifier;
	float wallAvoidanceDegrees;

	UStaticMeshComponent* VisualComponent;
	UMaterialInstanceDynamic* DynamicMaterialInst;

	void SetRandomTarget();

	UPROPERTY(EditAnywhere)
		USphereComponent* PersonalSpace;

	TArray<AActor*> Overlappers;

	float lifetime = 0.f;

	enum Death_Status
	{
		Alive,
		Dead,
		Dead_Predator,
		Dead_Wall
	};
	Death_Status CurrentStatus = Death_Status::Alive;

	float getFitness();

	AEvolutionController* Spawner;

	void Crossover(ABoidAgent* otherBOID);
	void Mutation();
};
