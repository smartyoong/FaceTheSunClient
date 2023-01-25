// Fill out your copyright notice in the Description page of Project Settings.


#include "BossKrakenAnimInstance.h"
#include "BossKraken.h"
#include "FaceTheSunGameState.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "BossKarakenAiContoroller.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "KrakenSmashActor.h"
#include "KrakenBeamSkillActor.h"
#include "KrakenProjectile.h"

void UBossKrakenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = Cast<ABossKraken>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		Character = Cast<ABossKraken>(Pawn);
		if (Character)
		{
			Direction = FVector::DotProduct(Character->GetActorRightVector(), Character->GetVelocity());
		}
	}
}

void UBossKrakenAnimInstance::AnimNotify_DieBoss()
{
	auto Pawn = Cast<ABossKraken>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		ABossKraken* Enemy = Cast<ABossKraken>(Pawn);
		if (Enemy)
		{
			Enemy->IsDead = true;
			Enemy->MultiStopBGMSound();
			auto mode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
			if(mode)
			{
				auto state = Cast<AFaceTheSunGameState>(mode->GameState);
				if (state)
				{
					state->bBossKilled = true;
				}
			}
		}
	}
}

void UBossKrakenAnimInstance::AnimNotify_HealBoss()
{
	if (HealSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HealSound, Character->GetActorLocation(), Character->GetActorRotation(), 0.3f);
	if (HealParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(HealParticle, Character->GetMesh(),TEXT("Heal"),Character->GetActorLocation(), Character->GetActorRotation(),Character->GetActorScale()*40.f);
	}
}

void UBossKrakenAnimInstance::AnimNotify_SmashBoss()
{
	if(Smashclass)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		GetWorld()->SpawnActor<AKrakenSmashActor>(Smashclass,Character->GetMesh()->GetSocketLocation(TEXT("Smash")),Character->GetActorRotation(),ActorSpawnParams);
	}
}

void UBossKrakenAnimInstance::AnimNotify_BeamBoss()
{
	if(BeamClass)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		auto a = GetWorld()->SpawnActor<AKrakenBeamSkillActor>(BeamClass,Character->GetMesh()->GetSocketLocation(TEXT("Beam")),Character->GetActorRotation(),ActorSpawnParams);
		a->AttachToActor(Character,FAttachmentTransformRules::KeepWorldTransform,TEXT("Beam"));
	}
}

void UBossKrakenAnimInstance::AnimNotify_HomingBoss()
{
	if (ProjectileClass != nullptr)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			for(int32 i=0; i <3; ++i)
			{
				FString s = FString::Printf(TEXT("Homing%d"),i);
				GetWorld()->SpawnActor<AKrakenProjectile>(ProjectileClass, Character->GetMesh()->GetSocketLocation(FName(*s)), Character->GetActorRotation(), ActorSpawnParams);
			}
		}
}