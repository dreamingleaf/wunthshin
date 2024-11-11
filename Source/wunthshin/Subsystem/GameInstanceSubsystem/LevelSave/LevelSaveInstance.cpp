// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelSaveInstance.h"
#include "wunthshin/Actors/Pawns/Character/AA_WSCharacter.h"
#include "wunthshin/Actors/Item/Weapon/A_WSWeapon.h"

//void ULevelSaveInstance::OnLevelLoaded()
//{
//    // 저장된 무기 정보가 있으면 무기 복원
//    if (SavedWeaponClass)
//    {
//        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//        if (PlayerController)
//        {
//            AA_WSCharacter* Character = Cast<AA_WSCharacter>(PlayerController->GetPawn());
//            if (Character)
//            {
//                // 기존 무기가 있다면 삭제
//                UChildActorComponent* RightHandWeapon = Character->GetRightHandComponent();  // getter 사용
//                if (RightHandWeapon)
//                {
//                    RightHandWeapon->DestroyComponent();  // 기존 무기 삭제
//                }
//
//                // 무기 소환
//                AActor* NewWeapon = GetWorld()->SpawnActor<AActor>(SavedWeaponClass);
//                if (!NewWeapon)
//                {
//                    UE_LOG(LogTemp, Warning, TEXT("NewWeapon을 생성할 수 없습니다. SavedWeaponClass: %s"), *SavedWeaponClass->GetName());
//                    return;
//                }
//
//                // 무기 자산 이름 설정
//                AA_WSWeapon* Weapon = Cast<AA_WSWeapon>(NewWeapon);
//                if (Weapon)
//                {
//                    Weapon->SetWeaponAssetName(SavedWeaponAssetName);  // 무기 자산 이름 설정
//                }
//
//                // 소켓의 월드 좌표와 회전 값 가져오기
//                FTransform SocketWorldTransform = Character->GetMesh()->GetSocketTransform(SavedSocketName, RTS_World);
//                FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
//                FRotator SocketWorldRotation = SocketWorldTransform.GetRotation().Rotator();
//
//                // 디버그 로그로 소켓 위치와 회전 확인 (월드 좌표 기준)
//                UE_LOG(LogTemp, Warning, TEXT("소켓 위치 (월드 기준): %s, 회전 (월드 기준): %s"), *SocketWorldLocation.ToString(), *SocketWorldRotation.ToString());
//
//                // 캐릭터의 월드 위치 확인
//                FVector CharacterWorldLocation = Character->GetActorLocation();
//                UE_LOG(LogTemp, Warning, TEXT("캐릭터 월드 위치: %s"), *CharacterWorldLocation.ToString());
//
//                // 무기의 위치와 회전 설정
//                NewWeapon->SetActorLocation(SocketWorldLocation);  // 소켓 위치에 맞춰 무기 위치 설정
//                NewWeapon->SetActorRotation(SocketWorldRotation);  // 소켓 회전에 맞춰 무기 회전 설정
//
//                // 무기를 소켓에 부착
//                NewWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SavedSocketName);
//
//                // 무기가 아웃라이너에 보이도록 설정
//                NewWeapon->SetActorHiddenInGame(false);  // 무기 숨기지 않음
//                UStaticMeshComponent* WeaponMesh = Cast<UStaticMeshComponent>(NewWeapon->GetRootComponent());
//                if (WeaponMesh)
//                {
//                    WeaponMesh->SetVisibility(true);  // 메시 보이게 설정
//                    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
//                }
//
//                // 충돌 활성화
//                NewWeapon->SetActorEnableCollision(true);  // 충돌 활성화
//
//                // 무기 위치와 회전 디버그 출력
//                UE_LOG(LogTemp, Warning, TEXT("NewWeapon 위치: %s"), *NewWeapon->GetActorLocation().ToString());
//                UE_LOG(LogTemp, Warning, TEXT("NewWeapon 회전: %s"), *NewWeapon->GetActorRotation().ToString());
//            }
//        }
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("SavedWeaponClass가 설정되지 않았습니다."));
//    }
//}


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
                // 레벨이 변경된 후 캐릭터의 월드 위치를 가져옵니다.
                FVector CharacterWorldLocation = Character->GetActorLocation();
                UE_LOG(LogTemp, Warning, TEXT("새로운 캐릭터 월드 위치: %s"), *CharacterWorldLocation.ToString());

                // 기존 무기가 있다면 삭제
                UChildActorComponent* RightHandWeapon = Character->GetRightHandComponent();  // getter 사용
                if (RightHandWeapon)
                {
                    RightHandWeapon->DestroyComponent();  // 기존 무기 삭제
                }

                RightHandWeapon->CreateChildActor([](AActor* InActor)
                    {
                        Weapon->SetWeaponAssetName(SavedWeaponAssetName);  // 무기 자산 이름 설정
                    });

                //// 무기가 아웃라이너에 보이도록 설정
                //NewWeapon->SetActorHiddenInGame(false);  // 무기 숨기지 않음
                //UStaticMeshComponent* WeaponMesh = Cast<UStaticMeshComponent>(NewWeapon->GetRootComponent());
                //if (WeaponMesh)
                //{
                //    WeaponMesh->SetVisibility(true);  // 메시 보이게 설정
                //    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
                //}

                // 충돌 활성화
               // NewWeapon->SetActorEnableCollision(true);  // 충돌 활성화

                // 무기 위치와 회전 디버그 출력
               /* UE_LOG(LogTemp, Warning, TEXT("NewWeapon 위치: %s"), *NewWeapon->GetActorLocation().ToString());
                UE_LOG(LogTemp, Warning, TEXT("NewWeapon 회전: %s"), *NewWeapon->GetActorRotation().ToString());*/
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SavedWeaponClass가 설정되지 않았습니다."));
    }
}
