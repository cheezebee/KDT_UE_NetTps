// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerAnimInstance.h"
#include "NetTpsCharacter.h"

void UNetPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Player ��������
	player = Cast<ANetTpsCharacter>(TryGetPawnOwner());

}

void UNetPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player)
	{
		// �� �� (dirV)
		dirV = FVector::DotProduct(player->GetActorForwardVector(), player->GetVelocity());

		// �� �� (dirH)
		dirH = FVector::DotProduct(player->GetActorRightVector(), player->GetVelocity());

		//0 ~ 360 --> -180 ~ 180
		pitchAngle = -player->GetBaseAimRotation().Pitch;
		UE_LOG(LogTemp, Warning, TEXT("anlge : %f"), pitchAngle);

		pitchAngle = -player->GetBaseAimRotation().GetNormalized().Pitch;
		UE_LOG(LogTemp, Warning, TEXT("anlge normailized : %f"), pitchAngle);
	}	
}

void UNetPlayerAnimInstance::AnimNotify_ReloadComplete()
{
	player->ReloadComplete();
}
