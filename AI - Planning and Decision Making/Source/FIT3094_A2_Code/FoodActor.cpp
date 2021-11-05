// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodActor.h"

// Sets default values
AFoodActor::AFoodActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	VisibleComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetupAttachment(VisibleComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFoodActor::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AFoodActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

