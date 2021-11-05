// Fill out your copyright notice in the Description page of Project Settings.


#include "WithdrawResourceAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VillageCentreActor.h"
#include "Builder.h"

WithdrawResourceAction::WithdrawResourceAction()
{
	Reset();
}

WithdrawResourceAction::~WithdrawResourceAction()
{
}

bool WithdrawResourceAction::IsActionDone()
{
	// Each xToDeposit is set to -1 to signify completeness
	if (WoodToCollect == -1 || StoneToCollect == -1)
		return true;
	return false;
}

bool WithdrawResourceAction::CheckProceduralPrecondition(AGOAPActor* Agent)
{
	// Check if we need to find a VillageCentre
	if (VillageCentreList.Num()==0)
	{

		TArray<AActor*> mOverlaps;
		TArray<AActor*> mIgnores;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(),
			5000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		for (auto actor : mOverlaps)
		{
			AVillageCentreActor* villageCast = Cast<AVillageCentreActor>(actor);
			if (villageCast)
			{
				VillageCentreList.Add(villageCast);
			}
		}
	}

	// Check if the village has any resources
	


	// Get the Amount of resources to collect
	ABuilder* BuilderCast = Cast<ABuilder>(Agent);
	if (BuilderCast)
	{
		StoneToCollect = BuilderCast->maxStone - BuilderCast->NumStone;
		WoodToCollect = BuilderCast->maxWood - BuilderCast->NumWood;
	}

	// Check if we have found
	if (VillageCentreList.Num() >= 1)
	{
		Target = VillageCentreList[0];

		AVillageCentreActor* VillageCast = Cast<AVillageCentreActor>(Target);
		if (VillageCast)
		{
			if (VillageCast->WoodResources == 0 && VillageCast->StoneResources == 0)
				return false;
		}

		return true;
	}

	// Failed somewhere
	return false;
}

bool WithdrawResourceAction::PerformAction(AGOAPActor* Agent, float DeltaTime)
{
	// Cast Agent and Target
	ABuilder* BuilderCast = Cast<ABuilder>(Agent);
	AVillageCentreActor* VillageCast = Cast<AVillageCentreActor>(Target);
	if (!BuilderCast || !VillageCast)
		return false;

	RunningTime += DeltaTime;
	// After 1 second grab all items
	if (RunningTime >= Timer)
	{
		// Dont grab more than there is
		WoodToCollect = FMath::Min(WoodToCollect, VillageCast->WoodResources);
		StoneToCollect = FMath::Min(StoneToCollect, VillageCast->StoneResources);

		VillageCast->WoodResources -= WoodToCollect;
		VillageCast->StoneResources -= StoneToCollect;

		BuilderCast->NumWood += WoodToCollect;
		BuilderCast->NumStone += StoneToCollect;

		WoodToCollect = -1;
		StoneToCollect = -1;
	}
	return true;
}

bool WithdrawResourceAction::RequiresInRange()
{
	return true;
}

void WithdrawResourceAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	Timer = 1;
	RunningTime = 0;
	StoneToCollect = 5;
	WoodToCollect = 10;
}
