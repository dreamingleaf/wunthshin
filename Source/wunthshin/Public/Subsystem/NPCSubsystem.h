// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Interface/DataTableQuery.h"
#include "NPCSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class WUNTHSHIN_API UNPCSubsystem : public UGameInstanceSubsystem, public IDataTableQuery
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Table", meta=(AllowPrivateAccess = "true"))
	UDataTable* AssetDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Table", meta=(AllowPrivateAccess = "true"))
	UDataTable* StatDataTable;
	
public:
	UNPCSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

};