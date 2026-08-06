#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "JagobViewport.generated.h"

UCLASS()
class HELLOFACAMPFIRESTORY_API UJagobViewport : public UGameViewportClient
{
GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnViewportAreaRefreshed, const FVector2D&, Size, const FVector2D&, Origin);

public:
	virtual void Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice) override;
	virtual void NotifyPlayerAdded(int32 PlayerIndex, ULocalPlayer* AddedPlayer) override;
	virtual void Draw(FViewport* InViewport, FCanvas* SceneCanvas) override;
	virtual void UpdateActiveSplitscreenType() override;

	/**
	 * Sets the background material for the viewport.
	 * 
	 * @Param NewMaterial The new material to be used as the background for the viewport.
	 * @Param ViewportAreaParameterName The name of the parameter in the background material that represents the area of the player viewport (if it is smaller than the game viewport).
	 */
	UFUNCTION(BlueprintCallable, Category = "Tart|Viewport", Meta = (AutoCreateRefTerm = "ViewportAreaParameterName"))
	void SetBackgroundMaterial(UMaterialInterface* NewMaterial, const FName& NewViewportAreaMaterialParameter = NAME_None);

	/**
	 * Sets the size and origin of the viewport.
	 *
	 * @Param NewSize The new size of the viewport, mapped from 0-1.
	 * @Param NewOrigin The new origin of the viewport, where (0,0) is the upper left corner of the screen.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tart|Viewport", Meta = (AutoCreateRefTerm = "NewSize,NewOrigin"))
	void SetViewportAreaBounds(const FVector2D& NewSize, const FVector2D& NewOrigin);

	/**
	 * Gets the size and origin of the viewport.
	 * 
	 * @Param Size The size of the viewport, mapped from 0-1.
	 * @Param Origin The origin of the viewport, where (0,0) is the upper left corner of the screen.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tart|Viewport")
	void GetViewportAreaBounds(FVector2D& Size, FVector2D& Origin) const;

	/** Event that is called when the viewport area is refreshed. */
	UPROPERTY(BlueprintAssignable, Category = "Tart|Viewport")
	FOnViewportAreaRefreshed OnViewportAreaRefreshed;

	/** Gets the target aspect ratio. */
	UFUNCTION(BlueprintCallable, Category = "Tart|Viewport")
	float GetTargetAspectRatio() const;

	/** Sets the target aspect ratio. */
	UFUNCTION(BlueprintCallable, Category = "Tart|Viewport")
	void SetTargetAspectRatio(float NewAspectRatio);

private:
	/** Refreshes the viewport area. */
	void RefreshViewportArea();

	UPROPERTY()
	TObjectPtr<UMaterialInterface> BackgroundMaterial = nullptr;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CachedBackgroundMaterial = nullptr;

	/** Size of the player's viewport, mapped from 0-1. Scaled to be potentially smaller than the actual game viewport. */
	FVector2D ScaledViewportSize = FVector2D(1.0f, 1.0f);

	/** Origin of the viewport, where (0,0) is the upper left corner of the screen. */
	FVector2D ViewportOrigin = FVector2D(0.0f, 0.0f);

	/** The name of the parameter in the Viewport Background Material that represents the area of the viewport. */
	FName ViewportAreaMaterialParameter = NAME_None;
	
	float TargetAspectRatio = 1.777778f;
};
