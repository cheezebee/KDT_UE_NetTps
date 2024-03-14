// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include <Components/Image.h>
#include <Components/HorizontalBox.h>

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (isShowDamageUI)
	{
		// ������ UI �� ������ ������ �ʰ� ����.	
		// 1. currOpacity �� �پ��� ����
		currOpacity -= InDeltaTime;
		// 2. �� ���� ������ UI �� opacity ������ ����
		damageUI->SetRenderOpacity(currOpacity);
		// 3. currOpacity �� 0���� ���ų� �۾�����
		if (currOpacity <= 0)
		{
			// 4. ������ UI �� �Ⱥ��̰� ����
			isShowDamageUI = false;
		}
	}	
}

void UMainWidget::ShowPistolUI(bool isShow)
{
	if (isShow)
	{
		crosshair->SetVisibility(ESlateVisibility::Visible);
		bulletPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		crosshair->SetVisibility(ESlateVisibility::Hidden);
		bulletPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainWidget::AddBullet()
{
	// �Ѿ� ���� ����
	UUserWidget* bullet = CreateWidget(GetWorld(), bulletFactory);
	// ������ �Ѿ� ������ bulletPanel �߰�����
	bulletPanel->AddChildToHorizontalBox(bullet);
}

void UMainWidget::RemoveBullet()
{
	// bulletPanel �� �ڽ��� ���� - 1
	int32 idx = bulletPanel->GetChildrenCount() - 1;
	// idx ��° �ڽ��� ������
	bulletPanel->RemoveChildAt(idx);
}

void UMainWidget::ShowDamageUI()
{
	isShowDamageUI = true;
	currOpacity = 1;
}
