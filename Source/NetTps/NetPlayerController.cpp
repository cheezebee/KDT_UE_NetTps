// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerController.h"
#include "NetTpsGameMode.h"
#include <GameFramework/SpectatorPawn.h>
#include <Kismet/KismetSystemLibrary.h>

void ANetPlayerController::RespawnPlayer()
{
	ServerRPC_RespawnPlayer();
}

void ANetPlayerController::ServerRPC_RespawnPlayer_Implementation()
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

void ANetPlayerController::ChangeToSpectator()
{
	ServerRPC_ChangeToSpectator();
}

void ANetPlayerController::ServerRPC_ChangeToSpectator_Implementation()
{
	APawn* player = GetPawn();
	if (player)
	{
		ANetTpsGameMode* gm = Cast<ANetTpsGameMode>(GetWorld()->GetAuthGameMode());

		// ������ Pawn ������
		ASpectatorPawn* spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, player->GetActorTransform());

		// ���� �÷��̾� Possess ����
		UnPossess();

		// ������ Pawn �� Possess
		Possess(spectator);

		// ���� �÷��̾� ����
		player->Destroy();

		// 5�� �ڿ� �ٽ� ��Ƴ���
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("RespawnPlayer"), 5, false);
	}
}
