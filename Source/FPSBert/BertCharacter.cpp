// Fill out your copyright notice in the Description page of Project Settings.

#include "BertCharacter.h"


// Sets default values
ABertCharacter::ABertCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    // Create a first person camera component.
    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    // Attach the camera component to our capsule component.
    FPSCameraComponent->SetupAttachment((USceneComponent*)GetCapsuleComponent());
    
    // Position the camera slightly above the eyes.
    FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
    // Allow the pawn to control camera rotation.
    FPSCameraComponent->bUsePawnControlRotation = true;

    // Create a first person mesh component for the owning player.
    FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    // Only the owning player sees this mesh.
    FPSMesh->SetOnlyOwnerSee(true);
    // Attach the FPS mesh to the FPS camera.
    FPSMesh->SetupAttachment(FPSCameraComponent);
    // Disable some environmental shadowing to preserve the illusion of having a single mesh.
    FPSMesh->bCastDynamicShadow = false;
    FPSMesh->CastShadow = false;
    
    // The owning player doesn't see the regular (third-person) body mesh.
    GetMesh()->SetOwnerNoSee(true);

}

// Called when the game starts or when spawned
void ABertCharacter::BeginPlay()
{
	Super::BeginPlay();
    if (GEngine)
    {
        // Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
    }
	
}

// Called every frame
void ABertCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABertCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Set up "movement" bindings.
    PlayerInputComponent->BindAxis("MoveForward", this, &ABertCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ABertCharacter::MoveRight);
    // Set up "look" bindings.
    PlayerInputComponent->BindAxis("Turn", this, &ABertCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &ABertCharacter::AddControllerPitchInput);
    // Set up "action" bindings.
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABertCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABertCharacter::StopJump);
}

void ABertCharacter::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void ABertCharacter::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void ABertCharacter::StartJump()
{
    bPressedJump = true;
}

void ABertCharacter::StopJump()
{
    bPressedJump = false;
}
