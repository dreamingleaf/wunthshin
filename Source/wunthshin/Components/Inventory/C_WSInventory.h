// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_WSInventory.generated.h"

class USG_WSItemMetadata;
class AA_WSItem;

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

USTRUCT()
struct FInventoryPair
{
	GENERATED_BODY()

public:
	FInventoryPair() : Metadata(nullptr), Count(0) {}

	FInventoryPair(const USG_WSItemMetadata* InMetadata) :
		Metadata(InMetadata), Count(1) {}

	UPROPERTY(VisibleAnywhere)
	const USG_WSItemMetadata* Metadata;

	UPROPERTY(VisibleAnywhere)
	uint32 Count;

	bool operator==(const FInventoryPair& InOther) 
	{
		// 테이블로부터 생성된 포인터 비교, unique하다 가정
		return Metadata == InOther.Metadata && 
			Count == InOther.Count;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WUNTHSHIN_API UC_WSInventory : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	TArray<FInventoryPair> Items;

	FInventoryPair* FindItem(const USG_WSItemMetadata* InMetadata);

public:	
	// Sets default values for this component's properties
	UC_WSInventory();

	int32 FindItemIndex(const USG_WSItemMetadata* InMetadata) const;

	const TArray<FInventoryPair>& GetItems() const { return Items; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void AddItem(AA_WSItem* InItem, int InCount = 1);	// 아이템 추가
	void RemoveItem(AA_WSItem* InItem, int InCount = 1); // 아이템 빼기
	void UseItem(uint32 Index, AActor* InTarget, int InCount = 1);	// 아이템 사용

};
