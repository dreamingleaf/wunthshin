#include "CharacterStatsComponent.h"
#include "Engine/DataTable.h"
#include "Logging/LogMacros.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "wunthshin/Actors/AA_WSCharacter.h" 

DEFINE_LOG_CATEGORY(LogCharacterStatsComponent);

UCharacterStatsComponent::UCharacterStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // 스태미나 회복 및 감소 속도 초기화
    StaminaRecoveryRate = 5.0f; // 초당 회복량
    StaminaDepletionRate = 10.0f; // 빠르게 달릴 때 감소량
}

void UCharacterStatsComponent::BeginPlay()
{
    Super::BeginPlay(); // 부모 클래스의 BeginPlay 호출

    InitializeStats(); // 스탯 초기화
}

void UCharacterStatsComponent::DecreaseHP(const float InValue)
{
    if (InValue < 0)
    {
        // 입력은 양수로 (오버/언더플로우 체크를 위해)
        ensureAlways(false);
        IncreaseHP(FMath::Abs(InValue));
        return;
    }

    if (FMath::Abs(std::numeric_limits<float>::min() + CurrentStats.HP) < InValue)
    {
        UE_LOG(LogCharacterStatsComponent, Warning, TEXT("%s: Underflow! assuming HP as 0"), *GetOwner()->GetName());
        CurrentStats.HP = 0;
        return;
    }

    CurrentStats.HP = FMath::Clamp(CurrentStats.HP - InValue, 0, CurrentStats.MaxHP);
}

void UCharacterStatsComponent::IncreaseHP(const float InValue)
{
    if (InValue < 0)
    {
        // 입력은 양수로 (오버/언더플로우 체크를 위해)
        ensureAlways(false);
        DecreaseHP(FMath::Abs(InValue));
        return;
    }

    if (std::numeric_limits<float>::max() - CurrentStats.HP < InValue)
    {
        UE_LOG(LogCharacterStatsComponent, Warning, TEXT("%s: Overflow! assuming HP as MaxHP"), *GetOwner()->GetName());
        CurrentStats.HP = CurrentStats.MaxHP;
        return;
    }

    CurrentStats.HP = FMath::Clamp(CurrentStats.HP + InValue, 0, CurrentStats.MaxHP);
}

void UCharacterStatsComponent::InitializeStats()
{
    // "Player" 항목의 스탯을 찾는다
    FCharacterStats* Stats = nullptr;

#ifdef WITH_EDITOR
    if (GetWorld()->IsEditorWorld())
    {
        Stats = GEditor->GetEditorSubsystem<UCharacterEditorSubsystem>()->GetRowValue<FCharacterStats>(TEXT("Player"));
    }
    else if (GetWorld()->IsGameWorld())
    {
        Stats = GetWorld()->GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->GetRowValue<FCharacterStats>(TEXT("Player"));
    }
    else
    {
        // 지원하지 않는 월드
        check(Stats);
    }
#else
        Stats = GetWorld()->GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->GetRowValue<FCharacterStats>(TEXT("Player"));
#endif

    if (Stats)
    {
        CurrentStats.HP = Stats->MaxHP;
        CurrentStats.MaxHP = Stats->MaxHP;
        CurrentStats.Stamina = Stats->Stamina;
        UE_LOG(LogTemp, Warning, TEXT("MaxHP: %f, Stamina: %f"), CurrentStats.MaxHP, CurrentStats.Stamina);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Row not found in CharacterStatsTable!"));
    }
}

void UCharacterStatsComponent::UpdateStamina(float DeltaTime, bool bIsFastRunning)
{
    if (bIsFastRunning)
    {
        CurrentStats.Stamina -= StaminaDepletionRate * DeltaTime;
        CurrentStats.Stamina = FMath::Max(CurrentStats.Stamina, 0.0f); // 스태미나가 0 이하로 내려가지 않도록

        // 스태미나가 0이 되면 UnFastRun 호출
        if (CurrentStats.Stamina <= 0.0f)
        {
            CurrentStats.Stamina = 0.0f; // 스태미나를 0으로 클램프
            AA_WSCharacter* Character = Cast<AA_WSCharacter>(GetOwner());
            if (Character)
            {
                Character->HandleStaminaDepleted(); // 공개 메서드 호출
            }
        }
    }
    else
    {

        float check = StaminaRecoveryRate * DeltaTime;
        CurrentStats.Stamina += StaminaRecoveryRate * DeltaTime;
        CurrentStats.Stamina = FMath::Min(CurrentStats.Stamina, 100.0f); // 최대 스태미나로 제한
    }
}

void UCharacterStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 여기서 스태미나 업데이트 로직을 추가
    AA_WSCharacter* Character = Cast<AA_WSCharacter>(GetOwner());
    if (Character)
    {
        if (FMath::Abs(Character->GetMovementComponent()->Velocity.Dot(FVector::ForwardVector + FVector::RightVector)) > 0.f)
        {
            UpdateStamina(DeltaTime, Character->IsFastRunning());
        }
    }
}
