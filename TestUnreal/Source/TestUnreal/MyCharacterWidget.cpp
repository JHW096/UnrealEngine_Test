// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterWidget.h"
#include "MyStatComponent.h"
#include "Components/ProgressBar.h"

void UMyCharacterWidget::BindHp(class UMyStatComponent* StatComp)
{
	//������ �ڱ� ����� ����� ���� ���
	//PB_HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	CurrentStatComp = StatComp;
	StatComp->OnHpChanged.AddUObject(this, &UMyCharacterWidget::UpdateHp);

	/*
		StatComponent���� HP�� �ٲ���� ��,
		�������� update�� ȣ��Ǿ� UI�� ���ŵ� ���̴�.
		��� �帧
	*/
}

void UMyCharacterWidget::UpdateHp()
{
	if (CurrentStatComp.IsValid())
	{
		PB_HpBar->SetPercent(CurrentStatComp->GetHpRatio());
	}
	
}