// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FirstAtteckAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class WUNTHSHIN_API UFirstAtteckAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UFirstAtteckAnimNotifyState();
	
protected:
	virtual void NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime);
};
