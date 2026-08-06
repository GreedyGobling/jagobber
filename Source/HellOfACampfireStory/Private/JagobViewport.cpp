#include "Public/JagobViewport.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "JagobUISettings.h"
#include "Materials/MaterialInstanceDynamic.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(JagobViewport)

void UJagobViewport::Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
		
	if (const UJagobUISettings* UISettings = UJagobUISettings::Get())
	{
		if (TObjectPtr<UMaterialInterface> LoadedBackgroundMaterial = UISettings->GetDefaultViewportBackgroundMaterial().LoadSynchronous())
		{
			SetBackgroundMaterial(LoadedBackgroundMaterial, UISettings->GetDefaultViewportAreaParameterName());
		}
		else
		{
			SetBackgroundMaterial(nullptr, UISettings->GetDefaultViewportAreaParameterName());
		}
	}
	
	IConsoleManager::Get().FindConsoleVariable(TEXT("r.SecondaryScreenPercentage.GameViewport"))->Set(100);
	IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"))->Set(100);
}

void UJagobViewport::NotifyPlayerAdded(int32 PlayerIndex, class ULocalPlayer* AddedPlayer)
{
	Super::NotifyPlayerAdded(PlayerIndex, AddedPlayer);
	
	if (const UJagobUISettings* UISettings = UJagobUISettings::Get())
	{
		SetViewportAreaBounds(UISettings->GetDefaultViewportSize(), UISettings->GetDefaultViewportOrigin());
		SetTargetAspectRatio(UISettings->GetDefaultTargetAspectRatio());
	}
}

void UJagobViewport::Draw(FViewport* InViewport, FCanvas* SceneCanvas)
{
	Super::Draw(Viewport, SceneCanvas);

	if (CachedBackgroundMaterial)
	{
		const FVector2D GameViewportSize(InViewport->GetSizeXY());

		// Draw a full-screen quad with the background material - since the material will cover the entire viewport,
		// it will have to define a transparent area for the player to view the game world.
		FCanvasTileItem BackgroundTileItem(FVector2D(0, 0), CachedBackgroundMaterial->GetRenderProxy(), GameViewportSize);
		BackgroundTileItem.BlendMode = SE_BLEND_Opaque;
		SceneCanvas->DrawItem(BackgroundTileItem);
	}
}

void UJagobViewport::UpdateActiveSplitscreenType()
{
	Super::UpdateActiveSplitscreenType();
	RefreshViewportArea();
}


void UJagobViewport::SetBackgroundMaterial(UMaterialInterface* NewMaterial, const FName& NewViewportAreaMaterialParameter)
{
	if (NewMaterial != BackgroundMaterial)
	{
		BackgroundMaterial = NewMaterial;
		CachedBackgroundMaterial = BackgroundMaterial ? UMaterialInstanceDynamic::Create(BackgroundMaterial, this) : nullptr;
	}
	
	ViewportAreaMaterialParameter = NewViewportAreaMaterialParameter;

	RefreshViewportArea();
}

void UJagobViewport::SetViewportAreaBounds(const FVector2D& Size, const FVector2D& Origin)
{
	ScaledViewportSize = Size;
	ViewportOrigin = Origin;
	
	RefreshViewportArea();
}

void UJagobViewport::GetViewportAreaBounds(FVector2D& Size, FVector2D& Origin) const
{
	Size = ScaledViewportSize;
	Origin = ViewportOrigin;
}

void UJagobViewport::RefreshViewportArea()
{
	FVector2D FinalViewportSize = ScaledViewportSize;
    FVector2D FinalViewportOrigin = ViewportOrigin;
 
    if (TargetAspectRatio > 0.0f)
    {
        FVector2D ViewportSize;
    	GetViewportSize(ViewportSize);
        
        if (ViewportSize.X > 0 && ViewportSize.Y > 0)
        {
            const float AspectRatio = ViewportSize.X / ViewportSize.Y;
            if (AspectRatio > TargetAspectRatio)
            {
                const float AdjustedWidth = (ScaledViewportSize.Y * ViewportSize.Y * TargetAspectRatio) / ViewportSize.X;
            	
                FinalViewportSize.X = AdjustedWidth;
                FinalViewportOrigin.X = ViewportOrigin.X + (ScaledViewportSize.X - AdjustedWidth) * 0.5f;
            }
            else if (AspectRatio < TargetAspectRatio)
            {
            	const float AdjustedHeight = (ScaledViewportSize.X * ViewportSize.X / TargetAspectRatio) / ViewportSize.Y;
            	
                FinalViewportSize.Y = AdjustedHeight;
                FinalViewportOrigin.Y = ViewportOrigin.Y + (ScaledViewportSize.Y - AdjustedHeight) * 0.5f;
            }
        }
    }
 
    // Apply the calculated elimination of excess space to the PlayerData
    const TArray<ULocalPlayer*>& PlayerList = GetOuterUEngine()->GetGamePlayers(this);
    for (int32 Index = 0; Index < PlayerList.Num(); ++Index)
    {
        if (Index < SplitscreenInfo[ESplitScreenType::None].PlayerData.Num())
        {
            SplitscreenInfo[ESplitScreenType::None].PlayerData[Index].SizeX = FinalViewportSize.X;
            SplitscreenInfo[ESplitScreenType::None].PlayerData[Index].SizeY = FinalViewportSize.Y;
            SplitscreenInfo[ESplitScreenType::None].PlayerData[Index].OriginX = FinalViewportOrigin.X;
            SplitscreenInfo[ESplitScreenType::None].PlayerData[Index].OriginY = FinalViewportOrigin.Y;
        }
    }
 
    // Update the dynamic material so the background mask matches the new constrained area
    if (CachedBackgroundMaterial && ViewportAreaMaterialParameter != NAME_None)
    {
        CachedBackgroundMaterial->SetVectorParameterValue(ViewportAreaMaterialParameter, 
            FVector4(FinalViewportSize, FinalViewportOrigin));
    }
    
    OnViewportAreaRefreshed.Broadcast(FinalViewportSize, FinalViewportOrigin);
}

float UJagobViewport::GetTargetAspectRatio() const
{
	return TargetAspectRatio;
}

void UJagobViewport::SetTargetAspectRatio(float NewAspectRatio)
{
	TargetAspectRatio = NewAspectRatio;
	RefreshViewportArea();
}
