// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageCentreActor.h"

// Sets default values
AVillageCentreActor::AVillageCentreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVillageCentreActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVillageCentreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

