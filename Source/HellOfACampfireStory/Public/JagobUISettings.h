#pragma once
#include "Engine/DeveloperSettings.h"
#include "JagobUISettings.generated.h"

/** UI-specific settings for the Tartbit plugin. */
UCLASS(ClassGroup = Jabobber, Config = Jagobber, DefaultConfig)
class HELLOFACAMPFIRESTORY_API UJagobUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UJagobUISettings(const FObjectInitializer& InitializerModule);

	/** Retrieves the settings. */
	static const ThisClass* Get()
	{
		return GetDefault<ThisClass>();
	}

	/** Retrieves the default viewport size. */
	FVector2D GetDefaultViewportSize() const { return DefaultViewportSize; }

	/** Retrieves the default viewport origin. */
	FVector2D GetDefaultViewportOrigin() const { return DefaultViewportOrigin; }

	/** Retrieves the default viewport background material. */
	TSoftObjectPtr<UMaterialInterface> GetDefaultViewportBackgroundMaterial() const { return DefaultViewportBackgroundMaterial; }

	/** Retrieves the default viewport area parameter name. */
	FName GetDefaultViewportAreaParameterName() const { return DefaultViewportAreaParameterName; }

	/** Retrieves the default target aspect ratio. */
	float GetDefaultTargetAspectRatio() const { return DefaultTargetAspectRatio; }
	
private:
	/** Default size of the viewport, mapped from 0-1. */
	UPROPERTY(Config, EditAnywhere, Category = "Viewport")
	FVector2D DefaultViewportSize = FVector2D(1.0f, 1.0f);

	/** Default origin of the viewport, where (0,0) is the upper left corner of the screen. */
	UPROPERTY(Config, EditAnywhere, Category = "Viewport")
	FVector2D DefaultViewportOrigin = FVector2D(0.0f, 0.0f);

	/** Default material to be used as the background for the viewport. */
	UPROPERTY(Config, EditAnywhere, Category = "Viewport")
	TSoftObjectPtr<UMaterialInterface> DefaultViewportBackgroundMaterial = nullptr;

	/** The default name of the parameter in the Viewport Background Material that represents the area of the viewport. */
	UPROPERTY(Config, EditAnywhere, Category = "Viewport")
	FName DefaultViewportAreaParameterName = NAME_None;

	/** The desired aspect ratio (e.g. 1.77 for 16:9). 0.0 means no constraint. */
	UPROPERTY(Config, EditAnywhere, Category = "Viewport")
	float DefaultTargetAspectRatio = 1.777778f;
};
