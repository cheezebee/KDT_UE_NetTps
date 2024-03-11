// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include <Components/ProgressBar.h>

void UHealthBar::UpdateHealthBar(float currHP, float maxHP)
{
	// 0 ~ 1 �� ���̰�
	float percent = currHP / maxHP;
	healthBar->SetPercent(percent);
}
