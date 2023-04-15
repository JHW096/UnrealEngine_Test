// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterWidget.h"
#include "MyStatComponent.h"
#include "Components/ProgressBar.h"

void UMyCharacterWidget::BindHp(class UMyStatComponent* StatComp)
{
	//변수를 자기 맘대로 만들고 싶은 경우
	//PB_HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	CurrentStatComp = StatComp;
	StatComp->OnHpChanged.AddUObject(this, &UMyCharacterWidget::UpdateHp);

	/*
		StatComponent에서 HP가 바뀌었을 때,
		연동해준 update가 호출되어 UI가 갱신될 것이다.
		라는 흐름
	*/
}

void UMyCharacterWidget::UpdateHp()
{
	if (CurrentStatComp.IsValid())
	{
		PB_HpBar->SetPercent(CurrentStatComp->GetHpRatio());
	}
	
}