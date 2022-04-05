// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicSplineController.generated.h"

UCLASS()
class RACINGLINE_API ABasicSplineController : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
		class USplineComponent* Spline;

	UPROPERTY(EditDefaultsOnly, Category = "SplineController")
		TSubclassOf<class AActor> ActorToMoveClass;

	AActor* ActorToMove;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		float TotalPathTimeController;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		bool bSplineInLoop;

	bool bCanMoveActor;

	float StartTime;

public:
	// Sets default values for this actor's properties
	ABasicSplineController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
