// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "Kismet/GameplayStatics.h"
#include "splinePath.h"

// Sets default values
ACar::ACar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	if (timeline)
	{
		timeline->SetPlayRate(1 / TimeTocomplete);
		timeline->SetTimelineLength(1.00f);
		timeline->SetLooping(1);
		timeline->PlayFromStart();
	}
}

void ACar::MoveObject()
{
	timeline->Play();
}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();
	splinePath = UGameplayStatics::GetActorOfClass(GetWorld(), AsplinePath::StaticClass());

}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

