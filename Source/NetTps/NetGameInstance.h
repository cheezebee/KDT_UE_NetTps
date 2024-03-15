// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetGameInstance.generated.h"



/**
 * 
 */
UCLASS()
class NETTPS_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	// ���� ����� �Լ�
	UFUNCTION(BlueprintCallable)
	void CreateMySession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

public:
	// �� �������� (���� �����, ���� �˻�, ���� ����)
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionInterface;
};
