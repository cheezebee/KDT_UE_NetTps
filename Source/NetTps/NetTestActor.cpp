// Fill out your copyright notice in the Description page of Project Settings.


#include "NetTestActor.h"
#include <Net/UnrealNetwork.h>

// Sets default values
ANetTestActor::ANetTestActor()
{
	PrimaryActorTick.bCanEverTick = true;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TEXT"));
	SetRootComponent(compMesh);

	// ����� �����ϰ� ����
	//bReplicates = true;
	SetReplicates(true);
}

void ANetTestActor::BeginPlay()
{
	Super::BeginPlay();

	mat = compMesh->CreateDynamicMaterialInstance(0);
}

void ANetTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintNetLog();

	// ���࿡ ������� ȸ�� ���Ѷ�
	if (HasAuthority())
	{
		AddActorLocalRotation(FRotator(0, 50 * DeltaTime, 0));
		rotYaw = GetActorRotation().Yaw;
	}
	// Ŭ���̾�Ʈ ���
	/*else
	{
		FRotator rot = GetActorRotation();
		rot.Yaw = rotYaw;
		SetActorRotation(rot);
	}*/
	
	TestScale();	

	TestColor();
}

void ANetTestActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetTestActor, rotYaw);
	DOREPLIFETIME(ANetTestActor, scaleValue);
	DOREPLIFETIME(ANetTestActor, matColor);
}


void ANetTestActor::OnRep_RotYaw()
{
	FRotator rot = GetActorRotation();
	rot.Yaw = rotYaw;
	SetActorRotation(rot);
}

void ANetTestActor::TestScale()
{
	// ���࿡ �������
	if (HasAuthority())
	{
		// 1. ���࿡ bScaleIncrease true ���
		if (bScale == true)
		{
			// 1 - 1. FVector �ڷ��� ���� ���� ũ�⸦ ���� ����
			FVector s = GetActorScale3D();
			// 1 - 2. ���� �����Ǹ� 50 * DeltaTime ��ŭ ������Ű��
			s += FVector(1 * GetWorld()->DeltaTimeSeconds);
			// 1 - 3. s �� ���� ������ ������ ����.
			SetActorScale3D(s);
			// 1 - 4. ���࿡ 1.5���� Ŀ���� bScaleIncrease �� false;
			if (s.X > 1.5f)
			{
				bScale = false;
			}
			scaleValue = s;
		}
		// 2. �׷��� ������
		else
		{
			// 1 - 1. FVector �ڷ��� ���� ���� ũ�⸦ ���� ����
			FVector s = GetActorScale3D();
			// 1 - 2. ���� �����Ǹ� 50 * DeltaTime ��ŭ ���ҽ�Ű��
			s -= FVector(1 * GetWorld()->DeltaTimeSeconds);
			// 1 - 3. s �� ���� ������ ������ ����.
			SetActorScale3D(s);
			// 1 - 4. ���࿡ 0.5���� �۾����� bScaleIncrease �� true;
			if (s.X < 0.5f)
			{
				bScale = true;
			}
			scaleValue = s;
		}
	}
	else
	{
		SetActorScale3D(scaleValue);
	}
}

void ANetTestActor::TestColor()
{
	// ������� ����ó��(2�ʸ��� ������ �� ����)
	if (HasAuthority())
	{
		currTime += GetWorld()->GetDeltaSeconds();
		if (currTime > 2)
		{
			currTime = 0;
			matColor = FLinearColor::MakeRandomColor();
			mat->SetVectorParameterValue(TEXT("FloorColor"), matColor);
		}
	}
	else
	{
		mat->SetVectorParameterValue(TEXT("FloorColor"), matColor);
	}
}

void ANetTestActor::PrintNetLog()
{
	// Connection ����
	FString conStr = GetNetConnection() != nullptr ? TEXT("Valid Connect") : TEXT("InValid Connect");
	// ���� ���� Actor
	FString ownerStr = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	// Role
	// ROLE_Authority : ��� ������ �� ���� �ִ� (���� ����)
	// ROLE_AutonomousProxy : ���� (Input) �� �����ϴ�.
	// ROLE_SimulatedProxy : ���⸸ (�ùķ��̼Ǹ�) �����Ѵ�.
	FString localRoleStr = UEnum::GetValueAsString<ENetRole>(GetLocalRole());
	FString remoteRoleStr = UEnum::GetValueAsString<ENetRole>(GetRemoteRole());

	FString log = FString::Printf(TEXT("Connection : %s\nOwner Name : %s\nLocalRole : %s\nRemoteRole : %s"),
		*conStr, *ownerStr, *localRoleStr, *remoteRoleStr);

	DrawDebugString(
		GetWorld(),
		GetActorLocation() + FVector::UpVector * 100,
		log,
		nullptr,
		FColor::Yellow,
		0,
		true,
		1.0);
}

