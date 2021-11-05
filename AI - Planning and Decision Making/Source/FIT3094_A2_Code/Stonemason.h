// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPActor.h"
#include "Stonemason.generated.h"

/**
 * 
 */
UCLASS()
class FIT3094_A2_CODE_API AStonemason : public AGOAPActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AStonemason();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function for getting the currentl world state. This is vurtual as
	// it probably be overidden by child agents as they will have different information
	TMap<FString, bool> GetWorldState() override;
	// Create a goal state we want to acheive. We could have multiple and pick one
	TMap<FString, bool> CreateGoalState() override;

	UPROPERTY(EditAnywhere)
		int NumStone;
	int maxStone;
	
};
