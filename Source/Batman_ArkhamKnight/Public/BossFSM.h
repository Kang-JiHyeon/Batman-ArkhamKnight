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
	RightAttack,
	LeftAttack,
	DoubleRightAttack,
	DoubleLeftAttack,
	Damage,
	Die,
	SavePrisoner,
	FastMove,

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BFSM)
	EBossState mState = EBossState::Idle;

	// ������
	void IdleState();
	// �̵�����
	void MoveState();
	// ���ݻ���(��Ÿ)
	void RightAttackState();
	// ���ݻ���(��Ÿ)
	void LeftAttackState();
	// ���ݻ���(��Ÿ)
	void DoubleRightAttackState();
	// ���ݻ���(��Ÿ)
	void DoubleLeftAttackState();
	// �ǰݻ���
	void DamageState();
	// ��������
	void DieState();
	// �˼��ݱ����
	void SavePrisonerState();
	// ���� ����
	void FastMoveState();

	// ���ð�
	UPROPERTY(EditAnywhere,Category= BFSM)
	float idleDelayTime = 2;
	// ����ð�(�߰��Ǿ� ���ð��� �ʰ��� �ð�)
	float currentTime = 0;

	// Ÿ�� �÷��̾�
	UPROPERTY(EditAnywhere,Category= BFSM)
	class APlayerCharacter* Ptarget;

	// Ÿ�� ��ġ
	UPROPERTY(EditAnywhere)
	FVector direction;

	// ���� ��ġ ���
	UPROPERTY(EditAnywhere)
	class ABoss* me;
	
	UPROPERTY(EditAnywhere)
	float attackRange = 250.f;

	UPROPERTY(EditAnywhere)
	float attackDelayTime = 1.5f;

	// move or idle
	UPROPERTY(EditAnywhere,Category= BFSM)
	float moveDelayTime = 2;

	// fastmove
	UPROPERTY(EditAnywhere,Category= BFSM)
	float fastDelayTime =2;

	UPROPERTY(EditAnywhere,Category= BFSM)
	float fastRange = 100.0f;

	// animation
	UPROPERTY()
	class UBossAnim* anim;

	// damage
	void OnDamageProcess();

	UPROPERTY(EditAnywhere,Category= BFSM)
	float damageDelayTime = 2;

	// hp
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category= BFSM)
	int32 BossHp = 10;


		
};
