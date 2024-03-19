// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyWidget.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	// lobby widget ���� viewport �� ������
	lobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), lobbyWidgetFactory);
	lobbyWidget->AddToViewport();

	// ���콺 ������ ����
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
}
