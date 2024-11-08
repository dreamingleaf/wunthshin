// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelSaveInstance.h"
#include "wunthshin/Actors/Pawns/Character/AA_WSCharacter.h"


void ULevelSaveInstance::OnLevelLoaded()
{
    // 저장된 무기 정보가 있으면 무기 소환
    if (SavedWeaponClass)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            AA_WSCharacter* Character = Cast<AA_WSCharacter>(PlayerController->GetPawn());
            if (Character)
            {
                // 캐릭터가 무기를 가지고 있으면 삭제하고 새로 소환
                UChildActorComponent* RightHandWeapon = Character->GetRightHandComponent();  // getter 사용
                if (RightHandWeapon)
                {
                    RightHandWeapon->DestroyComponent();  // Destroy() -> DestroyComponent()
                }

                // 무기 소환 및 장착
                AActor* NewWeapon = GetWorld()->SpawnActor<AActor>(SavedWeaponClass);
                if (NewWeapon)
                {
                    // 1. NewWeapon의 루트 컴포넌트가 UChildActorComponent인지 확인
                    UChildActorComponent* WeaponComponent = Cast<UChildActorComponent>(NewWeapon->GetRootComponent());

                    if (WeaponComponent)
                    {
                        // 2. 무기를 RightHandWeapon에 설정
                        RightHandWeapon = WeaponComponent;

                        // 3. 무기 자산을 캐릭터에 장착
                        RightHandWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SavedSocketName);

                        // 4. 무기 자산 이름 설정
                        AA_WSWeapon* Weapon = Cast<AA_WSWeapon>(RightHandWeapon->GetChildActor());
                        if (Weapon)
                        {
                            Weapon->SetWeaponAssetName(SavedWeaponAssetName);  // 무기 자산 이름 설정
                        }
                        else
                        {
                            UE_LOG(LogTemp, Warning, TEXT("NewWeapon의 자식 액터가 AA_WSWeapon이 아닙니다."));
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("NewWeapon의 루트 컴포넌트가 UChildActorComponent가 아닙니다."));
                    }
                }
            }
        }
    }
}
