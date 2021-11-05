// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"

class ATreeActor;

/**
 * 
 */
class FIT3094_A2_CODE_API GatherWoodAction : public GOAPAction
{
public:
	GatherWoodAction();
	~GatherWoodAction();

	bool IsActionDone() override;
	virtual bool CheckProceduralPrecondition(AGOAPActor* Agent);
	virtual bool PerformAction(AGOAPActor* Agent, float DeltaTime);
	virtual bool RequiresInRange();

private:

	virtual void Reset();

	TArray<ATreeActor*> TreeList;

	int32 WoodToGather;
};
