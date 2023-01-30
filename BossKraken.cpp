// Fill out your copyright notice in the Description page of Project Settings.


#include "BossKraken.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "BossKrakenAnimInstance.h"

ABossKraken::ABossKraken()
{
    CurrentPlaySound = CreateDefaultSubobject<UAudioComponent>(TEXT("audio"));
}
void ABossKraken::BeginPlay()
{
    Super::BeginPlay();
    StartHP = HP;
}

void ABossKraken::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABossKraken, StartHP);
}

void ABossKraken::MultiHPUnder30_Implementation()
{

    if(CheerUpSound)
         CurrentPlaySound->SetSound(CheerUpSound);
    MultiPlayBGMSound();
}

void ABossKraken::MultiHeal_Implementation()
{
    if(HealAnim)
    {
        auto PE =Cast<UBossKrakenAnimInstance>(GetMesh()->GetAnimInstance());
		if (PE)
		{
			PE->StopAllMontages(0.f);
        }
        GetMesh()->GetAnimInstance()->Montage_Play(HealAnim);
        if(HP > 0)
            HP += HealHP;
    }
}

void ABossKraken::MultiStopBGMSound_Implementation()
{
    if(CurrentPlaySound)
    {
        if(CurrentPlaySound->IsPlaying())
            CurrentPlaySound->FadeOut(3.f, 0.f);
    }
}

void ABossKraken::MultiPlayBGMSound_Implementation()
{
    if(CurrentPlaySound)
    {
        CurrentPlaySound->FadeIn(5.f,0.8f);
    }
}

void ABossKraken::MultiBeam_Implementation()
{
    if(BeamAnim)
    {
        auto PE =Cast<UBossKrakenAnimInstance>(GetMesh()->GetAnimInstance());
		if (PE)
		{
			PE->StopAllMontages(0.f);
            PE->Montage_Play(BeamAnim);
        }
    }
}
void ABossKraken::MultiSmash_Implementation()
{
    auto PE =Cast<UBossKrakenAnimInstance>(GetMesh()->GetAnimInstance());
	if (PE)
	{
		PE->StopAllMontages(0.f);
        PE->Montage_Play(SmashAnim);
     }
}
void ABossKraken::MultiProjectile_Implementation()
{
     auto PE =Cast<UBossKrakenAnimInstance>(GetMesh()->GetAnimInstance());
	if (PE)
	{
		PE->StopAllMontages(0.f);
        PE->Montage_Play(HomingAnim);
    }
}

void ABossKraken::StartBossGame()
{
    SetActorLocation(FVector(-92410.0f,-95010.f,600.f));
    if(ScreamBossSound)
        UGameplayStatics::PlaySound2D(GetWorld(),ScreamBossSound);
    if(InitStartSound)
    {
         CurrentPlaySound->SetSound(InitStartSound);
        MultiPlayBGMSound();
    }
}