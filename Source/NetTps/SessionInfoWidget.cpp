// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionInfoWidget.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>


void USessionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_JoinSession->OnClicked.AddDynamic(this, &USessionInfoWidget::OnClickJoinSession);
}

void USessionInfoWidget::OnClickJoinSession()
{
}

void USessionInfoWidget::SetInfo(int32 idx, FString info)
{
	// �� ��° �������� ��Ƴ���
	sessionIdx = idx;
	// ȭ�鿡 ���̴� ���� ����
	text_SessionInfo->SetText(FText::FromString(info));
}
