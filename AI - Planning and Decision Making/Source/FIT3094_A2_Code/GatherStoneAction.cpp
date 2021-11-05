// Fill out your copyright notice in the Description page of Project Settings.


#include "GatherStoneAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StoneActor.h"
#include "Stonemason.h"

GatherStoneAction::GatherStoneAction()
{
	Reset();
}

GatherStoneAction::~GatherStoneAction()
{
}

bool GatherStoneAction::IsActionDone()
{
	AStoneActor* Stone = Cast<AStoneActor>(Target);
	if (Stone && Stone->StoneResources == 0)
	{
		// Stop if the tree has run out of wood
		StoneList.Remove(Stone);
		Target->Destroy();
		return true;
	}
	// Also stop if we have gathered enough
	else if (StoneToGather == 0)
		return true;

	return false;
}

bool GatherStoneAction::CheckProceduralPrecondition(AGOAPActor* Agent)
{
	// Check a list of tres if we do not have one
	if (StoneList.Num() == 0)
	{
		TArray<AActor*> mOverlaps;
		TArray<AActor*> mIgnores;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(),
			5000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		for (auto actor : mOverlaps)
		{
			AStoneActor* tempCast = Cast<AStoneActor>(actor);
			if (tempCast)
			{
				StoneList.Add(tempCast);
			}
		}
	}

	// Find the nearest resources
	AStoneActor* NearestStone = nullptr;
	for (auto stone : StoneList)
	{
		if (NearestStone)
		{
			if (FVector::Dist(stone->GetActorLocation(), Agent->GetActorLocation()) < FVector::Dist(NearestStone->GetActorLocation(), Agent->GetActorLocation()))
			{
				// if its shorter we have a new clossest
				NearestStone = stone;
			}
		}
		else
		{
			NearestStone = stone;
		}
	}

	// Get Max Wood from Agent
	AStonemason* casted = Cast<AStonemason>(Agent);
	if (casted)
		StoneToGather = casted->maxStone - casted->NumStone;

	// If we have a food
	if (NearestStone)
	{
		Target = NearestStone;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("TREE: BAD"));
	// failed somewhere
	return false;
}

bool GatherStoneAction::PerformAction(AGOAPActor* Agent, float DeltaTime)
{
	AStoneActor* Stone = Cast<AStoneActor>(Target);
	AStonemason* Stonemason = Cast<AStonemason>(Agent);
	if (!Stone || !Stonemason)
		return false;

	RunningTime += DeltaTime;

	if (RunningTime >= Timer)
	{
		RunningTime = 0;
		// Get some wood
		Stonemason->NumStone += 1;
		Stone->StoneResources -= 1;

		StoneToGather -= 1;
	}

	return true;
}

bool GatherStoneAction::RequiresInRange()
{
	return true;
}

void GatherStoneAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	Timer = 3;
	RunningTime = 0;
	StoneToGather = 20;
}
