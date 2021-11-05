// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"

class AVillageCentreActor;

/**
 * 
 */
class FIT3094_A2_CODE_API DepositResourceAction : public GOAPAction
{
public:
	DepositResourceAction();
	~DepositResourceAction();

	bool IsActionDone() override;
	virtual bool CheckProceduralPrecondition(AGOAPActor* Agent);
	virtual bool PerformAction(AGOAPActor* Agent, float DeltaTime);
	virtual bool RequiresInRange();

private:

	virtual void Reset();

	TArray<AVillageCentreActor*> VillageCentreList;
	AVillageCentreActor* VillageCentre;

	int32 WoodToDeposit;
	int32 StoneToDeposit;
	int32 MetalToDeposit;
};
