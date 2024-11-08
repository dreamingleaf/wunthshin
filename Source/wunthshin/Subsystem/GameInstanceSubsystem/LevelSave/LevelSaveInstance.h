#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "UObject/NoExportTypes.h"
#include "wunthshin/Actors/Pawns/Character/AA_WSCharacter.h"
#include "wunthshin/Actors/Item/Weapon/A_WSWeapon.h"
#include "LevelSaveInstance.generated.h"

class AA_WSCharacter;
class AA_WSWeapon;

UCLASS(Blueprintable)
class ULevelSaveInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    // 저장할 무기 정보
    UPROPERTY(BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AA_WSWeapon> SavedWeaponClass;

    UPROPERTY(BlueprintReadWrite, Category = "Weapon")
    FName SavedSocketName;

    UPROPERTY(BlueprintReadWrite, Category = "Weapon")
    FName SavedWeaponAssetName;

    // 레벨 로딩 후 무기 소환
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void OnLevelLoaded();
};



