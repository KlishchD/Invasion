#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "StatusWidget.generated.h"

UCLASS()
class INVASION_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void OnHealthChanged(float Health);
	void OnManaChanged(float Mana);

protected:
	UPROPERTY(EditAnywhere, Meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
	TObjectPtr<UProgressBar> ManaBar;
};
