// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "BossFSM.h"
#include "EnemyPlayer.h"
#include "Prisoner.h"


// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	fsm = CreateDefaultSubobject<UBossFSM>(TEXT("FSM"));

	// �ִϸ��̼� �������Ʈ �Ҵ��ϱ�
	ConstructorHelpers::FClassFinder<UAnimInstance> animClass(TEXT("/Script/Engine.AnimBlueprint'/Game/SYH/Blueprints/ABP_SYHBoss.ABP_SYHBoss_C'"));
	if (animClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animClass.Class);
	}
}	

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super:: NotifyActorBeginOverlap(OtherActor);

	APrisoner* prisoner = Cast<APrisoner>(OtherActor);
	if (prisoner && fsm->mState == EBossState::FastMove)
	{
		FVector velo = prisoner->GetActorLocation() - GetActorLocation();
		velo.Normalize();
		velo *= FVector(1200.0f, 1200.0f, 0.0f); // �� �࿡ ����

		prisoner->LaunchCharacter(velo, true, false);
		UE_LOG(LogTemp, Warning, TEXT(" Boss NotifyActorBeginOverlap"));
	}
}


