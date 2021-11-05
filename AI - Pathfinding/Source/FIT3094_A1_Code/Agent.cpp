// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent.h"

#include "LevelGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Food.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 50;
	MoveSpeed = 100;
	Tolerance = 20;

	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	//Scene->SetupAttachment(RootComponent);
	RootComponent = Scene;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->InitSphereRadius(75.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(Scene);
	CollisionSphere->bHiddenInGame = true;
	CollisionSphere->SetRelativeLocation(FVector(49.999962, 50.000053, 0));
	//CollisionSphere->SetWorldLocation(FVector(49.999962, 50.000053, 0));
}

// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();

	// Set a timer for every two seconds and call the decrease health function
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAgent::DecreaseHealth, 2.0f, true, 2.0f);

	
}

void AAgent::DecreaseHealth()
{
	// Decrease health by one and if 0 then destroy object
	Health--;

	if(Health <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Destroy();
	}
}

// Called every frame
void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check Collisions and prevent them
	TArray<AActor*> OverlappingActors;
	CollisionSphere->GetOverlappingActors(OverlappingActors, AAgent::StaticClass());
	if (OverlappingActors.Num() != 0 && Path.Num() != 0)
	{
		bool smallestPath = false;
		bool recalculateRequired = false;
		TArray<GridNode*> IgnoreNodes;
		for (auto* overlap : OverlappingActors)
		{

			AAgent* temp = Cast<AAgent>(overlap);
			if (temp->Path.Num() > Path.Num())
			{
				smallestPath = true;
			}
			if (temp->Path.Num() > 1)
			{
				if (temp->Path[0] == Path[0])
				{
					recalculateRequired = true;
					IgnoreNodes.Add(temp->Path[0]);
					IgnoreNodes.Add(temp->Path[1]);
				}
			}
		}
		if (smallestPath && recalculateRequired) // If you have the smallest path you wait
		{
			Path.Empty();
			GeneratePath(SomeFood, IgnoreNodes);
		}
	}
	
	
	if(Path.Num() > 0)
	{
		GridNode* TargetNode = Path[0]; // Save a pointer to the node

		FVector CurrentPosition = GetActorLocation();
			
		float TargetXPos = Path[0]->X * ALevelGenerator::GRID_SIZE_WORLD;
		float TargetYPos = Path[0]->Y * ALevelGenerator::GRID_SIZE_WORLD;

		FVector TargetPosition(TargetXPos, TargetYPos, CurrentPosition.Z);

		FVector Direction = TargetPosition - CurrentPosition;
		Direction.Normalize();

		CurrentPosition += Direction * MoveSpeed * DeltaTime;
		SetActorLocation(CurrentPosition);

		if(FVector::Dist(CurrentPosition, TargetPosition) <= Tolerance)
		{
			CurrentPosition = TargetPosition;
			Path.RemoveAt(0);
		}

		// Eat the Health Item
		if (Path.Num() == 0 && TargetNode->ObjectAtLocation != nullptr)
		{
			TargetNode->ObjectAtLocation->Destroy();
			Health = 50;
		}
		

	}
	// If not current path: Find a path to a food
	else if (Path.Num() == 0) {
		SomeFood = ChooseFood();
		TArray<GridNode*> EmptyIgnoreNodes;
		if (SomeFood != nullptr) { GeneratePath(SomeFood, EmptyIgnoreNodes); }
	}
}

void AAgent::GeneratePath(GridNode* GoalNode, TArray<GridNode*> IgnoreNodes)
{
	// Ignore Nodes is a array of nodes that the algorithm will ignore, for example, collision handling
	FString found = TEXT("NO ROUTE");
	// Open and Closed Lists
	TArray<GridNode*> OpenList;
	TArray<GridNode*> ClosedList;
	GridNode* smallestFNode;
	GridNode* CurrentNode;

	// Get a pointer to the level Generator
	UWorld* world = GetWorld();
	ALevelGenerator* levelGenerator = Cast<ALevelGenerator>(UGameplayStatics::GetActorOfClass(world, ALevelGenerator::StaticClass()));

	int GridX = GetActorLocation().X / ALevelGenerator::GRID_SIZE_WORLD;
	int GridY = GetActorLocation().Y / ALevelGenerator::GRID_SIZE_WORLD;
	GridNode* StartNode = levelGenerator->WorldArray[GridX][GridY];

	StartNode->G = 0;
	StartNode->H = EstimateCost(StartNode, GoalNode);
	StartNode->F = StartNode->G + StartNode->H;

	OpenList.Add(StartNode);

	while (OpenList.Num() != 0)
	{
		// Take Out the node with the smallest F
		smallestFNode = OpenList[0];
		for (GridNode* node : OpenList)
		{
			if (node->F < smallestFNode->F)
			{
				smallestFNode = node;
				found = TEXT("ROUTE FOUND");
			}
		}
		CurrentNode = smallestFNode;
		OpenList.Remove(CurrentNode);
		ClosedList.Add(CurrentNode);

		// Found the Goal
		if (CurrentNode == GoalNode)
		{
			ConnectPath(GoalNode);
			break;
		}
		// Make sure we dont walk over any health bars on the way that arent the goal node
		else if (CurrentNode != StartNode && CurrentNode->ObjectAtLocation)
		{
			continue;
		}

		// Get Neighbour nodes
		TArray<GridNode*> neighbours;
		if (CurrentNode->X != 0) { neighbours.Add(levelGenerator->WorldArray[CurrentNode->X - 1][CurrentNode->Y]); }
		if (CurrentNode->Y + 1 < ALevelGenerator::GRID_SIZE_WORLD) { neighbours.Add(levelGenerator->WorldArray[CurrentNode->X][CurrentNode->Y + 1]); }
		if (CurrentNode->X + 1 < ALevelGenerator::GRID_SIZE_WORLD) { neighbours.Add(levelGenerator->WorldArray[CurrentNode->X + 1][CurrentNode->Y]); }
		if (CurrentNode->Y != 0) { neighbours.Add(levelGenerator->WorldArray[CurrentNode->X][CurrentNode->Y - 1]); }

		// Iterate Over Neighbours
		for (GridNode* nextNode : neighbours)
		{
			UE_LOG(LogTemp, Warning, TEXT("- Check Neighbour"));
			// Ignore nodes in the closed ist and only go to empty nodees
			// Do not use any nodes that should be ignored
			// Also make sure not to walk over any food on the way
			if (!ClosedList.Contains(nextNode)
				&& !IgnoreNodes.Contains(nextNode)
				&& (nextNode->GridType == GridNode::Open 
					|| nextNode->GridType == GridNode::Swamp) ) 
			{

				float possibleG = CurrentNode->G + nextNode->GetTravelCost();
				bool possibleGBetter = false;

				// Is it in our list of nodes to check?
				if (!OpenList.Contains(nextNode))
				{
					OpenList.Add(nextNode);
					nextNode->H = EstimateCost(nextNode, GoalNode);
					possibleGBetter = true;
					UE_LOG(LogTemp, Warning, TEXT("-- Add Neighbour"));
				}
				// It already is, but it has the least cost to travel
				else if (possibleG < nextNode->G)
				{
					possibleGBetter = true;
				}
				// Nodes values need to be calculated
				if (possibleGBetter)
				{
					nextNode->Parent = CurrentNode;
					nextNode->G = possibleG;
					nextNode->F = nextNode->G + nextNode->H;
				}
			}

		}
	}
	// Reset Any Values for other Agents
	for (GridNode* node : OpenList)
	{
		node->Parent = nullptr;
		node->F = 0;
		node->G = 0;
		node->H = 0;
	}
	for (GridNode* node : ClosedList)
	{
		node->Parent = nullptr;
		node->F = 0;
		node->G = 0;
		node->H = 0;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, found);
	if (found == TEXT("ROUTE FOUND") && Path.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Que?"));
		SomeFood = ChooseFood();

	}
	else if (found == TEXT("NO ROUTE") ){
		SomeFood = ChooseFood();
	}
}

int AAgent::EstimateCost(GridNode* node1, GridNode* node2)
{
	return FMath::Abs(node1->X - node2->X) + FMath::Abs(node1->Y - node2->Y);
}

void AAgent::ConnectPath(GridNode* GoalNode)
{
	// Add the generated path to the Path array
	TArray<GridNode*> VisitedList;
	GridNode* CurrentNode = GoalNode;
	while (CurrentNode != nullptr)
	{
		VisitedList.Add(CurrentNode);
		CurrentNode = CurrentNode->Parent;
	}
	int i = VisitedList.Num() - 1;
	while (i >= 0)
	{
		Path.Add(VisitedList[i]);
		i--;
	}
}

GridNode* AAgent::ChooseFood()
{
	// Get an array of Food
	TArray<AActor*> foodArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AFood::StaticClass(), foodArray);
	
	if (foodArray.Num() == 0) { return nullptr; }

	// Select a food that isnt far but not tooo far
	AActor* bestFood;
	bestFood = foodArray[0];
	float bestDistance = 0;
	float maxDistance = Health * 100;

	for (auto* checkFood : foodArray)
	{
		float checkDistance = distanceToPoint(checkFood->GetActorLocation());
		AFood* tempF = Cast<AFood>(checkFood);
		if (checkDistance > bestDistance && checkDistance < maxDistance && !tempF->IsEaten)
		{
			bestFood = checkFood;
			bestDistance = checkDistance;
		}
	}
	AFood* selectedFood = Cast<AFood>(bestFood);
	selectedFood->IsEaten = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), distanceToPoint(selectedFood->GetActorLocation())));
	//AFood* selectedFood = Cast<AFood>(foodArray[0]);
	//AActor* selectedFood = foodArray[0];

	// Get a pointer to the level Generator
	ALevelGenerator* levelGenerator = Cast<ALevelGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGenerator::StaticClass()));
	
	// Get the grid node a food is on
	int GridX = selectedFood->GetActorLocation().X / ALevelGenerator::GRID_SIZE_WORLD;
	int GridY = selectedFood->GetActorLocation().Y / ALevelGenerator::GRID_SIZE_WORLD;
	GridNode* GridPoint = levelGenerator->WorldArray[GridX][GridY];
	return GridPoint;
}

float AAgent::distanceToPoint(FVector Point)
{
	FVector curLocation = this->GetActorLocation();
	FVector DistanceVector(curLocation.X - Point.X, curLocation.Y - Point.Y, 0);
	return DistanceVector.Size();
}
