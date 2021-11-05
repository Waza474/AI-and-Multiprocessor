// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StateMachine.h"
#include "GOAPAction.h"
#include "Components/SphereComponent.h"
#include "GOAPActor.generated.h"

UCLASS()
class FIT3094_A2_CODE_API AGOAPActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGOAPActor();

	// State Machine Valid Sates
	// Technically nothin is not valid only ussed for initialization
	enum Actor_States
	{
		State_Nothing,
		State_Idle,
		State_Move,
		State_Action,
		State_Stall
	};

	// Variable for handling movement
	const float Tolerance = 5;
	float MoveSpeed;
	UPROPERTY(EditAnywhere)
		float Health;
	float maxHealth;
	float lowHealth = -1;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;
	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionSphere;

	Actor_States previousState;
	TArray<AGOAPActor*> stalledOverlaps;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The Dreaded State Machine!
	StateMachine<Actor_States, AGOAPActor>* ActionStateMachine;

	// A list of all available actions this Agent can do
	TSet<GOAPAction*> AvailableActions;
	// The set of actions that the agent is currently planning to use
	TQueue<GOAPAction*> CurrentActions;

	// State Machine Functions
	void OnIdleEnter();
	void OnIdleUpdate(float DeltaTime);
	void OnIdleExit();
	// State Machine Functions
	void OnMoveEnter();
	void OnMoveUpdate(float DeltaTime);
	void OnMoveExit();
	// State Machine Functions
	void OnActionEnter();
	void OnActionUpdate(float DeltaTime);
	void OnActionExit();
	// State Machine Stall Functions
	void OnStallEnter();
	void OnStallUpdate(float DeltaTime);
	void OnStallExit();
	UFUNCTION()
	void CollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor*
		OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool
		bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CollisionEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Function for getting the current world state. This is virtual as
	// it probably should be overidden by child agents as they will have
	// different information
	virtual TMap<FString, bool> GetWorldState();
	// Create a goal state we want to acheive. We could have multiple and pick one
	virtual TMap<FString, bool> CreateGoalState();
	// A plan was unable to be found. Handle this behaviour (Probably just idle)
	virtual void OnPlanFailed(TMap<FString, bool> FailedGoalState);
	// A plan was aborted mid way. The provided action failed to execute
	// Handle this behaviour in some way
	virtual void OnPlanAborted(GOAPAction* FailedAction);

	FTimerHandle TimerHandle;
	void DecreaseHealth();
	bool healthy = true;
	void NeedFoodNewPlan();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
