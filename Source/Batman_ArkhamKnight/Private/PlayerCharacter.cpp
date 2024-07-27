// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnim.h"
#include "Components/BoxComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��������
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	// ī�޶�
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// ȸ�� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ��Ʈ�ѷ��� �����ͼ� PlayerController���� ĳ�����Ѵ�.
	auto* playerController = Cast<APlayerController>(Controller);

	// ĳ���� �����ߴٸ�
	if (playerController != nullptr)
	{
		// �� ��ü�� �̿��ؼ� EnhancedInputLocalPlayerSubsystem�� �����´�.
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

		// SubSys�� �̿��Ͽ� AddMappingContext�� �Ѵ�.
		if (subSys != nullptr)
		{
			subSys->AddMappingContext(IMP_Player, 0);
		}
	}
	
	// �ִϸ��̼�
	PlayerAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ȸ�� �������� �̵��ϰ� �ʹ�.
	// 1.ControlRotation�� �̿��ؼ� Transform �� �����
	FTransform controlTransform = FTransform(GetControlRotation());
	// 2. TransformDirection ����� �̿��ؼ� ������ ����
	Direction = controlTransform.TransformVector(Direction);
	// 3. �� �������� �̵��Ѵ�.
	AddMovementInput(Direction, 1);
	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnActionMove);
	Input->BindAction(IA_Move, ETriggerEvent::Completed, this, &APlayerCharacter::OnActionMoveCompleted);
	Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::OnActionLook);
	Input->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APlayerCharacter::OnActionDodge);
	Input->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::OnActionAttack);
	//Input->BindAction(IA_Dodge, ETriggerEvent::Completed, this, &APlayerCharacter::OnActionDodgeCompleted);

}

void APlayerCharacter::OnActionMove(const FInputActionValue& Value)
{
	if (PlayerAnim->bDodge)
	{
		return;
	}

	FVector2D v = Value.Get<FVector2D>();

	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Normalize();


	if(bMove == false)
		bMove = true;
}

void APlayerCharacter::OnActionMoveCompleted(const FInputActionValue& Value)
{
	bMove = false;
}

void APlayerCharacter::OnActionLook(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();

    AddControllerPitchInput(-v.Y);
    AddControllerYawInput(v.X);
}

/// <summary>
/// �̵� ������ �ְ�, �����̽� �ٸ� 2�� ���� ��� �����⸦ �Ѵ�.
/// </summary>
/// <param name="Value"></param>
void APlayerCharacter::OnActionDodge(const FInputActionValue& Value)
{
	if(bMove == false) return;

	float currtime = GetWorld()->GetTimeSeconds();

	if (currtime - LastDodgeKeyPressTime <= DoublePressInterval)
	{
        if (PlayerAnim != nullptr)
        {
            PlayerAnim->SetDodge(true);
        }

		GetCharacterMovement()->Velocity = DodgeSpeed * GetActorForwardVector();
        Jump();
	}
	
	LastDodgeKeyPressTime = currtime;
}

void APlayerCharacter::OnActionAttack(const FInputActionValue& Value)
{
	
}
