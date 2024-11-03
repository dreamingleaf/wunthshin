// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../Source/wunthshin/AnimInstance/BaseAnimInstance.h"

#include "C_WSWeapon.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WUNTHSHIN_API UC_WSWeapon : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UC_WSWeapon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void AttackDefault();

	virtual void AttackSecondary();

	void ResetCounter();

	

protected: // Owner
	// 이 무기의 소유권자 Pawn
	APawn* OwningPawn = nullptr;
	UBaseAnimInstance* BasicAnimInstance = nullptr;

protected: // Input
	class UInputMappingContext* IMC_Weapon = nullptr;

protected : // MTG
	UAnimMontage* FirstWeaponAttackMontage;

	//연속 공격 카운트
protected:
	float continuous_attack_count = 0;
	FTimerHandle ResetCounterTimerHandle;
	float ResetTime = 2.0f;
	
};
