// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerFSM.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Prisoner.h"
#include "PrisonerAnim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	// HP
	HP = MaxHp;
	
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
	case EPrisonerState::Run:
		RunState(DeltaTime);
		break;
	case EPrisonerState::BackMove:
		BackMoveState(DeltaTime);
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
	case EPrisonerState::Faint:
		FaintState(DeltaTime);
		break;
	case EPrisonerState::Die:
		DieState(DeltaTime);
		break;
	}
}

void UPrisonerFSM::SetState(EPrisonerState NextState)
{
	EPrisonerState prevState = mState;
	mState = NextState;
	currentTime = 0;
	switch (mState)
	{
	case EPrisonerState::Idle:
		break;
	case EPrisonerState::Move:
		break;
	case EPrisonerState::Run:
		break;
	case EPrisonerState::BackMove:
		break;
	case EPrisonerState::RightAttack:
		break;
	case EPrisonerState::LeftAttack:
		break;
	case EPrisonerState::Damage:
		break;
	case EPrisonerState::Faint:
		break;
	case EPrisonerState::Die:
		break;
	default:
		break;
	}
}

void UPrisonerFSM::IdleState(float& DeltaSeconds)
{

	currentTime += DeltaSeconds;
	if (currentTime > idleDelayTime)
	{

		if (FMath::RandBool())
		{
			SetState(EPrisonerState::Move);
			anim->PanimState = mState;
		}
		else
		{
			SetState(EPrisonerState::BackMove);
			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::MoveState(float& DeltaSeconds){

	// Ÿ���� ������
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	// �÷��̾� �������� �̵�
	me->AddMovementInput(dir.GetSafeNormal(), 0.05f);

	currentTime += DeltaSeconds;
	// ���ݹ������� �÷��̾ ������ �����ϰ� �ʹ�. 
	if (currentTime > moveDelayTime)
	{
		int32 value = FMath::RandRange(0, 5);
		if (value<5)
		{
			SetState(EPrisonerState::Move);
			anim->PanimState = mState;
		}
		else
		{
			SetState(EPrisonerState::Run);
			anim->PanimState = mState;
		}
	}

}

void UPrisonerFSM::RunState(float& DeltaSeconds)
{
	// �÷��̾ ���� �޸��� �ʹ�.
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	float dist = me->GetDistanceTo(Ptarget);
	me->AddMovementInput(dir.GetSafeNormal(), 0.7f);

	currentTime += DeltaSeconds;
	if (currentTime > 1.3) {
		if (dist < 100)
		{
			// ������ ���ݰ� ���� ������ �����ϰ� ������ �ϰ� �ʹ�.

			if (FMath::RandBool()) SetState(EPrisonerState::LeftAttack);
			else SetState(EPrisonerState::RightAttack);
			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::BackMoveState(float& DeltaSeconds)
{
	// Ÿ���� ������
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	// �ް����� ġ�� �ʹ� = �÷��̾ �ٶ󺸸� �ڷ� �Ȱ� �ʹ�.
	// �������� ȸ�� (���� ���)
	FRotator currentRotation = me->GetActorRotation();
	FRotator targetRotation = dir.Rotation();
	FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaSeconds, 10.0f); // 10.0f�� ȸ�� �ӵ�, �ʿ信 ���� ���� ����
	me->SetActorRotation(newRotation);

	// �÷��̾��� �ݴ� �������� �̵�
	FVector backwardDir = -dir.GetSafeNormal();
	me->AddMovementInput(backwardDir,0.1f);
	currentTime += DeltaSeconds;
	if (currentTime > backmoveDelayTime)
	{
		SetState(EPrisonerState::Move);
		anim->PanimState = mState;
	}

	float distance = dir.Size();
}

void UPrisonerFSM::RightAttackState(float& DeltaSeconds)
{
	// ���ݹ������� �÷��̾ ������ ���� 1�� �Ŀ� �����ϰ� �ʹ�.
	// ������ ��ġ�� �ϰ� �ʹ�.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{
		// ��ġ�� �ϰ� �� �� �ٽ� �̵����� �����ϰ� �ʹ�.
		float dist = me->GetDistanceTo(Ptarget);
		if (dist < attackDistance)
		{
			anim->PanimState = mState;
		}
		else
		{
			if (FMath::RandBool())
			{
				SetState(EPrisonerState::Move);
			}
			else
			{
				SetState(EPrisonerState::BackMove);
			}
			
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
		// ��ġ�� �ϰ� �� �� �ٽ� �̵����� �����ϰ� �ʹ�.
		float dist = me->GetDistanceTo(Ptarget);
		if (dist < attackDistance)
		{
			anim->PanimState = mState;
		}
		else
		{
			if (FMath::RandBool())
			{
				SetState(EPrisonerState::Move);
			}
			else
			{
				SetState(EPrisonerState::BackMove);
			}

			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::DamageState(float& DeltaSeconds)
{

	FVector dir = me->GetActorLocation() - Ptarget->GetActorLocation();
	float dis = dir.Size();
	dir.Normalize();


	if (dis < 150) {
		me->GetCharacterMovement()->Velocity = dir * 1500;
	}

	currentTime += DeltaSeconds;
	if (currentTime > damageDelayTime)
	{
		if (HP/MaxHp < 0.5f && HP/MaxHp >0)
		{
			SetState(EPrisonerState::Faint);
			anim->PanimState = mState;
		}
		else if (HP / MaxHp == 0)
		{
			SetState(EPrisonerState::Die);
			anim->PanimState = mState;
		}
		else
		{
			SetState(EPrisonerState::Move);
			anim->PanimState = mState;
		}
	}

}

void UPrisonerFSM::FaintState(float& DeltaSeconds)
{

	currentTime += DeltaSeconds;
	if (currentTime > FaintDelayTime)
	{
		SetState(EPrisonerState::Move);
		anim->PanimState = mState;
	}
}


void UPrisonerFSM::DieState(float& DeltaSeconds)
{
	
}

void UPrisonerFSM::OnMyTakeDamage(int32 damage)

// �������� �Դٰ� ���� HP ���ϰ� �Ǹ� �������¿� ���� �ʹ�.
// �������¿� ���� ���� ���� �ð����Ŀ� �ٽ� �̵� ���·� �����ϰ� �ʹ�.
{
	HP -= damage;
	if (mState == EPrisonerState::Die)
	{
	
	}
	else {
		SetState(EPrisonerState::Damage);
		anim->PanimState = mState;
	}
}
