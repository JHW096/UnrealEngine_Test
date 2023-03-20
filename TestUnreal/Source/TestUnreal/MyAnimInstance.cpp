// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

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
	}
}
