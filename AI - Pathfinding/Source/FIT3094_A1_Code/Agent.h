// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

UCLASS()
class FIT3094_A1_CODE_API AAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAgent();

	int Health;
	float MoveSpeed;
	float Tolerance;

	TArray<GridNode*> Path;

	GridNode* SomeFood;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timed function that decreases health every 2 seconds;
	void DecreaseHealth();

	// Handle for Timer
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionSphere;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GeneratePath(GridNode* GoalNode, TArray<GridNode*> IgnoreNodes);
	int EstimateCost(GridNode* node1, GridNode* node2);
	void ConnectPath(GridNode* GoalNode);
	GridNode* ChooseFood();
	float distanceToPoint(FVector Point);

};
