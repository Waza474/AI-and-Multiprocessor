// Fill out your copyright notice in the Description page of Project Settings.


#include "Predator.h"



#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "BoidAgent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APredator::APredator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	AsteroidVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Representation"));
	AsteroidVisual->SetupAttachment(RootComponent);

	//Get Sphere Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		AsteroidVisual->SetStaticMesh(SphereVisualAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Materials/Mat_Red.Mat_Red"));
	if (FoundMaterial.Succeeded())
	{
		DynamicMaterialInst = UMaterialInstanceDynamic::Create((UMaterial*)FoundMaterial.Object, AsteroidVisual);
		AsteroidVisual->SetMaterial(0, DynamicMaterialInst);
	}

	//Set Speed, Position and Target
	speed = 400;
	
	SetRandomTarget();

	target = nullptr;
	boidCheckInterval = 10.0f;
}

// Called when the game starts or when spawned
void APredator::BeginPlay()
{
	Super::BeginPlay();
	position = this->GetActorLocation();
}

// Called every frame
void APredator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (counter <= 0 || target == nullptr)
	{
		counter = boidCheckInterval;

		// Sphere Overlap Actors
		TArray<AActor*> OverlappingBoids;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);

		TArray<AActor*> Killers;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
			this->GetActorLocation(),
			400.0f,
			ObjectTypes,
			ABoidAgent::StaticClass(),
			IgnoreActors,
			OverlappingBoids);

		ABoidAgent* ClosestBoid = nullptr;
		float minDistance = 0.f;
		// Get Closest One
		for (auto* actor : OverlappingBoids)
		{
			ABoidAgent* boid = Cast<ABoidAgent>(actor);
			if (boid)
			{
				if (minDistance == 0.f)
					ClosestBoid = boid;
				else
				{
					if (FVector::Dist(position, boid->GetActorLocation()) < FVector::Dist(position, ClosestBoid->GetActorLocation()))
					{
						ClosestBoid = boid;
					}
				}
			}
		}

		target = ClosestBoid;
	}
	counter -= DeltaTime;

	if (target != nullptr)
	{
		SetActorLocation(FMath::VInterpConstantTo(position, target->GetActorLocation(), DeltaTime, speed));

		if (FVector::Dist(position, target->GetActorLocation()) < 40.0f)
		{
			counter = 0;
			target = nullptr;
		}
	}

	else
	{
		SetActorLocation(FMath::VInterpConstantTo(position, targetLocation, DeltaTime, speed));
		if (FVector::Dist(position, targetLocation) < 100.0f)
		{
			SetRandomTarget();
		}
	}

	position = this->GetActorLocation();

}

void APredator::SetRandomTarget()
{
	targetLocation.X = FMath::FRandRange(0, 5500);
	targetLocation.Y = FMath::FRandRange(0, 10500);
}
