// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerController.h"
#include "NetTpsGameMode.h"

void ANetPlayerController::RespawnPlayer()
{
	// �ش� Controller �� ��Ʈ�� �ϰ� �ִ� Pawn
	APawn* player = GetPawn();

	// ���� ������
	UnPossess();

	// ���� Player �� ����
	player->Destroy();

	// ���ο� Player �� ���� �ٽ� ����
	ANetTpsGameMode* gm = Cast<ANetTpsGameMode>(GetWorld()->GetAuthGameMode());
	gm->RestartPlayer(this);
}
