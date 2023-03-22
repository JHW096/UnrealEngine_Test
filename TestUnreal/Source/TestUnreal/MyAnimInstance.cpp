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
	//중복되지 않게
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super = UAnimInstance
	Super::NativeUpdateAnimation(DeltaSeconds);

	//나의 소유Pawn을 GET하겠다. Try임으로 100%는 아님.
	auto pawn = TryGetPawnOwner();

	//따라서 isValid를 통해 Get하였는지 Check
	if (IsValid(pawn))
	{
		//속도의 크기를 받아옴.
		Speed = pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(pawn);
		if (Character)
		{
			isFalling = Character->GetMovementComponent()->IsFalling();
		}
	}
}


