// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Queue.h"
#include "CoreMinimal.h"

// Forward Declartion is done in the header file
// Again to avoid circular dependencies
class AGOAPActor;
class GOAPAction;

// We use a vasic struct called a GOAP Node when planning
// This functions fairly similar to our GridNode from previous weeks!
// These are used when building the search graph in the planner
struct GOAPNode
{
	// Where did we come from?
	GOAPNode* Parent;
	// The cost so far to reach this point
	float RunningCost;
	// The current state of the world when reaching this point
	// This will be the wrold state with all previous effect applied
	TMap<FString, bool> State;
	// A node is associated with an action. This is that action
	GOAPAction* Action;
};

/**
 * 
 */
class FIT3094_A2_CODE_API GOAPPlanner
{
public:
	GOAPPlanner();
	~GOAPPlanner();

	// The function called to generate the action plane
	// The agent calling it passes itself through along with its Available Actions
	// and Action Queue
	// NOTE: We are using c++ so these MUST be reference or pointers. Otherwise
	// it is pass by copy. This is not an issue with C# or other non-C languages
	static bool Plan(AGOAPActor* Agent, const TSet<GOAPAction*>& AvailableActions,
		TQueue<GOAPAction*>& PlannedActions, TMap<FString, bool> WorldState,
		TMap<FString, bool> GoalState);

protected:
	// This function builds out the node graph until it exhausts all possible paths
	// It uses a combination of A* and brute force to this as well. It could be optimized.
	// Calls itself recursively to build out nodes in the graph and find goals
	// Again this is not well optimized so should be used carefully
	static bool BuildGraphRecursive(TArray<GOAPNode*>& AllNodes, GOAPNode* Parent,
		TArray<GOAPNode*>& GoalNodes, const TSet<GOAPAction*>& AvailableActions,
		TMap<FString, bool>& GoalState);

	// Function used for creating a subset of actions with an action removed. This is used
	// to reduce available actions as more are checked off the list (aka dont call the same action twice)
	// Do note this could potentially cause issues later if you wanted to call the same action twice.
	static TSet<GOAPAction*> CreateActionSubset(const TSet<GOAPAction*>& AvailableActions,
		GOAPAction* RemoveAction);

	// Function compares a set of conditions to a state. Used to both determine if goals are met
	// and also if preconditions match existing world state
	static bool CheckConditionsInState(TMap<FString, bool>& Conditions,
		TMap<FString, bool>& State);

	// Combine a current state with changes (aka effects) of an actions
	static TMap<FString, bool> PopulateNewState(const TMap<FString, bool>& CurrentState,
		TMap<FString, bool>& Changes);
};
