// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

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
	}
}
