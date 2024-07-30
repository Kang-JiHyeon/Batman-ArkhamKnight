// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpline.h"
#include "Components/SplineComponent.h"

// Sets default values
AEnemySpline::AEnemySpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	EnemySpline = CreateDefaultSubobject<USplineComponent>(TEXT("EnemySpline"));
	EnemySpline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemySpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

