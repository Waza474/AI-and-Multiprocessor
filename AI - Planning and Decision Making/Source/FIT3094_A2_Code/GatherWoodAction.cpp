// Fill out your copyright notice in the Description page of Project Settings.


#include "GatherWoodAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TreeActor.h"
#include "GOAPActor.h"
#include "Woodcutter.h"

GatherWoodAction::GatherWoodAction()
{
	Tag = TEXT("Gather Wood Action");
	Reset();
}

GatherWoodAction::~GatherWoodAction()
{
}

bool GatherWoodAction::IsActionDone()
{
	ATreeActor* Tree = Cast<ATreeActor>(Target);
	if (Tree && Tree->WoodResources == 0)
	{
		// Stop if the tree has run out of wood
		TreeList.Remove(Tree);
		Target->Destroy();
		return true;
	}
	// Also stop if we have gathered enough
	else if (WoodToGather == 0)
		return true;


	return false;
}

bool GatherWoodAction::CheckProceduralPrecondition(AGOAPActor* Agent)
{
	// Check a list of tres if we do not have one
	if (TreeList.Num() == 0)
	{
		TArray<AActor*> mOverlaps;
		TArray<AActor*> mIgnores;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(),
			5000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		for (auto actor : mOverlaps)
		{
			ATreeActor* tempCast = Cast<ATreeActor>(actor);
			if (tempCast)
			{
				TreeList.Add(tempCast);
			}
		}
	}

	// Find the nearest resources
	ATreeActor* NearestTree = nullptr;
	for (auto tree : TreeList)
	{
		if (NearestTree)
		{
			if (FVector::Dist(tree->GetActorLocation(), Agent->GetActorLocation()) < FVector::Dist(NearestTree->GetActorLocation(), Agent->GetActorLocation()))
			{
				// if its shorter we have a new clossest
				NearestTree = tree;
			}
		}
		else
		{
			NearestTree = tree;
		}
	}
	// Get Max Wood from Agent
	AWoodcutter* casted = Cast<AWoodcutter>(Agent);
	if (casted)
		WoodToGather = casted->maxWood - casted->NumWood;

	// If we have a food
	if (NearestTree)
	{
		Target = NearestTree;
		return true;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("TREE: BAD"));
	// failed somewhere
	return false;

}

bool GatherWoodAction::PerformAction(AGOAPActor* Agent, float DeltaTime)
{
	ATreeActor* Tree = Cast<ATreeActor>(Target);
	AWoodcutter* WoodCutter = Cast<AWoodcutter>(Agent);
	if (!Tree || !WoodCutter)
		return false;

	RunningTime += DeltaTime;

	if (RunningTime >= Timer)
	{
		RunningTime = 0;
		// Get some wood
		WoodCutter->NumWood += 1;
		Tree->WoodResources -= 1;
		WoodToGather -= 1;
	}
	
	return true;
}

bool GatherWoodAction::RequiresInRange()
{
	return true;
}

void GatherWoodAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	Timer = 1;
	RunningTime = 0;
	WoodToGather = 50;
}
