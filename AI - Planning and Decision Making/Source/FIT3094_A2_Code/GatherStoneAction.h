// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"

class AStoneActor;

/**
 * 
 */
class FIT3094_A2_CODE_API GatherStoneAction : public GOAPAction
{
public:
	GatherStoneAction();
	~GatherStoneAction();

	bool IsActionDone() override;
	virtual bool CheckProceduralPrecondition(AGOAPActor* Agent);
	virtual bool PerformAction(AGOAPActor* Agent, float DeltaTime);
	virtual bool RequiresInRange();

private:

	virtual void Reset();

	TArray<AStoneActor*> StoneList;

	int32 StoneToGather;
};
