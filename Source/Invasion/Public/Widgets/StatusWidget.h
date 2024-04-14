#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
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
	void OnActiveSpellChanged(class USpell* NewAciveSpell);

protected:
	UPROPERTY(EditAnywhere, Meta = (BindWidget))
	TObjectPtr<UImage> SpellIcon;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
	TObjectPtr<UProgressBar> ManaBar;
};
