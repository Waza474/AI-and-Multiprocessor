// Fill out your copyright notice in the Description page of Project Settings.


#include "DepositResourceAction.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Woodcutter.h"
#include "Stonemason.h"
#include "VillageCentreActor.h"


DepositResourceAction::DepositResourceAction()
{
	Tag = TEXT("Deposit Action");
	Reset();
}

DepositResourceAction::~DepositResourceAction()
{
}

bool DepositResourceAction::IsActionDone()
{
	// Each xToDeposit is set to -1 to signify completeness
	if (WoodToDeposit == -1 || StoneToDeposit == -1 || MetalToDeposit == -1)
		return true;
	return false;
}

bool DepositResourceAction::CheckProceduralPrecondition(AGOAPActor* Agent)
{
	// Get the village centre if hasn't yet
	if(VillageCentreList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEPO: please"));
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
	//UE_LOG(LogTemp, Warning, TEXT("DEPO: VCcheck: %s"), *VillageCentre->GetActorLabel());

	// Set the amount of wood/stone to be deposited
	// Currently this doesn't actually have any affect as this will be called before actor gets any resource
	AWoodcutter* WoodcutterCast = Cast<AWoodcutter>(Agent);
	if (WoodcutterCast)
	{
		WoodToDeposit = WoodcutterCast->NumWood;
	}

	AStonemason* StonemasonCast = Cast <AStonemason>(Agent);
	if (StonemasonCast)
	{
		StoneToDeposit = StonemasonCast->NumStone;
	}

	// Ensure that a villageCentre is found and atleast 1 resource will be deposited
	// && (WoodToDeposit != 0 || StoneToDeposit != 0 || MetalToDeposit != 0)
	if (VillageCentreList.Num() != 0)
	{
		Target = VillageCentreList[0];
		UE_LOG(LogTemp, Warning, TEXT("DEPO: good"));
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("DEPO: BAD"));
	return false;
}

bool DepositResourceAction::PerformAction(AGOAPActor* Agent, float DeltaTime)
{
	AVillageCentreActor* Village = Cast<AVillageCentreActor>(Target);
	AWoodcutter* WoodcutterCast = Cast<AWoodcutter>(Agent);
	AStonemason* StonemasonCast = Cast <AStonemason>(Agent);
	if (!Village)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEPO: no village/target"));
		return false;
	}

	RunningTime += DeltaTime;
	// Deposit Instantly
	if (RunningTime >= Timer)
	{
		if (WoodcutterCast)
		{
			Village->WoodResources += WoodcutterCast->NumWood;
			WoodcutterCast->NumWood = 0;
			WoodToDeposit = -1;
		}
		else if (StonemasonCast)
		{
			Village->StoneResources += StonemasonCast->NumStone;
			StonemasonCast->NumStone = 0;
			StoneToDeposit = -1;
		}
		else
			return false;
	}

	return true;
}

bool DepositResourceAction::RequiresInRange()
{
	return true;
}

void DepositResourceAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	Timer = 1;
	RunningTime = 0;
	WoodToDeposit = 0;
	StoneToDeposit = 0;
	MetalToDeposit = 0;
	
}
