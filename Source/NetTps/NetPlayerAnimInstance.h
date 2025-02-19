// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NetPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UNetPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public: 

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool hasPistol;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float dirH;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float dirV;

	UPROPERTY()
	class ANetTpsCharacter* player;

};
