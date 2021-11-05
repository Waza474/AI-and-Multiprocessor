// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder.h"
#include "EatFoodAction.h"
#include "WithdrawResourceAction.h"
#include "BuildBuildingAction.h"

ABuilder::ABuilder()
{
	NumStone = 0;
	NumWood = 0;
	maxStone = 5;
	maxWood = 10;
	Health = 400;
	maxHealth = 400;
	MoveSpeed = 90;
}

void ABuilder::BeginPlay()
{
	Super::BeginPlay();

	// Put my actions here later
	EatFoodAction* EatAction = new EatFoodAction();
	EatAction->AddPrecondition("NeedFood", true);
	EatAction->AddEffect("NeedFood", false);

	WithdrawResourceAction* WithdrawAction = new WithdrawResourceAction();
	WithdrawAction->AddPrecondition("HasRoom", true);
	WithdrawAction->AddEffect("HasRoom", false);
	WithdrawAction->AddEffect("HasResources", true);

	BuildBuildingAction* BuildAction = new BuildBuildingAction();
	BuildAction->AddPrecondition("HasBuilt", false);
	BuildAction->AddPrecondition("HasResources", true);
	BuildAction->AddEffect("HasBuilt", true);


	AvailableActions.Add(EatAction);
	AvailableActions.Add(WithdrawAction);
	AvailableActions.Add(BuildAction);

}

void ABuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health < 15)
	{
		Super::NeedFoodNewPlan();
	}
	else
	{
		healthy = true;
	}
}

TMap<FString, bool> ABuilder::GetWorldState()
{
	TMap<FString, bool> WorldState = Super::GetWorldState();

	WorldState.Add("NeedFood", Health < 15);
	WorldState.Add("HasRoom", NumStone < maxStone || NumWood < maxWood);
	WorldState.Add("HasResources", NumStone > 0 || NumWood > 0);
	WorldState.Add("HasBuilt", false);

	return WorldState;
}

TMap<FString, bool> ABuilder::CreateGoalState()
{
	TMap<FString, bool> GoalState;

	GoalState.Add("NeedFood", false);
	GoalState.Add("HasBuilt", true);

	return GoalState;
}
