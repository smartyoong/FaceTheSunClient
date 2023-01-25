// Fill out your copyright notice in the Description page of Project Settings.


#include "KrakenGoToAirSkill.h"
#include "FaceTheSunCharacter.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"


void UKrakenGoToAirSkill::SmashBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AFaceTheSunCharacter* Character = Cast<AFaceTheSunCharacter>(OtherActor);
	if(Character != nullptr)
	{
		Character->LaunchCharacter(FVector(0.f,0.f,1000.f),false,false);
        UGameplayStatics::ApplyDamage(Character, 30.f,nullptr,nullptr,nullptr);
        OnComponentBeginOverlap.RemoveAll(this);
	}
}
void UKrakenGoToAirSkill::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle WaitHandle;
    OnComponentBeginOverlap.AddDynamic(this, &UKrakenGoToAirSkill::SmashBeginOverlap);
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
        IsFinish = true;
	}), 2.f, false);
}