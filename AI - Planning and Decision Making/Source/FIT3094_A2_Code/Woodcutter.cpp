// Fill out your copyright notice in the Description page of Project Settings.


#include "Woodcutter.h"
#include "EatFoodAction.h"
#include "GatherWoodAction.h"
#include "DepositResourceAction.h"

AWoodcutter::AWoodcutter()
{
	NumWood = 0;
	MoveSpeed = 80;
	Health = 50;
	maxHealth = 50;
	maxWood = 50;
	lowHealth = 15;
}

void AWoodcutter::BeginPlay()
{
	Super::BeginPlay();

	// Put my actions here later
	EatFoodAction* EatAction = new EatFoodAction();
	EatAction->AddPrecondition("NeedFood", true);
	EatAction->AddEffect("NeedFood", false);

	GatherWoodAction* GatherAction = new GatherWoodAction();
	GatherAction->AddPrecondition("HasRoom", true);
	GatherAction->AddEffect("HasRoom", false);
	GatherAction->AddEffect("HasWood", true);

	DepositResourceAction* DepositAction = new DepositResourceAction();
	DepositAction->AddPrecondition("DepositedWood", false);
	
	DepositAction->AddPrecondition("HasWood", true);
	//DepositAction->AddPrecondition("HasRoom", false);
	DepositAction->AddEffect("DepositedWood", true);
	
	AvailableActions.Add(EatAction);
	AvailableActions.Add(GatherAction);
	AvailableActions.Add(DepositAction);

}

void AWoodcutter::Tick(float DeltaTime)
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

TMap<FString, bool> AWoodcutter::GetWorldState()
{
	TMap<FString, bool> WorldState = Super::GetWorldState();

	WorldState.Add("NeedFood", Health < 15); // false
	WorldState.Add("HasRoom", NumWood < maxWood); // true
	WorldState.Add("HasWood", NumWood > 0); //false
	WorldState.Add("DepositedWood", false); // false

	return WorldState;
}

TMap<FString, bool> AWoodcutter::CreateGoalState()
{
	TMap<FString, bool> GoalState;

	GoalState.Add("NeedFood", false);
	//GoalState.Add("HasWood", true);
	GoalState.Add("DepositedWood", true);

	return GoalState;
}
