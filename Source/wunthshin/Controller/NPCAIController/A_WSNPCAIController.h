// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "A_WSNPCAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class WUNTHSHIN_API AA_WSNPCAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BehaivorTree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BehaivorTree", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;

	// 사용중인 에셋의 복사본
	// 컴포넌트 설정 대신 해당 에셋을 바꾸어 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

public:
	AA_WSNPCAIController();

	UFUNCTION(BlueprintCallable)
	void SetBehaviorTree(UBehaviorTree* InAsset);
	
	virtual void BeginPlay() override;

};