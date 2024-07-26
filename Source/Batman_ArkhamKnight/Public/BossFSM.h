// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFSM.generated.h"

UENUM(BlueprintType)
enum class EBossState :uint8
{
	Idle,
	Move,
	Attack,
	DoubleAttack,
	//Damage,
	//Die,
	//SaveEnemy,
	//FastMove,

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EBossState mState = EBossState::Idle;

	// ������
	void IdleState();
	// �̵�����
	void MoveState();
	// ���ݻ���(��Ÿ)
	void AttackState();
	// ���ݻ���(��Ÿ)
	void DoubleAttackState();
	//// �ǰݻ���
	//void DamageState();
	//// ��������
	//void DieState();
	//// �˼��ݱ����
	//void SavePrisonerState();
	//// ���������
	//void FastMoveState();

	// ���ð�
	UPROPERTY(EditAnywhere,Category=FSM)
	float idleDelayTime = 2;
	// ����ð�(�߰��Ǿ� ���ð��� �ʰ��� �ð�)
	float currentTime = 0;

	// Ÿ�� �÷��̾�
	UPROPERTY(EditAnywhere,Category=FSM)
	class AEnemyPlayer* Ptarget;

	// ���� ��ġ ���
	UPROPERTY(EditAnywhere)
	class ABoss* me;
	
	UPROPERTY(EditAnywhere)
	float attackRange = 250.f;

	UPROPERTY(EditAnywhere)
	float attackDelayTime = 2.0f;
		
};
