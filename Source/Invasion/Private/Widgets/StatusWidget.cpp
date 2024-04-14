#include "Widgets/StatusWidget.h"
#include "Invasion/InvasionCharacter.h"

void UStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AInvasionCharacter* Character = GetOwningPlayerPawn<AInvasionCharacter>();
	Character->GetOnHealthChnaged().AddUObject(this, &ThisClass::OnHealthChanged);
	Character->GetOnManaChanged().AddUObject(this, &ThisClass::OnManaChanged);
	Character->GetSpellComponent()->GetOnActiveSpellChanged().AddUObject(this, &ThisClass::OnActiveSpellChanged);

	SpellIcon->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Icon"), Character->GetSpellComponent()->GetActiveSpell()->Icon);
	HealthBar->SetPercent(Character->GetHealthNormalized());
	ManaBar->SetPercent(Character->GetManaNormalized());
}

void UStatusWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (AInvasionCharacter* Character = GetOwningPlayerPawn<AInvasionCharacter>())
	{
		Character->GetOnHealthChnaged().RemoveAll(this);
		Character->GetOnManaChanged().RemoveAll(this);
	}
}

void UStatusWidget::OnHealthChanged(float Health)
{
	AInvasionCharacter* Character = GetOwningPlayerPawn<AInvasionCharacter>();
	HealthBar->SetPercent(Character->GetHealthNormalized());
}

void UStatusWidget::OnManaChanged(float Mana)
{
	AInvasionCharacter* Character = GetOwningPlayerPawn<AInvasionCharacter>();
	ManaBar->SetPercent(Character->GetManaNormalized());
}

void UStatusWidget::OnActiveSpellChanged(USpell* NewAciveSpell)
{
	SpellIcon->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Icon"), NewAciveSpell->Icon);
}
