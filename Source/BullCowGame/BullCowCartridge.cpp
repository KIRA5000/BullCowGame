// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWords.h"

TArray<FString> UBullCowCartridge::GetValidWordList()
{
    TArray<FString> ValidWords;

    for (int32 i = 0; i < Words.Num(); i++)
    {
        if ((Words[i].Len() == (Level + 3)) && (IsIsogram(Words[i])))
        {
            ValidWords.Emplace(Words[i]);
        }
    }

    return ValidWords;
}

void UBullCowCartridge::SetUpGame()
{
    auto ValidWords = GetValidWordList();

    bGameOver = false;
    HiddenWord = ValidWords[FMath::RandRange(0, ValidWords.Num() - 1)];
    Lives = HiddenWord.Len();

    // Welcoming Player
    PrintLine(TEXT("Level: %i"), Level);
    PrintLine(TEXT("Isogram is a %i letter word."), HiddenWord.Len()); //Magic Number Remove
    PrintLine(TEXT("Remember you have %i lives."), Lives);
    PrintLine(TEXT("Hidden Word was: %s"), *HiddenWord); // Debug Line

    PrintLine(TEXT("Please enter a word:"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Hidden Word was: %s"), *HiddenWord);
    PrintLine(TEXT("Press Enter to restart the Game ..."));
}

bool UBullCowCartridge::IsIsogram(const FString& Guess) const
{
    TArray<TCHAR> Chars;
    auto NewGuess = Guess.ToLower();

    for (auto i = 0; i < NewGuess.Len(); i++)
    {
        auto ch = NewGuess[i];

        if (Chars.Contains(ch))
        {
            return false;
        }
        else
        {
            Chars.Push(ch);
        }
    }

    return true;
}

BullCowCount UBullCowCartridge::GetBullCowCount(const FString& Guess) const
{
    BullCowCount Count;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            Count.BullCount++;
        }
        else
        {
            for (int32 j = 0; j < Guess.Len(); j++)
            {
                if (i == j)
                {
                    continue;
                }

                if (Guess[i] == HiddenWord[j])
                {
                    Count.CowCount++;
                    break;
                }
            }
        }
    }

    return Count;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    // Checking User Input
    if (HiddenWord == Guess)
    {
        PrintLine(TEXT("Your word: ") + Guess + TEXT(" is correct"));
        PrintLine(TEXT("You win!"));

        Level = (Level + 1)%3;

        if (Level == 0)
        {
            PrintLine(TEXT("Congrats you completed all levels."));
            PrintLine(TEXT("Restarting from level one."));

            Level = 1;
        }

        EndGame();
    }
    else
    {
        auto Count = GetBullCowCount(Guess);

        PrintLine(TEXT("You got %i Bulls and %i Cows."), Count.BullCount, Count.CowCount);
        PrintLine(TEXT("Your word: ") + Guess + TEXT(" is incorrect"));

        if (Guess.Len() == HiddenWord.Len())
        {
            if (IsIsogram(Guess))
            {
                Lives--;
            }
        }
        else
        {
            PrintLine(TEXT("Your word is not Valid."));
            PrintLine(TEXT("Please enter a word of size " + FString::FromInt(HiddenWord.Len())));
        }

        // Check if User have exhausted his/her lives
        if (Lives == 0)
        {
            PrintLine(TEXT("The word was: ") + HiddenWord);
            PrintLine(TEXT("You lose!"));

            EndGame();
        }
        else
        {
            PrintLine(TEXT("Remaining lives: " + FString::FromInt(Lives)));
        }
    }
}

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Level = 1;

    PrintLine(TEXT("Welcome to BullCow Game"));
    // Setting Up Game
    SetUpGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetUpGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}