// Fill out your copyright notice in the Description page of Project Settings.


#include "LichSkillActor.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FaceTheSunCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ALichSkillActor::ALichSkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LichComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Colliison"));
	RootComponent = LichComp;
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Effect->SetupAttachment(RootComponent);
	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	Sound->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALichSkillActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle SkillHandle;
	LichComp->OnComponentBeginOverlap.AddDynamic(this,&ALichSkillActor::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(SkillHandle,FTimerDelegate::CreateLambda([&]()
{
	Destroy(); // 지속시간후 파괴
}), 1.f, false);
}

// Called every frame
void ALichSkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ALichSkillActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFaceTheSunCharacter* ch = Cast<AFaceTheSunCharacter>(OtherActor);
	if(ch)
	{
		UGameplayStatics::ApplyDamage(ch,30.f,nullptr,nullptr,nullptr);
	}
}
