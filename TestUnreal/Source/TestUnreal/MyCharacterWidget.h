// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREAL_API UMyCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void BindHp(class UMyStatComponent* StatComp);

	void UpdateHp();

private:

	TWeakObjectPtr<class UMyStatComponent> CurrentStatComp;
	
	//BluePrint ProgressBar와 같은 이름
	//툴상으로 만든 바와 변수의 이름을 같게하여 맵핑이 되게끔 유도 (meta=BindWidget)
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PB_HpBar;
};
