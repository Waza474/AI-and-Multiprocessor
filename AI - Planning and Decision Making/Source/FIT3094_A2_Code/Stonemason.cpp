// Fill out your copyright notice in the Description page of Project Settings.


#include "Stonemason.h"
#include "EatFoodAction.h"
#include "GatherStoneAction.h"
#include "DepositResourceAction.h"

AStonemason::AStonemason()
{
	NumStone = 0;
	MoveSpeed = 80;
	Health = 100;
	maxHealth = 100;
	maxStone = 20;
}

void AStonemason::BeginPlay()
{
	Super::BeginPlay();

	// Put my actions here later
	EatFoodAction* EatAction = new EatFoodAction();
	EatAction->AddPrecondition("NeedFood", true);
	EatAction->AddEffect("NeedFood", false);

	GatherStoneAction* GatherAction = new GatherStoneAction();
	GatherAction->AddPrecondition("HasRoom", true);
	GatherAction->AddEffect("HasRoom", false);
	GatherAction->AddEffect("HasStone", true);

	DepositResourceAction* DepositAction = new DepositResourceAction();
	DepositAction->AddPrecondition("DepositeStone", false);
	DepositAction->AddPrecondition("HasStone", true);
	DepositAction->AddEffect("DepositeStone", true);

	AvailableActions.Add(EatAction);
	AvailableActions.Add(GatherAction);
	AvailableActions.Add(DepositAction);

}

void AStonemason::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health < 20)
	{
		Super::NeedFoodNewPlan();
	}
	else
	{
		healthy = true;
	}
}

TMap<FString, bool> AStonemason::GetWorldState()
{
	TMap<FString, bool> WorldState = Super::GetWorldState();

	WorldState.Add("NeedFood", Health < 20); // false
	WorldState.Add("HasRoom", NumStone < maxStone); // true
	WorldState.Add("HasWood", NumStone > 0); //false
	WorldState.Add("DepositeStone", false); // false

	return WorldState;
}

TMap<FString, bool> AStonemason::CreateGoalState()
{
	TMap<FString, bool> GoalState;

	GoalState.Add("NeedFood", false);
	GoalState.Add("DepositeStone", true);

	return GoalState;
}
