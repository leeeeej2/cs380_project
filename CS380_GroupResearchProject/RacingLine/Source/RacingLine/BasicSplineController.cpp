// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicSplineController.h"
#include "Components/SplineComponent.h"

// Sets default values
ABasicSplineController::ABasicSplineController()
{
	PrimaryActorTick.bCanEverTick = true;

	TotalPathTimeController = 10.0f;
	bCanMoveActor = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	//Root->SetupAttachment(GetRootComponent());

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	//Spline->SetupAttachment(GetRootComponent());
	Spline->SetupAttachment(Root);
	Spline->Duration = TotalPathTimeController;
	Spline->bDrawDebug = true;
}

void ABasicSplineController::BeginPlay()
{
	Super::BeginPlay();

	FVector SpawnLocation = GetActorLocation();
	FActorSpawnParameters    SpawnParams;

	FRotator SpawnRotation;

	SpawnRotation.Pitch = 0.0f;

	SpawnRotation.Yaw = 0.0f;

	SpawnRotation.Roll = 0.0f;

	if (ActorToMoveClass != nullptr)
	{
		ActorToMove = GetWorld()->SpawnActor<AActor>(ActorToMoveClass, Spline->GetComponentTransform());
		
		if (ActorToMove != nullptr)
		{
			StartTime = GetWorld()->GetTimeSeconds();
			bCanMoveActor = true;

		}
	}
}

void ABasicSplineController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((ActorToMove != nullptr) && (bCanMoveActor))
	{
		float CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTimeController;


		float Distance = Spline->GetSplineLength() * CurrentSplineTime;
		
		FVector Position = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		ActorToMove->SetActorLocation(Position);

		FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
		ActorToMove->SetActorRotation(Rotator);

		if (CurrentSplineTime >= 1.0f)
		{
			if (bSplineInLoop)
			{
				bCanMoveActor = true;

				StartTime = GetWorld()->GetTimeSeconds();

				CurrentSplineTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTimeController;
			}
		}
	}

}

