// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class AGOAPActor;

/**
 * 
 */
class FIT3094_A2_CODE_API GOAPAction
{
public:
	GOAPAction();
	~GOAPAction();

	FString Tag;

	// List of condistions required for this action to be valid
	TMap<FString, bool> Preconditions;
	// The changes this action has on the world
	TMap<FString, bool> Effects;

	// The cost of the action
	// All actions have a cost associated with them
	// You can change this at run time if needed!
	float ActionCost;

	// Often Actions will work on a target
	// We store a generic Actor as the target here
	// Sub classes can cast this as needed.
	AActor* Target;

	// Reset the action to its base state
	void DoReset();

	// Adding and removing preconditions
	void AddPrecondition(FString Name, bool State);
	void RemovePrecondition(FString Name);

	// Adding and removing effects
	void AddEffect(FString Name, bool State);
	void RemoveEffect(FString Name);

	// Set & Check whether or not the Action is within change
	bool IsInRange() const { return InRange; }
	void SetInRange(const bool Range) { InRange = Range; }

	// Pure Virtual functions that child classes MUST implement
	// Whether or not an action has finished executing
	virtual bool IsActionDone() = 0;
	// Check procedural preconditions at run time. Provided an agent
	virtual bool CheckProceduralPrecondition(AGOAPActor* Agent) = 0;
	// Actually perform the actions
	// Returns true unless the action can no longer be completed
	// This is called each frame until the action is done
	virtual bool PerformAction(AGOAPActor* Agent, float DeltaTime) = 0;
	// Whether or not the action actually requires something in range
	// Some actions can be executed anywhere. Others need specific spots
	virtual bool RequiresInRange() = 0;

private:
	bool InRange;

	// Do Reset above handles calling this.
	// Sub-Classes can handle their own addition reset requirements here
	virtual void Reset() = 0;

protected:
	float RunningTime;
	// How often to do the action
	float Timer;

};
