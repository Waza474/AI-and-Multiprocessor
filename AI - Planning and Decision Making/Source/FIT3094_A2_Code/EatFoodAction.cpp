// Fill out your copyright notice in the Description page of Project Settings.


#include "EatFoodAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FoodActor.h"
#include "GOAPActor.h"

EatFoodAction::EatFoodAction()
{
	Reset();
}

EatFoodAction::~EatFoodAction()
{
}

bool EatFoodAction::IsActionDone()
{
	if (RunningTime >= Timer)
	{
		return true;
	}
	return false;
}

bool EatFoodAction::CheckProceduralPrecondition(AGOAPActor* Agent)
{
	// Check a list of food if we do not have one
	if (FoodList.Num() == 0)
	{
		TArray<AActor*> mOverlaps;
		TArray<AActor*> mIgnores;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(),
			5000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		for (auto actor : mOverlaps)
		{
			AFoodActor* tempCast = Cast<AFoodActor>(actor);
			if (tempCast)
			{
				FoodList.Add(tempCast);
			}
		}
	}

	// Find the nearest resources
	AFoodActor* NearestFood = nullptr;
	for (auto food : FoodList)
	{
		if (NearestFood)
		{
			if (FVector::Dist(food->GetActorLocation(), Agent->GetActorLocation()) < FVector::Dist(NearestFood->GetActorLocation(), Agent->GetActorLocation()))
			{
				// if its shorter we have a new clossest
				NearestFood = food;
			}
		}
		else
		{
			NearestFood = food;
		}
	}

	// If we have a food
	if (NearestFood)
	{
		Target = NearestFood;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("FOOD: BAD"));
	// failed somewhere
	return false;
}

bool EatFoodAction::PerformAction(AGOAPActor* Agent, float DeltaTime)
{
	RunningTime += DeltaTime;

	if (RunningTime >= Timer)
	{
		Agent->Health = Agent->maxHealth;

		AFoodActor* temp = Cast<AFoodActor>(Target);
		FoodList.Remove(temp);
		Target->Destroy();
		Target = nullptr;
	}
	
	return true;
}

bool EatFoodAction::RequiresInRange()
{
	return true;
}

void EatFoodAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	Timer = 2;
	RunningTime = 0;
}
