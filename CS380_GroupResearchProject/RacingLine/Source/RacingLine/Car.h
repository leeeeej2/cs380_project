// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Car.generated.h"

UCLASS()
class RACINGLINE_API ACar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACar();

	AActor* splinePath;		
	TSubclassOf<class AActor> WhatToSpawn;

	void MoveObject();
	float TimeTocomplete = 5.f;
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* timeline;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
