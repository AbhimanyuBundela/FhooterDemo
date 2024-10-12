// Fill out your copyright notice in the Description page of Project Settings.


#include "FhooterAnimInstance.h"
#include "FhooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Fhooter/Weapon/Weapon.h"
#include "Fhooter/FhooterTypes/CombatState.h"

void UFhooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	FhooterCharacter = Cast<AFhooterCharacter>(TryGetPawnOwner());
}

void UFhooterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (FhooterCharacter == nullptr)
	{
		FhooterCharacter = Cast<AFhooterCharacter>(TryGetPawnOwner());
	}
	if (FhooterCharacter == nullptr) return;

	FVector Velocity = FhooterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = FhooterCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = FhooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = FhooterCharacter->IsWeaponEquipped();
	EquippedWeapon = FhooterCharacter->GetEquippedWeapon();
	bIsCrouched = FhooterCharacter->bIsCrouched;
	bAiming = FhooterCharacter->IsAiming();
	TurningInPlace = FhooterCharacter->GetTurningInPlace();
	bRotateRootBone = FhooterCharacter->ShouldRotateRootBone();
	bElimmed = FhooterCharacter->IsElimmed();
	bHoldingTheFlag = FhooterCharacter->IsHoldingTheFlag();

	// Offset Yaw for Strafing
	FRotator AimRotation = FhooterCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(FhooterCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = FhooterCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = FhooterCharacter->GetAO_Yaw();
	AO_Pitch = FhooterCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && FhooterCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		FhooterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), LeftHandTransform.GetRotation(), OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}

	bUseFABRIK = FhooterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;
	bool bFABRIKOverride = FhooterCharacter->IsLocallyControlled() &&
		FhooterCharacter->GetCombatState() != ECombatState::ECS_ThrowingGrenade &&
		FhooterCharacter->bFinishedSwapping;
	if (bFABRIKOverride)
	{
		bUseFABRIK = !FhooterCharacter->IsLocallyReloading();
	}
	bUseAimOffsets = FhooterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !FhooterCharacter->GetDisableGameplay();
	bTransformRightHand = FhooterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !FhooterCharacter->GetDisableGameplay();
}
