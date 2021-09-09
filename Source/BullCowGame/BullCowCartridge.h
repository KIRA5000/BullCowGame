// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct BullCowCount
{
	int32 BullCount = 0;
	int32 CowCount = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;

	// Your declarations go below!
	private:
	int32 Lives, Level;
	bool bGameOver;
	FString HiddenWord;

	void SetUpGame();
	void EndGame();
	TArray<FString> GetValidWordList();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Guess) const;
	BullCowCount GetBullCowCount(const FString& Guess) const;
};
