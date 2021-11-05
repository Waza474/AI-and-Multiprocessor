// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreeActor.generated.h"

UCLASS()
class FIT3094_A2_CODE_API ATreeActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;
public:
	UPROPERTY(EditAnywhere, Category = "Resources")
		int WoodResources = 100;
	
public:	
	// Sets default values for this actor's properties
	ATreeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
