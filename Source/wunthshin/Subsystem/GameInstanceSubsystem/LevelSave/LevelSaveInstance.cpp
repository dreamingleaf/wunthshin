// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelSaveInstance.h"
#include "wunthshin/Actors/Pawns/Character/AA_WSCharacter.h"
#include "wunthshin/Actors/Item/Weapon/A_WSWeapon.h"


void ULevelSaveInstance::OnLevelLoaded()
{
    // 저장된 무기 정보가 있으면 무기 복원
    if (SavedWeaponClass)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            AA_WSCharacter* Character = Cast<AA_WSCharacter>(PlayerController->GetPawn());
            if (Character)
            {
                // 1. 기존 무기가 있다면 바로 복원
                UChildActorComponent* RightHandWeapon = Character->GetRightHandComponent();  // getter 사용
                if (RightHandWeapon)
                {
                    // 기존 무기 삭제
                    RightHandWeapon->DestroyComponent();  // 기존 무기 삭제
                }

                // 2. 무기 소환
                AActor* NewWeapon = GetWorld()->SpawnActor<AActor>(SavedWeaponClass);
                if (!NewWeapon)
                {
                    UE_LOG(LogTemp, Warning, TEXT("NewWeapon을 생성할 수 없습니다. SavedWeaponClass: %s"), *SavedWeaponClass->GetName());
                    return;
                }

                // 3. 무기 자산 이름 설정
                AA_WSWeapon* Weapon = Cast<AA_WSWeapon>(NewWeapon);
                if (Weapon)
                {
                    Weapon->SetWeaponAssetName(SavedWeaponAssetName);  // 무기 자산 이름 설정
                }

                // 4. 무기 위치 설정: WeaponProp01 소켓에 Attach
                FTransform SocketTransform = Character->GetMesh()->GetSocketTransform(SavedSocketName);
                FVector SocketLocation = SocketTransform.GetLocation();
                FRotator SocketRotation = SocketTransform.GetRotation().Rotator();

                // 소켓 위치와 회전 확인
                UE_LOG(LogTemp, Warning, TEXT("Socket 위치: %s, 회전: %s"), *SocketLocation.ToString(), *SocketRotation.ToString());

                // 무기의 위치를 소켓에 맞추어 설정
                NewWeapon->SetActorLocation(SocketLocation);
                NewWeapon->SetActorRotation(SocketRotation);

                // 무기를 소켓에 부착
                NewWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SavedSocketName);

                // 무기가 아웃라이너에 보이도록 설정
                NewWeapon->SetActorHiddenInGame(false);  // 무기 숨기지 않음
                UStaticMeshComponent* WeaponMesh = Cast<UStaticMeshComponent>(NewWeapon->GetRootComponent());
                if (WeaponMesh)
                {
                    WeaponMesh->SetVisibility(true);  // 메시 보이게 설정
                }

                // 충돌 활성화
                NewWeapon->SetActorEnableCollision(true); // 충돌 활성화

                // 확인: 무기가 실제로 소켓에 부착되었는지 확인
                UE_LOG(LogTemp, Warning, TEXT("NewWeapon 위치: %s"), *NewWeapon->GetActorLocation().ToString());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SavedWeaponClass가 설정되지 않았습니다."));
    }
}
