// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class EPlayerState
{
	Idle, 
	Move,
	Attack,
	Dodge,
	Damage,
	Die
};



UCLASS()
class BATMAN_ARKHAMKNIGHT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// ī�޶�
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

	// Ű �Է�
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMP_Player;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Dodge;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Attack;

	// �ִϸ��̼�
	UPROPERTY(EditDefaultsOnly)
	class UPlayerAnim* PlayerAnim;

	// ����
	FVector Direction;

	EPlayerState PlayerState = EPlayerState::Idle;

	// ������ ����
	bool bMove;

	// ȸ�� ���� ����
	UPROPERTY(EditDefaultsOnly)
	float DoublePressInterval = 0.5f;
    float LastDodgeKeyPressTime = 0;
    bool bIsDodgeKeyPressed = false;

	UPROPERTY(EditDefaultsOnly)
	float DodgeSpeed = 1000;

private:
	void OnActionMove(const FInputActionValue& Value);
	void OnActionMoveCompleted(const FInputActionValue& Value);
	void OnActionLook(const FInputActionValue& Value);
	void OnActionDodge(const FInputActionValue& Value);
	void OnActionAttack(const FInputActionValue& Value);

public:
	void OnChangedDodgeSpeed(bool bDefault);
};
