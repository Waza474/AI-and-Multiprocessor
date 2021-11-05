// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidAgent.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Predator.h"
#include "EvolutionController.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"


// Sets default values
ABoidAgent::ABoidAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Representation"));
	VisualComponent->SetupAttachment(RootComponent);
	//RootComponent = VisualComponent;

	//Get Sphere Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		VisualComponent->SetStaticMesh(SphereVisualAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Materials/Mat_Blue.Mat_Blue"));
	if (FoundMaterial.Succeeded())
	{
		DynamicMaterialInst = UMaterialInstanceDynamic::Create((UMaterial*)FoundMaterial.Object, VisualComponent);
		VisualComponent->SetMaterial(0, DynamicMaterialInst);
	}
	
	
	SetRandomTarget();

	// Genom Generation
	speed = FMath::RandRange(0.8 * 400, 400);
	comfortZoneRadius = FMath::RandRange(150, 250);
	predatorRunModifier = FMath::RandRange(0.5, 3);
	wallAvoidanceDegrees = FMath::RandRange(10, 90);


	// Set up personal Space
	PersonalSpace = CreateDefaultSubobject<USphereComponent>(TEXT("Personal Space"));
	PersonalSpace->InitSphereRadius(comfortZoneRadius);
	PersonalSpace->SetCollisionProfileName(TEXT("Personal Space"));
	//PersonalSpace->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PersonalSpace->SetupAttachment(VisualComponent);
	PersonalSpace->SetVisibility(true);
	//PersonalSpace->SetHiddenInGame(false);
	//PersonalSpace->SetupAttachment(RootComponent);
	



}

// Called when the game starts or when spawned
void ABoidAgent::BeginPlay()
{
	Super::BeginPlay();
	position = this->GetActorLocation();
}

// Called every frame
void ABoidAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Are there any boids in our personal space?
	TArray<ABoidAgent*> OverlappingBoids;
	TArray<APredator*> OverlappingPredators;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<AActor*> Killers;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
		this->GetActorLocation(),
		50.0f,
		ObjectTypes,
		ABoidAgent::StaticClass(),
		IgnoreActors,
		Killers
	);

	// Trace for wall Collisions
	FHitResult OutputHit;
	bool TraceResult;
	FVector TraceEnd = targetLocation - GetActorLocation();
	TraceEnd.Normalize();
	FVector TraceStart = TraceEnd * 100 + GetActorLocation();
	TraceEnd *= 400;
	TraceEnd += GetActorLocation();

	//TraceEnd.Z = 0;
	//TraceEnd = GetActorLocation() + TraceEnd;

	ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldStatic;
	FCollisionQueryParams QueryParam;
	FCollisionResponseParams ResponseParam;
	
	TraceResult = GetWorld()->LineTraceSingleByChannel(OutputHit,
		TraceStart,
		TraceEnd,
		TraceChannel,
		QueryParam,
		ResponseParam);
	
	AStaticMeshActor* WallCast = Cast<AStaticMeshActor>(OutputHit.GetActor());
	
	if (WallCast)
		TraceResult = true;
	else
		TraceResult = false;

	//UE_LOG(LogTemp, Warning, TEXT("HIT: %s"), *OutputHit.GetActor()->GetActorLabel());

	
	//DrawDebugLine(GetWorld(), GetActorLocation(), targetLocation, FColor::Orange, false, DeltaTime * 2);
	//DrawDebugLine(GetWorld(), GetActorLocation(), TraceEnd, FColor::Magenta, false, DeltaTime * 2);

	// Personal Space
	PersonalSpace->GetOverlappingActors(Overlappers, AActor::StaticClass());
	for (auto* boid : Overlappers)
	{
		ABoidAgent* ABoidCast = Cast<ABoidAgent>(boid);
		if (ABoidCast && ABoidCast != this)
		{
			OverlappingBoids.Add(ABoidCast);
		}
		else
		{
			APredator* PredCast = Cast<APredator>(boid);
			if (PredCast)
			{
				OverlappingPredators.Add(PredCast);
			}
		}
	}

	// Check if any on top of us, requiring death of boid
	if (Killers.Num() > 0)
	{
		APredator* PredCast = Cast<APredator>(Killers[0]);
		AStaticMeshActor* MeshCast = Cast<AStaticMeshActor>(Killers[0]);
		if (PredCast)
			CurrentStatus = Death_Status::Dead_Predator;
		else if (MeshCast)
			CurrentStatus = Death_Status::Dead_Wall;
		Spawner->boidDeath(this);
		return;
	}

	/*****************************************************************************/
	// Priority List
	// Predator Avoidance > Static Collison Avoidance > Flocking > Random Movement
	/*****************************************************************************/

	// Predator Avoidance
	/*****************************************************************************/
	if (OverlappingPredators.Num() > 0)
	{
		FVector RunVector = FVector(0, 0, 0);
		for (auto* predator : OverlappingPredators)
		{
			RunVector += this->GetActorLocation() - predator->GetActorLocation();

			// Check if maybe the bad one is too close to use?
			if (FVector::Dist(GetActorLocation(), predator->GetActorLocation()) <= 50.0f)
			{
				CurrentStatus = Death_Status::Dead_Predator;
				Spawner->boidDeath(this);
				return;
			}
		}
		// Still alive, continue
		RunVector.Z = 0;
		RunVector.Normalize();
		targetLocation = GetActorLocation() + RunVector * speed * DeltaTime * predatorRunModifier;
		SetActorLocation(targetLocation);
	}

	// Static Collision Avoidance
	/*****************************************************************************/
	
	else if (TraceResult)
	{
		//UE_LOG(LogTemp, Warning, TEXT("WALL"));
		
		bool circleResult = false;

		FVector circleTraceEnd = TraceEnd - GetActorLocation();
		circleTraceEnd.Normalize();
		circleTraceEnd *= 800;
		FVector Rotated = circleTraceEnd;

		float deg = wallAvoidanceDegrees;
		int iMax = (360 / deg) * 1.5;

		// Trace around in a circle Every X degrees 
		// Currently 30 degrees
		//while (!circleResult)
		for (int i = 0; i < iMax; i++)
		{
			// Trace
			circleResult = GetWorld()->LineTraceSingleByChannel(OutputHit,
				TraceStart,
				Rotated + GetActorLocation(),
				TraceChannel,
				QueryParam,
				ResponseParam);
			//DrawDebugLine(GetWorld(), TraceStart, Rotated + GetActorLocation(), FColor::Orange, false, 2*DeltaTime);

			// Rotate
			Rotated = circleTraceEnd.RotateAngleAxis(deg, FVector(0, 0, 1));

			if (deg > 0)
				deg *= -1;
			else
				deg = deg * -1 + wallAvoidanceDegrees;

			// Check if we need to continue or we found a spot
			if (!circleResult)
			{
				targetLocation = Rotated + GetActorLocation();
				break;
			}
			
		}
		if (circleResult) 
		{
			SetRandomTarget();
			//UE_LOG(LogTemp, Warning, TEXT("W -- True"));
		}
		else 
		{
			
			//UE_LOG(LogTemp, Warning, TEXT("W -- False"));
		}

		// Move to new direction
		SetActorLocation(FMath::VInterpConstantTo(position, targetLocation, DeltaTime, speed));

	}
	
	// Flocking
	/*****************************************************************************/
	else if (OverlappingBoids.Num() > 0)
	{
		// Avoid Collision With Other Boids
		FVector v1 = FVector(0, 0, 0);
		// Match Velocity of Other Boids
		FVector v2 = FVector(0, 0, 0);
		// Flock Centering
		FVector TotalLocations = FVector(0, 0, 0);
		FVector v3 = FVector(0, 0, 0);

		for (auto* boid : OverlappingBoids)
		{
			v1 += this->GetActorLocation() - boid->GetActorLocation();

			v2 += boid->GetActorLocation() - boid->targetLocation; 

			TotalLocations += boid->GetActorLocation();
		}

		v2 = v2 / OverlappingBoids.Num();
		v3 = TotalLocations / OverlappingBoids.Num();
		v3 = v3 - GetActorLocation();

		// Add 3 Vectors togethor
		FVector vFinal = v1 + v2 + v3;
		vFinal.Normalize();
		vFinal.Z = 0;
		targetLocation = GetActorLocation() + vFinal * speed * DeltaTime;
		
		SetActorLocation(targetLocation);
	}

	// Cant flock? Go to a random location
	/*****************************************************************************/
	else
	{
		SetActorLocation(FMath::VInterpConstantTo(position, targetLocation, DeltaTime, speed));
	}

	if (FVector::Dist(position, targetLocation) < 100.0f)
	{
		SetRandomTarget();
	}


	position = this->GetActorLocation();
	lifetime += DeltaTime;
}

void ABoidAgent::SetRandomTarget()
{
	targetLocation.X = FMath::FRandRange(0, 5500);
	targetLocation.Y = FMath::FRandRange(0, 10500);
	targetLocation.Z = 100;
}

float ABoidAgent::getFitness()
{
	switch (CurrentStatus)
	{
	case Death_Status::Alive:
		return lifetime;
		break;
	case Death_Status::Dead_Predator:
		return lifetime * 0.5;
		break;
	case Death_Status::Dead_Wall:
		return lifetime * 0.75;
		break;
	default:
		return 0;
	}
}

void ABoidAgent::Crossover(ABoidAgent* otherBOID)
{
	// Spawn a new boid
	ABoidAgent* Child = Spawner->spawnNewBoid();

	if (Child != nullptr)
	{
		// Set genomes between parents
		Child->speed = speed;
		Child->comfortZoneRadius = comfortZoneRadius;
		Child->predatorRunModifier = otherBOID->predatorRunModifier;
		Child->wallAvoidanceDegrees = otherBOID->wallAvoidanceDegrees;

		// Set Child to Mutate
		Child->Mutation();
	}
}

void ABoidAgent::Mutation()
{
	// Randomly Regenerate one of the 4 genomes
	int dice = FMath::RandRange(1, 4);
	switch (dice)
	{
	case 1:
		speed = FMath::RandRange(0.8 * 400, 400);
		break;
	case 2:
		comfortZoneRadius = FMath::RandRange(150, 250);
		break;
	case 3:
		predatorRunModifier = FMath::RandRange(0.5, 3);
		break;
	case 4:
		wallAvoidanceDegrees = FMath::RandRange(10, 90);
		break;
	}
}

/*
speed = FMath::RandRange(0.8 * 400, 400);
comfortZoneRadius = FMath::RandRange(150, 250);
predatorRunModifier = FMath::RandRange(0.5, 3);
wallAvoidanceDegrees = FMath::RandRange(10, 90);
*/