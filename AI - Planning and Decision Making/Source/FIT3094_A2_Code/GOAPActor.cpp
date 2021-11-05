// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPActor.h"
#include "GOAPPlanner.h"

// Sets default values
AGOAPActor::AGOAPActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionStateMachine = new StateMachine<Actor_States, AGOAPActor>(this, State_Nothing);
	ActionStateMachine->RegisterState(State_Idle, &AGOAPActor::OnIdleEnter,
		&AGOAPActor::OnIdleUpdate, &AGOAPActor::OnIdleExit);
	ActionStateMachine->RegisterState(State_Action, &AGOAPActor::OnActionEnter,
		&AGOAPActor::OnActionUpdate, &AGOAPActor::OnActionExit);
	ActionStateMachine->RegisterState(State_Move, &AGOAPActor::OnMoveEnter,
		&AGOAPActor::OnMoveUpdate, &AGOAPActor::OnMoveExit);
	ActionStateMachine->RegisterState(State_Stall, &AGOAPActor::OnStallEnter,
		&AGOAPActor::OnStallUpdate, &AGOAPActor::OnStallExit);
	ActionStateMachine->ChangeState(State_Idle);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetupAttachment(RootComponent);

	// Collision Sphere Setup
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionSphere->InitSphereRadius(50.0f);
	CollisionSphere->SetRelativeLocation(FVector(50,0,0));
	CollisionSphere->SetCollisionProfileName(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGOAPActor::CollisionBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AGOAPActor::CollisionEndOverlap);
}

// Called when the game starts or when spawned
void AGOAPActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGOAPActor::DecreaseHealth, 2.0f, true, 2.0f);
	
}

// Called every frame
void AGOAPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActionStateMachine->Tick(DeltaTime);

	
}

void AGOAPActor::OnIdleEnter()
{
}

void AGOAPActor::OnIdleUpdate(float DeltaTime)
{
	// In this sate we are planning our next move
	// Get world state
	TMap<FString, bool> WorldState = GetWorldState();
	// Get Desired Goal Sate
	TMap<FString, bool> GoalState = CreateGoalState();

	// Attempt to make a plan and check success
	if (GOAPPlanner::Plan(this, AvailableActions, CurrentActions,
		WorldState, GoalState))
	{
		UE_LOG(LogTemp, Warning, TEXT("GOAP ACTOR CLASS: Plan has been found"));
		ActionStateMachine->ChangeState(State_Action);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GOAP ACTOR CLASS: No Plan Found!!!"));
	}
}

void AGOAPActor::OnIdleExit()
{
}


void AGOAPActor::OnMoveEnter()
{
	// Entering into move state check to ensure we can move
	// If no actions return to idle immediately
	if (CurrentActions.IsEmpty())
	{
		ActionStateMachine->ChangeState(State_Idle);
		return;
	}

	// If current action requires an inrange check and the target is null. Return to planning
	GOAPAction* CurrentAction = *CurrentActions.Peek();
	UE_LOG(LogTemp, Warning, TEXT("!!!!! CA: %s"), *CurrentAction->Target->GetActorLabel());
	if (CurrentAction->RequiresInRange() && CurrentAction->Target == nullptr)
	{
		ActionStateMachine->ChangeState(State_Idle);
	}
}

void AGOAPActor::OnMoveUpdate(float DeltaTime)
{
	GOAPAction* CurrentAction = *CurrentActions.Peek();

	// This is a direct movement example
	// For assignments you ill need to use your pathfinding and grid based movement here :)
	////UE_LOG(LogTemp, Warning, TEXT("!!!!! CA: %s"), *CurrentAction->Tag);
	FVector Direction = CurrentAction->Target->GetActorLocation() - this->GetActorLocation();
	Direction.Normalize();

	// Update position based of direction and move speed
	FVector NewPos = GetActorLocation() + Direction * MoveSpeed * DeltaTime;

	// If we are close enough to target then snap to it
	if (FVector::Dist(NewPos, CurrentAction->Target->GetActorLocation()) <= Tolerance)
	{
		NewPos = CurrentAction->Target->GetActorLocation();
		// We are now in range of target so set it to true
		CurrentAction->SetInRange(true);
		// Moving is domne transition to action state
		ActionStateMachine->ChangeState(State_Action);
	}
	SetActorLocation(NewPos);

	// Set Rotation
	FVector Forward = FVector(1, 0, 0);
	float Dot = FVector::DotProduct(Forward, Direction);
	float Det = Forward.X * Direction.Y + Forward.Y * Direction.X; // Determinate
	float RotRadians = FMath::Atan2(Det, Dot);
	float RotDegrees = FMath::RadiansToDegrees(RotRadians);

	FRotator Rotator = FRotator(0, RotDegrees, 0);
	FQuat RotationQauternion = FQuat(Rotator);
	SetActorRotation(RotationQauternion);
}

void AGOAPActor::OnMoveExit()
{
}

void AGOAPActor::OnActionEnter()
{
}

void AGOAPActor::OnActionUpdate(float DeltaTime)
{
	// If we have no staes change to idle and exit immediately
	if (CurrentActions.IsEmpty())
	{
		ActionStateMachine->ChangeState(State_Idle);
		return;
	}

	// Check to see if our current action is finished
	GOAPAction* CurrentAction = *CurrentActions.Peek();
	if (CurrentAction->IsActionDone())
	{
		// WE are done with this action begone!
		CurrentActions.Dequeue(CurrentAction);
	}

	// If at this point we still have more actions continue
	if (!CurrentActions.IsEmpty())
	{
		// Get to the top of the queue
		// It is technically a pointer pointer which is why we dereference one layer :S
		CurrentAction = *CurrentActions.Peek();

		// Check to see if we need to be within rang efor an action
		// If no range requiremneent is needed return true
		// If a range requirement does exist then check to se if we are in range
		// Ternary operators are nasty but they do have their place!
		bool InRange = CurrentAction->RequiresInRange() ? CurrentAction->IsInRange() : true;

		// If we are in range attempt the action
		if (InRange)
		{
			// Attempt to perform action
			// Action can fail which is why we store the result
			bool IsActionSuccessfull = CurrentAction->PerformAction(this, DeltaTime);
			////UE_LOG(LogTemp, Warning, TEXT("GOAP ACTOR CLASS: Action Performed"));

			// If we fail change to the idle state and report that we had to abort the plan
			if (!IsActionSuccessfull)
			{
				ActionStateMachine->ChangeState(State_Idle);
				OnPlanAborted(CurrentAction);
			}
		}
		else
		{
			// at this point we have a valid action but are not in range. Commence movement
			ActionStateMachine->ChangeState(State_Move);
		}
	}
	else
	{
		// No ACtions remaining move to idle state
		ActionStateMachine->ChangeState(State_Idle);
	}
}

void AGOAPActor::OnActionExit()
{
}

void AGOAPActor::OnStallEnter()
{
}

void AGOAPActor::OnStallUpdate(float DeltaTime)
{
	// Check to see if another Actor is stalled
	// if 2 actors are stalling each other
	for (auto actor : stalledOverlaps)
	{
		if (actor->ActionStateMachine->GetCurrentState() == State_Stall)
		{
			// Check which has the shortest distance to que
			GOAPAction* act1 = *CurrentActions.Peek();
			FVector pos1 = act1->Target->GetActorLocation();
			float dis1 = FVector::Dist(GetActorLocation(), pos1);

			GOAPAction* act2 = *actor->CurrentActions.Peek();
			FVector pos2 = act2->Target->GetActorLocation();
			float dis2 = FVector::Dist(actor->GetActorLocation(), pos2);

			// Larger Distance will get right of way
			if (dis1 > dis2)
			{
				stalledOverlaps.Remove(actor);
				// Check if there are no other actors stalling us
				if (stalledOverlaps.Num() == 0)
					ActionStateMachine->ChangeState(previousState);
			}
		}
	}
}

void AGOAPActor::OnStallExit()
{
}

void AGOAPActor::CollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor: OVERLAP START"));
	// Collision Avoidance
	AGOAPActor* OtherCast = Cast<AGOAPActor>(OtherActor);
	if (OtherCast && ActionStateMachine->GetCurrentState() != State_Idle)
	{
		previousState = ActionStateMachine->GetCurrentState();
		stalledOverlaps.Add(OtherCast);
		ActionStateMachine->ChangeState(State_Stall);
	}
}

void AGOAPActor::CollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor: OVERLAP END"));
	// Collision Avodiance
	AGOAPActor* OtherCast = Cast<AGOAPActor>(OtherActor);
	if (OtherCast)
	{
		stalledOverlaps.Remove(OtherCast);
		
		// Check if there are no other actors stalling us
		if (stalledOverlaps.Num() == 0)
			ActionStateMachine->ChangeState(previousState);
	}
}

TMap<FString, bool> AGOAPActor::GetWorldState()
{
	TMap<FString, bool> CurrentWorldState;
	return CurrentWorldState;
}

TMap<FString, bool> AGOAPActor::CreateGoalState()
{
	TMap<FString, bool> GoalState;
	return GoalState;
}

void AGOAPActor::OnPlanFailed(TMap<FString, bool> FailedGoalState)
{
}

void AGOAPActor::OnPlanAborted(GOAPAction* FailedAction)
{
}

void AGOAPActor::DecreaseHealth()
{
	Health--;

	if (Health <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Destroy();
	}
}

void AGOAPActor::NeedFoodNewPlan()
{
	if (healthy)
	{
		CurrentActions.Empty();
		ActionStateMachine->ChangeState(State_Idle);
		healthy = !healthy;
	}
}
