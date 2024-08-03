// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
//#include "EnemyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Boss.h"
#include "BossAnim.h"
#include "PlayerCharacter.h"
#include "EnemyPlayer.h"
#include "Prisoner.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	Ptarget = Cast<APlayerCharacter>(actor);
	me = Cast<ABoss>(GetOwner());

	// animation
	anim = Cast<UBossAnim>(me->GetMesh()->GetAnimInstance());

	// fast move
	me->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &UBossFSM::OnMeshBeginOverlap);

	// hp	
	HP = MaxHP;
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EBossState::Idle:
		IdleState();
		break;
	case EBossState::Move:
		MoveState();
		break;
	case EBossState::RightAttack:
		RightAttackState();
		break;
	case EBossState::LeftAttack:
		LeftAttackState();
		break;
	case EBossState::DoubleRightAttack:
		DoubleRightAttackState();
		break;
	case EBossState::DoubleLeftAttack:
		DoubleLeftAttackState();
		break;
	case EBossState::Damage:
		DamageState();
		break;
	case EBossState::Die:
		DieState();
		break;
	case EBossState::FastMove:
		FastMoveState();
		break;
	}
	
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan,logMsg);
}

void UBossFSM::IdleState()
{
	
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{

		mState = EBossState::Move;
		currentTime = 0;
		anim->BanimState = mState;
	}
}

void UBossFSM::MoveState() // boss move to player or idle
{
	//Ÿ���� ������
	FVector destination = Ptarget->GetActorLocation();
	//����
	FVector dir = destination - me->GetActorLocation();
	//�������� �̵�
	me->AddMovementInput(dir.GetSafeNormal(), 0.1f);
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > moveDelayTime)
	{
		int32 statevalue = FMath::RandRange(0, 9);
		if (statevalue == 0)
		{
			mState = EBossState::Move;

		}
		else if (statevalue > 0 && statevalue <3)
		{
			direction = Ptarget->GetActorLocation() - me->GetActorLocation();
			mState = EBossState::FastMove;
		}
		else {

			if (dir.Size() < attackRange)
			{
				int32 attackstatevalue = FMath::RandRange(0, 4);
				if (attackstatevalue < 1)
				{
					if (FMath::RandBool()) {
						mState = EBossState::DoubleRightAttack;
						anim->BanimState = mState;
					}
					else {
						mState = EBossState::DoubleLeftAttack;
						anim->BanimState = mState;
					}
				}
				else if (attackstatevalue == 1 || attackstatevalue == 2)
				{
					anim->bAttackPlay = true;
					mState = EBossState::LeftAttack;
					anim->BanimState = mState;
					currentTime = attackDelayTime;
				}
				else
				{
					anim->bAttackPlay = true;
					mState = EBossState::RightAttack;
					anim->BanimState = mState;
					currentTime = attackDelayTime;
				}
			}

		}
		currentTime = 0;
		anim->BanimState = mState;
	}


	// prisoner�� player���� ������ ���������� �� �ݴ� ���µ� �ʿ�

}

void UBossFSM::RightAttackState() // smash
{
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	currentTime += GetWorld()->DeltaTimeSeconds;
	anim->bAttackPlay = true;

	if (currentTime > attackDelayTime)
	{
		mState = EBossState::Move;
		anim->BanimState = mState;
		currentTime = 0;
	}
}


void UBossFSM::LeftAttackState() // smash
{
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	currentTime += GetWorld()->DeltaTimeSeconds;
	anim->bAttackPlay = true;
	if (currentTime > attackDelayTime)
	{
		mState = EBossState::Move;
		anim->BanimState = mState;
		currentTime = 0;
	}
	
}

void UBossFSM::DoubleRightAttackState() // double smash
{
	UE_LOG(LogTemp, Warning, TEXT("DoubleRightAttack"));
	currentTime += GetWorld()->DeltaTimeSeconds;

	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	// double attack 2 type
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		mState = EBossState::Move;
		anim->BanimState = mState;
	}
}

void UBossFSM::DoubleLeftAttackState() // double smash
{
	UE_LOG(LogTemp, Warning, TEXT("DoubleRightAttack"));
	currentTime += GetWorld()->DeltaTimeSeconds;

	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	// double attack 2 type
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		mState = EBossState::Move;
		anim->BanimState = mState;
	}
}

void UBossFSM::DamageState()
{
	FVector dir = me->GetActorLocation() - Ptarget->GetActorLocation();
	float dis = dir.Size();
	dir.Normalize();


	if (dis < 120) {
		me->GetCharacterMovement()->Velocity = dir * 2000;
	}

	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > damageDelayTime)
	{
		if (HP < 0)
		{
			mState = EBossState::Die;
			anim->BanimState = mState;
		}
		else
		{
			mState = EBossState::Move;
			anim->BanimState = mState;
		}
		currentTime = 0;
	}
}

void UBossFSM::DieState()
{
	me->Destroy();
}



void UBossFSM::FastMoveState()
{
// player first position remember and go to there
// if player is there in boss root -> damage

	

	me->AddMovementInput(direction.GetSafeNormal(), 1.0f);
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	currentTime += GetWorld()->GetDeltaSeconds();

	if (currentTime >= fastDelayTime)
	{
		mState = EBossState::Move;
		anim->BanimState = mState;
		currentTime = 0;
	}
}

void UBossFSM::OnMyTakeDamage(int32 damage)

{
	if (mState == EBossState::Die)
	{
		return;
	}
	HP -= damage;
	if (HP > 0)
	{
		mState = EBossState::Damage;
		anim->BanimState = mState;
	}
	else
	{
		mState = EBossState::Die;
		anim->BanimState = mState;
	}

}

void UBossFSM::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// fastmove���¿��� mesh�� overlap�Ǹ� player�� �а� �ٽ� move���·� ���ƿ´�.
	auto* player = Cast<APlayerCharacter>(OtherActor);
	if (mState == EBossState::FastMove)
	{

		if (player != nullptr)
		{
			// player�� �и��� �Լ� �߰�
			mState = EBossState::Move;
			anim->BanimState = mState;
		}
	}
}

