// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Interfaces/OnlineSessionInterface.h>

// OnlineSessionInterface ���ؼ� �Ѵ�.
// ������ �����. 
// ������ �˻�
// ������ ����

void UNetGameInstance::Init()
{
	Super::Init();

	// �¶��� ���� �ý��� ��������
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		// ���� �������̽� ��������
		sessionInterface = subsys->GetSessionInterface();
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);
	}
}

void UNetGameInstance::CreateMySession()
{
	FOnlineSessionSettings sessionSettings;

	// true ������ �˻� �ȴ�.
	sessionSettings.bShouldAdvertise = true;

	// steam ����ϸ� �ش� �ɼ��� true ������ ���� �� �ִ�.
	sessionSettings.bUseLobbiesIfAvailable = true;

	// �ο� �� 
	sessionSettings.NumPublicConnections = 10;

	// Ŀ���� �ɼ�
	sessionSettings.Set(FName(TEXT("ROOM_NAME")), FString(TEXT("KHJ_ROOM")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// ���� ���� ��û
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	sessionInterface->CreateSession(*netID, FName(TEXT("lokimve7")), sessionSettings);
}

void UNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Success -- %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Fail"));
	}
}
