// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildBuildingAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BuildingActor.h"
#include "Builder.h"

BuildBuildingAction::BuildBuildingAction()
{
}

BuildBuildingAction::~BuildBuildingAction()
{
}

bool BuildBuildingAction::IsActionDone()
{
	if (WoodToPlace == -1 && StoneToPlace == -1)
		return true;
	return false;
}

bool BuildBuildingAction::CheckProceduralPrecondition(AGOAPActor* Agent)
{
	// Get a list of buildings if needed
	if (BuildingList.Num() == 0)
	{
		TArray<AActor*> mOverlaps;
		TArray<AActor*> mIgnores;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(),
			5000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		for (auto actor : mOverlaps)
		{
			ABuildingActor* tempCast = Cast<ABuildingActor>(actor);
			if (tempCast)
			{
				BuildingList.Add(tempCast);
			}
		}
	}

	// Get the Closest One
	ABuildingActor* ClosestBuilding = nullptr;
	for (auto building : BuildingList)
	{
		if (ClosestBuilding)
		{
			if (FVector::Dist(building->GetActorLocation(), Agent->GetActorLocation()) < FVector::Dist(ClosestBuilding->GetActorLocation(), Agent->GetActorLocation()))
			{
				// if its shorter we have a new clossest
				ClosestBuilding = building;
			}
		}
		else
		{
			ClosestBuilding = building;
		}
	}

	// Check if a building has been found
	if (ClosestBuilding)
	{
		Target = ClosestBuilding;
		return true;
	}

	// Something bad happened somewhere
	return false;
}

bool BuildBuildingAction::PerformAction(AGOAPActor* Agent, float DeltaTime)
{
	ABuilder* builder = Cast<ABuilder>(Agent);
	ABuildingActor* building = Cast<ABuildingActor>(Target);
	if (!builder || !building)
		return false;

	RunningTime += 1;
	// Deposit/Place resources after 1 second
	if (RunningTime >= Timer)
	{
		RunningTime = 0;
		
		// Check and place wood
		if (builder->NumWood > 0)
		{
			building->WoodResources += 1;
			builder->NumWood -= 1;
		}
		else
		{
			WoodToPlace = -1;
		}
		// Check and place stone
		if (builder->NumStone > 0)
		{
			building->StoneResources += 1;
			builder->NumStone -= 1;
		}
		else
		{
			StoneToPlace = -1;
		}
	}

	return true;
}

bool BuildBuildingAction::RequiresInRange()
{
	return true;
}

void BuildBuildingAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	Timer = 1;
	RunningTime = 0;
	StoneToPlace = 5;
	WoodToPlace = 10;
}
