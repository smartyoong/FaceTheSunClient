// Fill out your copyright notice in the Description page of Project Settings.


#include "KrakenBeamSkillActor.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FaceTheSunCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AKrakenBeamSkillActor::AKrakenBeamSkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BeamComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = BeamComp;
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effect->SetupAttachment(RootComponent);
	Sound= CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	Sound->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKrakenBeamSkillActor::BeginPlay()
{
	Super::BeginPlay();
	BeamComp->OnComponentBeginOverlap.AddDynamic(this,&AKrakenBeamSkillActor::OnOverlapBegin);
	BeamComp->OnComponentEndOverlap.AddDynamic(this,&AKrakenBeamSkillActor::OnOverlapEnd);
	GetWorld()->GetTimerManager().SetTimer(SkillHandle,FTimerDelegate::CreateLambda([&]()
{
	Destroy(); // 지속시간후 파괴
}), SkillTime, false);
}

void AKrakenBeamSkillActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFaceTheSunCharacter* ch = Cast<AFaceTheSunCharacter>(OtherActor);
	if(ch)
	{
		DamageCharacters.Add(ch);
	}
}

void AKrakenBeamSkillActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFaceTheSunCharacter* ch = Cast<AFaceTheSunCharacter>(OtherActor);
	if(ch)
	{
		DamageCharacters.Remove(ch);
	}
}

void AKrakenBeamSkillActor::Tick(float DeltaTime)
{
	for(int32 i =0; i < DamageCharacters.Num(); ++i)
	{
		UGameplayStatics::ApplyDamage(DamageCharacters[i], .5f,nullptr,nullptr,nullptr);
	}
}