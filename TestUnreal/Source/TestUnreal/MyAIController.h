// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREAL_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AMyAIController();

	//������ �� ȣ��Ǵ� �Լ�
	virtual void OnPossess(APawn* InPawn) override;
	
	//���ǰ� ������ �� �Լ�
	virtual void OnUnPossess() override;

private:

	void RandomMove();

private:

	FTimerHandle TimerHandle;
};
