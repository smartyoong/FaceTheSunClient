// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "FaceTheSunPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunCharacter.h"
#include "TP_WeaponComponent.h"

void UHUDUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	PC = Cast<AFaceTheSunPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDUserWidget::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
	Super::NativeTick(Geometry, DeltaSeconds);
	if (PC)
	{
		auto FC = Cast<AFaceTheSunCharacter>(PC->GetPawn());
		if (FC)
		{
			float CurrHP = FC->HP / 100.f;
			int32 TotalAmmo = FC->Gun1P->TotalAmmo;
			int32 CurrAmmo = FC->Gun1P->AmmoCount;
			PB_HP->SetPercent(CurrHP);
			TB_Ammo->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrAmmo, TotalAmmo)));
		}
	}
}
