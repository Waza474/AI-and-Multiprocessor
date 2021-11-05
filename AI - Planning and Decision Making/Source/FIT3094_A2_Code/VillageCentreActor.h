// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "VillageCentreActor.generated.h"

UCLASS()
class FIT3094_A2_CODE_API AVillageCentreActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

public:
	UPROPERTY(EditAnywhere, Category="Resources")
		int StoneResources = 0;
	UPROPERTY(EditAnywhere, Category = "Resources")
		int WoodResources = 0;
	UPROPERTY(EditAnywhere, Category = "Resources")
		int MetalResources = 0;
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Tools = 0;
	
public:	
	// Sets default values for this actor's properties
	AVillageCentreActor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
