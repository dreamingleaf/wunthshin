// Fill out your copyright notice in the Description page of Project Settings.


#include "wunthshin/AnimNotify/FirstAtteckAnimNotifyState.h"
#include "FirstAtteckAnimNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"


UFirstAtteckAnimNotifyState::UFirstAtteckAnimNotifyState()
{
}

void UFirstAtteckAnimNotifyState::NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyBegin(MeshComp, Animation, FrameDeltaTime);

	
	
	//// 이동 중 매 프레임마다 위치를 변경합니다. 지금 필요한 기능은 아님
	//FVector NewLocation = MeshComp->GetOwner()->GetActorLocation() + FVector(0.f, 200.f * FrameDeltaTime, 0.f); // y축으로 이동
	//MeshComp->GetOwner()->SetActorLocation(NewLocation);
	

	const FVector WeaponSoketLocation = MeshComp->GetSocketLocation(TEXT("RightHandWepon"));// 무기 컴포넌트 이름 넣어야함
	FHitResult HitResult;
	
	
	TArray<AActor*> IgnoreActors;
	// 오너에 따른 엑터에 따라 적과 플레이어를 설정하여 FName("BlockAll"),이부분을 수정하면 된다.
	AActor* OwningActor = MeshComp->GetOwner();



	// 콜리전 네임은 나중에 적과 다른플레이어를 구분하는 콜리전 생성시 변경해야함
	//  RightHandWeapon의 소켓의 위치에 따른 충돌을 탐치하여 인식함
	if (UKismetSystemLibrary::SphereTraceSingleByProfile(MeshComp,
		WeaponSoketLocation,
		WeaponSoketLocation,
		50,
		FName("BlockAll"),
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,// 디버그 보고싶지 않으면 None으로 변경
		HitResult,
		true,FLinearColor::Red, FLinearColor::Green,5.f))
	{
		AActor* DamagedActor = HitResult.GetActor();
		// 여기에 케릭터 스테이트의 공격력을 바탕으로 데미지를 넣으면 됨
	}
}

