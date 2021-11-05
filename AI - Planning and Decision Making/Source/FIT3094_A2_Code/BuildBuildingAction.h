// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapAction.h"

class ABuildingActor;
/**
 * 
 */
class FIT3094_A2_CODE_API BuildBuildingAction : public GOAPAction
{
public:
	BuildBuildingAction();
	~BuildBuildingAction();

	bool IsActionDone() override;
	virtual bool CheckProceduralPrecondition(AGOAPActor* Agent);
	virtual bool PerformAction(AGOAPActor* Agent, float DeltaTime);
	virtual bool RequiresInRange();

private:

	virtual void Reset();

	TArray<ABuildingActor*> BuildingList;

	int32 WoodToPlace;
	int32 StoneToPlace;
};
