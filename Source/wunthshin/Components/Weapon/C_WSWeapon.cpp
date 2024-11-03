// Fill out your copyright notice in the Description page of Project Settings.


#include "C_WSWeapon.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../../Actors/AA_WSCharacter.h"


// Sets default values for this component's properties
UC_WSWeapon::UC_WSWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
	{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ThirdPerson/Input/IMC_SwordAtteck.IMC_SwordAtteck'") };
	check(Asset.Object);
	IMC_Weapon = Asset.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MTG
	{ TEXT("/Script/Engine.AnimMontage'/Game/WawuCharacter/YinLin/AnimationSequence/MTG/MTG_SM_sword_and_shield_attack_Montage.MTG_SM_sword_and_shield_attack_Montage'") };
	check(MTG.Object);
	FirstWeaponAttackMontage = MTG.Object;






	
	

	// ...
}


// Called when the game starts
void UC_WSWeapon::BeginPlay()
{
	Super::BeginPlay();
	OwningPawn = Cast<APawn>(GetOwner()->GetAttachParentActor());
	
	/*USkeletalMeshComponent* MeshComponent = OwningPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (MeshComponent)
	{
		check(false);
		return;
	}
	AActor* OwnerActor = GetOwner();


	BasicAnimInstance = Cast<UBaseAnimInstance>(MeshComponent->GetAnimInstance());
	check(BasicAnimInstance);



	if (APlayerController* PC = Cast<APlayerController>(OwningPawn->GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem->HasMappingContext(IMC_Weapon))
		{
			Subsystem->AddMappingContext(IMC_Weapon, 0);
		}
		AActor* hi = Cast<AActor>(GetOwner());
		if (!hi->InputComponent)
		{
			hi->EnableInput(PC);
			UEnhancedInputComponent* EnhancedInputComponent = Cast< UEnhancedInputComponent>(hi->InputComponent);
			ensure(EnhancedInputComponent);
			const UInputAction* InputAction = nullptr;
			const TArray<FEnhancedActionKeyMapping>& Mappings = IMC_Weapon->GetMappings();
			for (auto& It : Mappings)
			{
				if (It.Action->GetFName() == TEXT("IA_Atteck"))
				{
					InputAction = It.Action.Get();
				}
			}
			if (InputAction)
			{
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::AttackDefault);
			}
			
		}
	}*/



	// ...
	
}

void UC_WSWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	

}

void UC_WSWeapon::AttackDefault()
{
	// todo: 몽타주 재생
	
	if (continuous_attack_count == 0) // 1타
	{
		if (FirstWeaponAttackMontage)
		{
			if (!BasicAnimInstance->Montage_IsPlaying(nullptr))
			{
				BasicAnimInstance->Montage_Play(FirstWeaponAttackMontage);
				continuous_attack_count++; 
			}
		}
	}
	else if (continuous_attack_count == 1) // 2타
	{
		if (!BasicAnimInstance->Montage_IsPlaying(nullptr))
		{
			//BasicAnimInstance->Montage_Play();
			continuous_attack_count++;
		}
	}
	else if (continuous_attack_count ==2)  //3타
	{
		if (!BasicAnimInstance->Montage_IsPlaying(nullptr))
		{
			//BasicAnimInstance->Montage_Play(e);
			continuous_attack_count= 0; // 초기화
			OwningPawn->GetWorldTimerManager().ClearTimer(ResetCounterTimerHandle);
		}
	}
	
	OwningPawn->GetWorldTimerManager().ClearTimer(ResetCounterTimerHandle);
	OwningPawn->GetWorldTimerManager().SetTimer(ResetCounterTimerHandle, this, &UC_WSWeapon::ResetCounter, ResetTime, false);


}

void UC_WSWeapon::ResetCounter()
{
	continuous_attack_count = 0;
	UE_LOG(LogTemp, Warning, TEXT("Attack Counter Reset"));

}


void UC_WSWeapon::AttackSecondary()
{
	// todo: 몽타주 재생
}




