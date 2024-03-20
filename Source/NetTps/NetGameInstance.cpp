// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Online/OnlineSessionNames.h>

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
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnDestroySessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindSessionComplete);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnJoinSessionComplete);
	}
}

void UNetGameInstance::CreateMySession(FString roomName, int32 maxPlayer)
{
	FOnlineSessionSettings sessionSettings;

	// true ������ �˻� �ȴ�.
	sessionSettings.bShouldAdvertise = true;

	// steam ����ϸ� �ش� �ɼ��� true ������ ���� �� �ִ�.
	sessionSettings.bUseLobbiesIfAvailable = true;

	// ���� �������� �ƴ����� �����ٰ���
	sessionSettings.bUsesPresence = true;
	// ���� �÷��� �߿� ������ �� �ְ�
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;
		
	// �ο� �� 
	sessionSettings.NumPublicConnections = maxPlayer;

	// Ŀ���� �ɼ�
	sessionSettings.Set(FName(TEXT("ROOM_NAME")), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// ���� ���� ��û
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	int32 rand = FMath::RandRange(1, 100000);
	mySessionName += FString::Printf(TEXT("%d"), rand);
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);
}

void UNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Success -- %s"), *SessionName.ToString());
		// Battle Map ���� �̵�����
		GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/BattleMap?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Fail"));
	}
}

void UNetGameInstance::DestroyMySession()
{
	sessionInterface->DestroySession(FName(mySessionName));
}

void UNetGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Success -- %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Fail"));
	}
}

void UNetGameInstance::FindOtherSession()
{
	sessionSearch = MakeShared<FOnlineSessionSearch>();

	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionSearch->MaxSearchResults = 10;

	// ���� �˻� ��û
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UNetGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		auto results = sessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Success - count : %d"), results.Num());

		for (int32 i = 0; i < results.Num(); i++)
		{
			FOnlineSessionSearchResult si = results[i];
			FString roomName;
			si.Session.SessionSettings.Get(FName(TEXT("ROOM_NAME")), roomName);
			UE_LOG(LogTemp, Warning, TEXT("%d name : %s, count : %d"), i, *roomName, si.Session.NumOpenPublicConnections);
			
			// ���� ���� ---> String ���� 
			// ������ �ִ� �ο�
			int32 maxPlayer = si.Session.SessionSettings.NumPublicConnections;
			// ������ ���� �ο� (�ִ� �ο� - ���� �ο�)

			int32 currPlayer = maxPlayer - si.Session.NumOpenPublicConnections;

			// ���̸� ( 5 / 10 )
			FString sessionInfo = FString::Printf(
				TEXT("%s ( %d / %d )"), 
				*roomName, currPlayer, maxPlayer);

			onSearchComplete.ExecuteIfBound(i, sessionInfo);
		}

		if (results.Num() > 0)
		{
			JoinOtherSession(0);
		}
		
		/*for (auto si : results)
		{
			FString roomName;
			si.Session.SessionSettings.Get(FName(TEXT("ROOM_NAME")), roomName);
		}*/
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Fail"));
	}
}

void UNetGameInstance::JoinOtherSession(int32 idx)
{
	//TArray<FOnlineSessionSearchResult> 
	auto results = sessionSearch->SearchResults;
	if (sessionInterface == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("sessionInterface is null"));
	}
	if (results.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("results Zero"));

	}
	UE_LOG(LogTemp, Warning, TEXT("results count : %d, idx : %d"), results.Num(), idx);
	sessionInterface->JoinSession(0, FName(mySessionName), results[idx]);
}

void UNetGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Success : %s"), *SessionName.ToString());
		FString url;
		// �����ؾ� �ϴ� Listen ���� URL�� �޾� ����
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("Join session URL : %s"), *url);
		
		if (!url.IsEmpty())
		{
			// �ش� URL �� ��������
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Fail : %d"), result);
	}
}
