// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/EditableTextBox.h>
#include <Components/ScrollBox.h>

#include "NetGameInstance.h"
#include "SessionInfoWidget.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// game instance ��������
	gi = Cast<UNetGameInstance>(GetGameInstance());
	

	// ���� ȭ�� ��ɵ�
	btn_MoveCreateSession->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickMoveCreateSession);
	btn_MoveSearchSession->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickMoveSearchSession);


	// ���� ���� ȭ�� ��ɵ�

	// text_PlayerCount �� �ʱⰪ�� 2�� ����
	text_PlayerCount->SetText(FText::AsNumber(slider_PlayerCount->GetValue()));
	// Slider �� ���� ����� �� �Լ� ���
	slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnValueChanged);

	btn_CreateSession->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickCreateSession);

	// ���� �˻� ȭ�� ��ɵ�
	btn_FindSession->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickFindSession);

	gi->onSearchComplete.BindUObject(this, &ULobbyWidget::OnSearchComplete);
}

void ULobbyWidget::OnClickMoveCreateSession()
{
	// widget switcher �� �̿��ؼ� 1��° Widget �� Ȱ��ȭ �Ƕ�!
	widgetSwitcher->SetActiveWidgetIndex(1);
}

void ULobbyWidget::OnClickMoveSearchSession()
{
	// widget switcher �� �̿��ؼ� 2��° Widget �� Ȱ��ȭ �Ƕ�!
	widgetSwitcher->SetActiveWidgetIndex(2);
}

void ULobbyWidget::OnValueChanged(float Value)
{
	// player count �ؽ�Ʈ ����
	text_PlayerCount->SetText(FText::AsNumber(Value));
}

void ULobbyWidget::OnClickCreateSession()
{
	gi->CreateMySession(
			edit_SessionName->GetText().ToString(), 
			slider_PlayerCount->GetValue());
}

void ULobbyWidget::OnClickFindSession()
{
	gi->FindOtherSession();
}

void ULobbyWidget::OnSearchComplete(int32 idx, FString info)
{
	// SessionInfoWidget ����
	auto widget = CreateWidget<USessionInfoWidget>(GetWorld(), sessionInfoWidgetFactory);
	// Scroll_RoomList �� �߰�
	scroll_RoomList->AddChild(widget);
	// ������� sessionInfo �� �����͸� ����
	widget->SetInfo(idx, info);
}
