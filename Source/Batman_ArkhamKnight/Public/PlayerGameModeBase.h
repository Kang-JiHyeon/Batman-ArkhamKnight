// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API APlayerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    virtual void StartPlay() override;

private:
	// ���� ������ �ּ�
    UPROPERTY(EditAnywhere, Category = "Camera")
    class ULevelSequence* AttackSequence;

    // ���� ������ �÷��̾�
    UPROPERTY()
    class ULevelSequencePlayer* SequencePlayer;

    // ���� ������ ����
    UPROPERTY()
    class ALevelSequenceActor* SequenceActor;

public:
    void PlaySequence();
    bool IsPlayingSequence();
};
