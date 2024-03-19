// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� ȭ�� ��ɵ�
	btn_MoveCreateSession->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickMoveCreateSession);
	btn_MoveSearchSession->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickMoveSearchSession);
}

void ULobbyWidget::OnClickMoveCreateSession()
{
	// widget switcher �� �̿��ؼ� 1��° Widget �� Ȱ��ȭ �Ƕ�!
	widgetSwitcher->SetActiveWidgetIndex(1);
}

void ULobbyWidget::OnClickMoveSearchSession()
{
}
