// Fill out your copyright notice in the Description page of Project Settings.


#include "splinePath.h"

// Sets default values
AsplinePath::AsplinePath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");

	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}
}

// Called when the game starts or when spawned
void AsplinePath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AsplinePath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

