// Fill out your copyright notice in the Description page of Project Settings.


#include "KrakenProjectile.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunCharacter.h"
#include "CommonEnemy.h"

AKrakenProjectile::AKrakenProjectile()
{
    HomingEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
    HomingEffect->SetupAttachment(RootComponent);
    HomingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
    HomingSound->SetupAttachment(RootComponent);

    GetProjectileMovement()->InitialSpeed = 500.f;
	GetProjectileMovement()->MaxSpeed = 500.f;
	GetProjectileMovement()->bRotationFollowsVelocity = true;
	GetProjectileMovement()->bShouldBounce = false;
    GetProjectileMovement()->bIsHomingProjectile = true;
    GetProjectileMovement()->HomingAccelerationMagnitude = 10000.f;
}

void AKrakenProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		// Only add impulse and destroy projectile if we hit a physics
		auto EnemyActor = Cast<AFaceTheSunCharacter>(OtherActor);
		if (EnemyActor)
		{
			if ((OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			}
			UGameplayStatics::ApplyDamage(EnemyActor, Damage, nullptr, this, UDamageType::StaticClass());
		}
        UE_LOG(LogTemp,Log,TEXT("%s"),*(Hit.ToString()));
		Destroy();
	}
}

void AKrakenProjectile::BeginPlay()
{
    Super::BeginPlay();
    GetCollisionComp()->OnComponentHit.AddDynamic(this, &AKrakenProjectile::OnHit);
    // 랜덤한적 타케팅
    TArray<AFaceTheSunCharacter*> CharacterArray;
    TArray<AActor*> ActorArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),ActorArray);
    for(auto i : ActorArray)
    {
        AFaceTheSunCharacter* ch = Cast<AFaceTheSunCharacter>(i);
        if(ch)
            CharacterArray.Add(ch);
    }
    int32 j = CharacterArray.Num();
    int32 q = FMath::RandRange(0,j-1);
    if(CharacterArray.Num())
    {
        GetProjectileMovement()->HomingTargetComponent = CharacterArray[q]->GetCapsuleComponent();
    }

    FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
        Destroy();
	}), 5.f, false);
}