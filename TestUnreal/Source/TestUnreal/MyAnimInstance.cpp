// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"


UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(
		TEXT("AnimMontage'/Game/Animations/Greystone_Skeleton_Montage.Greystone_Skeleton_Montage'"));

	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	
	Montage_Play(AttackMontage, 1.0f);
}

void UMyAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UMyAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UMyAnimInstance::AnimNotify_AttackHit()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit"));
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super = UAnimInstance
	Super::NativeUpdateAnimation(DeltaSeconds);

	//���� ����Pawn�� GET�ϰڴ�. Try������ 100%�� �ƴ�.
	auto pawn = TryGetPawnOwner();

	//���� isValid�� ���� Get�Ͽ����� Check
	if (IsValid(pawn))
	{
		//�ӵ��� ũ�⸦ �޾ƿ�.
		Speed = pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(pawn);
		if (Character)
		{
			isFalling = Character->GetMovementComponent()->IsFalling();
		}
	}
}


