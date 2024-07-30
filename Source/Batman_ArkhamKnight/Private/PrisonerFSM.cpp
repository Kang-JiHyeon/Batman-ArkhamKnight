// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerFSM.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Prisoner.h"
#include "PrisonerAnim.h"

// Sets default values for this component's properties
UPrisonerFSM::UPrisonerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPrisonerFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	Ptarget = Cast<APlayerCharacter>(actor);
	me = Cast<APrisoner>(GetOwner());

	// animation
	anim = Cast<UPrisonerAnim>(me->GetMesh()->GetAnimInstance());
	
}


// Called every frame
void UPrisonerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EPrisonerState::Idle:
		IdleState(DeltaTime);
		break;
	case EPrisonerState::Move:
		MoveState(DeltaTime);
		break;
	case EPrisonerState::RightAttack:
		RightAttackState(DeltaTime);
		break;
	case EPrisonerState::LeftAttack:
		LeftAttackState(DeltaTime);
		break;
	case EPrisonerState::Damage:
		DamageState(DeltaTime);
		break;
	case EPrisonerState::Die:
		DieState(DeltaTime);
		break;
	}
}

void UPrisonerFSM::IdleState(float& DeltaSeconds)
{

	currentTime += DeltaSeconds;
	if (currentTime > idleDelayTime)
	{

		mState = EPrisonerState::Move;
		currentTime = 0;
		anim->PanimState = mState;
	}
}

void UPrisonerFSM::MoveState(float& DeltaSeconds)
{
	// Ÿ���� ������
	FVector destination = Ptarget->GetActorLocation();
	// 
	FVector dir = destination - me->GetActorLocation();
	// ��� �� �÷��̾ ������ �ʰ� ����� �ް����� ġ�� �ʹ�.
	//if (FMath::RandBool())
	//{
		//�������� �̵�
		me->AddMovementInput(dir.GetSafeNormal(), 0.05f);
	//}
	//else
	//{
	//	// �ް����� ġ�� �ʹ�

	//	me->AddMovementInput(dir.GetSafeNormal(), 1.0f);

	//}
	


	// ���ݹ������� �÷��̾ ������ �����ϰ� �ʹ�. 
	float distance = dir.Size();
	if (distance < attackDistance)
	{
		// ������ ���ݰ� ���� ������ �����ϰ� ������ �ϰ� �ʹ�.
		if (FMath::RandBool())
		{
			mState = EPrisonerState::LeftAttack;
		}
		else
		{
			mState = EPrisonerState::RightAttack;
		}
	}

}

void UPrisonerFSM::RightAttackState(float& DeltaSeconds)
{
	// ���ݹ������� �÷��̾ ������ ���� 1�� �Ŀ� �����ϰ� �ʹ�.
	// ������ ��ġ�� �ϰ� �ʹ�.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		// ��ġ�� �ϰ� �� �� �ٽ� �̵����� �����ϰ� �ʹ�.
		float dist = me->GetDistanceTo(Ptarget);
		if (dist < attackDistance)
		{
			anim->PanimState = mState;
		}
		else
		{
			mState = EPrisonerState::Move;
			anim->PanimState = mState;
		}
	}
	

}

void UPrisonerFSM::LeftAttackState(float& DeltaSeconds)
{
	// ���ݹ������� �÷��̾ ������ ���� 1�� �Ŀ� �����ϰ� �ʹ�.
	// ���� ��ġ�� �ϰ� �ʹ�.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		// ��ġ�� �ϰ� �� �� �ٽ� �̵����� �����ϰ� �ʹ�.
		float dist = me->GetDistanceTo(Ptarget);
		if (dist < attackDistance)
		{
			anim->PanimState = mState;
		}
		else
		{
			mState = EPrisonerState::Move;
			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::DamageState(float& DeltaSeconds)
{

}

void UPrisonerFSM::DieState(float& DeltaSeconds)
{
	me->Destroy();
}

void UPrisonerFSM::OnMyTakeDamage(int32 damage)
{
	HP -= damage;
	if (HP > 0)
	{
		mState = EPrisonerState::Damage;
	}
	else
	{
		mState = EPrisonerState::Die;
	}
}
