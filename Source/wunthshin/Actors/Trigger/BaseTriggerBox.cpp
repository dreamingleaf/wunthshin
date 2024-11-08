#include "BaseTriggerBox.h"
#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "wunthshin/Subsystem/GameInstanceSubsystem/LevelSave/LevelSaveInstance.h"
#include "wunthshin/Actors/Pawns/Character/AA_WSCharacter.h"

ABaseTriggerBox::ABaseTriggerBox()
{
    // ActiveCollisionComponent 초기화
    ActiveCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ActiveCollisionComp"));
    ActiveCollisionComponent->SetupAttachment(RootComponent);

    // 충돌 프로필 수정
    ActiveCollisionComponent->SetCollisionObjectType(ECC_Pawn);
    ActiveCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    ActiveCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    ActiveCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnActivate);
    ActiveCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnDeactivate);
}

void ABaseTriggerBox::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseTriggerBox::OnActivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        AA_WSCharacter* A_WSCharacter = Cast<AA_WSCharacter>(OtherActor);
        if (A_WSCharacter)
        {
            ShowPortalUI(A_WSCharacter); // PlayerCharacter를 인자로 전달
        }
    }
}

void ABaseTriggerBox::OnDeactivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (PortalUI)
    {
        // UI가 보일 때만 제거
        if (bIsUIVisible)
        {
            PortalUI->RemoveFromParent();
            PortalUI = nullptr;
            bIsUIVisible = false; // UI가 사라짐

            // 플레이어 입력 활성화 코드...
            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
            if (PlayerController)
            {
                FInputModeGameOnly InputMode;
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = false; // 마우스 커서 숨김
            }
        }
    }
}

void ABaseTriggerBox::ShowPortalUI(AA_WSCharacter* A_WSCharacter)
{
    if (PortalUIClass && !PortalUI) // PortalUIClass가 nullptr이 아닐 때
    {
        PortalUI = CreateWidget<UUserWidget>(GetWorld(), PortalUIClass); // UPortalUI가 아닌 UUserWidget으로 변경
        if (PortalUI)
        {
            PortalUI->AddToViewport();

            // 버튼 클릭 이벤트 바인딩
            UButton* YesButton = Cast<UButton>(PortalUI->GetWidgetFromName(TEXT("YesButton")));
            UButton* NoButton = Cast<UButton>(PortalUI->GetWidgetFromName(TEXT("NoButton")));
            if (YesButton)
            {
                YesButton->OnClicked.AddDynamic(this, &ABaseTriggerBox::OnYesClicked);
            }
            if (NoButton)
            {
                NoButton->OnClicked.AddDynamic(this, &ABaseTriggerBox::OnNoClicked);
            }

            // 마우스 커서 표시 및 입력 모드 설정
            APlayerController* PlayerController = Cast<APlayerController>(A_WSCharacter->GetController());
            if (PlayerController)
            {
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(PortalUI->TakeWidget());
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = true;
            }

            // 캐릭터의 입력 비활성화
            A_WSCharacter->DisableInput(PlayerController);
        }
    }
}

void ABaseTriggerBox::OnYesClicked()
{
    // RightHandWeapon에 대한 정보를 저장
    ULevelSaveInstance* LevelSave = GetWorld()->GetGameInstance()->GetSubsystem<ULevelSaveInstance>();

    // 1. 플레이어 컨트롤러 확인
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    // 2. 캐릭터 확인
    AA_WSCharacter* Character = Cast<AA_WSCharacter>(PlayerController->GetPawn());
    if (!Character) return;

    // 3. 무기 정보 저장
    if (Character && Character->GetRightHandComponent())
    {
        if (LevelSave)
        {
            // 무기 정보 저장
            LevelSave->SavedWeaponClass = Character->GetRightHandComponent()->GetChildActorClass();
            LevelSave->SavedSocketName = Character->GetRightHandWeaponSocketName();
            //LevelSave->SavedWeaponAssetName = Character->GetRightHandComponent()->Get;  // 수정된 메서드 호출
            AA_WSWeapon* Weapon = Cast<AA_WSWeapon>(Character->GetRightHandComponent()->GetChildActor());
            if (Weapon)
            {
                LevelSave->SavedWeaponAssetName = Weapon->GetWeaponAssetName(); // GetWeaponAssetName()은 AA_WSWeapon에 있음
            }
        }
    }

    // 4. 레벨 전환
    if (!SelectedLevel.IsNull())
    {
        // 레벨 이름으로 레벨 로드
        UGameplayStatics::OpenLevel(GetWorld(), *SelectedLevel.GetAssetName());
        if (LevelSave)
        {
            LevelSave->OnLevelLoaded();  // 레벨 로딩 후 무기 소환
        }
    }

    // 5. UI 비활성화
    if (PortalUI)
    {
        PortalUI->RemoveFromParent();
        PortalUI = nullptr;
        bIsUIVisible = false; // UI가 사라짐
    }

    // 6. 입력 모드 복구
    if (PlayerController)
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false; // 마우스 커서 숨김
    }
}

void ABaseTriggerBox::OnNoClicked()
{
    if (PortalUI)
    {
        PortalUI->RemoveFromParent();
        PortalUI = nullptr;
        bIsUIVisible = false;

        // 플레이어 컨트롤러 가져오기
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            // 입력 모드 설정: 게임 전용
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = false; // 마우스 커서 숨김

            // 캐릭터의 입력 활성화
            if (AA_WSCharacter* A_WSCharacter = Cast<AA_WSCharacter>(PlayerController->GetPawn()))
            {
                A_WSCharacter->EnableInput(PlayerController);
            }
        }
    }
}

//// 무기 정보를 LevelSaveInstance에 저장하는 함수
//void ABaseTriggerBox::SaveWeaponToLevelSaveInstance()
//{
//    ULevelSaveInstance* LevelSave = NewObject<ULevelSaveInstance>(GetWorld());
//    if (LevelSave)
//    {
//        AA_WSCharacter* Character = Cast<AA_WSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
//        if (Character && Character->GetRightHandComponent())
//        {
//            // 무기 정보를 LevelSaveInstance에 저장
//            LevelSave->SavedWeaponClass = Character->GetRightHandComponent()->GetChildActorClass();
//            LevelSave->SavedSocketName = Character->GetRightHandWeaponSocketName(); // FName 타입을 그대로 저장
//            LevelSave->SavedWeaponAssetName = Character->GetAssetName();
//        }
//    }
//}