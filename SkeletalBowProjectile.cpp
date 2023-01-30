// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletalBowProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ASkeletalBowProjectile::ASkeletalBowProjectile()
{
    
    GetProjectileMovement()->InitialSpeed = 1500.f;
	GetProjectileMovement()->MaxSpeed = 1500.f;
	GetProjectileMovement()->bRotationFollowsVelocity = true;
	GetProjectileMovement()->bShouldBounce = false;
    SetLifeSpan(3.f);
}

void ASkeletalBowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != nullptr) && (OtherActor != this))
	{
		// Only add impulse and destroy projectile if we hit a physics
		auto EnemyActor = Cast<AFaceTheSunCharacter>(OtherActor);
		if (EnemyActor)
		{
			UGameplayStatics::ApplyDamage(EnemyActor, Damage, nullptr, this, UDamageType::StaticClass());
		}
		Destroy();
	}
}

void ASkeletalBowProjectile::BeginPlay()
{
    Super::BeginPlay();
    GetCollisionComp()->OnComponentHit.AddDynamic(this, &ASkeletalBowProjectile::OnHit);
}