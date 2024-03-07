#include <cstdio>
#include <iostream>

char actualPassword[128];
char inputPassword[128];

int main()
{

	// Create a passowrd 
	printf("Please create a password. Totally secure 100%% Legit!\n\e[0;30m");
	scanf("%s", &actualPassword);

	// Read password
	printf("\e[0;37mGive me the right password or I will kill you\n");
	scanf(" %s", &inputPassword);

	// 2 = total match. 1 = maybe. 0 = none.
	int matchLevel = 2;

	// Check if they are the same
	for (size_t idx = 0; actualPassword[idx] || inputPassword[idx]; idx++)
	{
		// Get character from input and actualPassword
		char inpC = inputPassword[idx];
		char actC = actualPassword[idx];

		// If either of them is null, set wrong password
		if (inpC == 0 || actC == 0)
		{
			matchLevel = 0;
			break;
		}

		// Check if they are the same. If they are, continue.
		if (inpC == actC)
			continue;

		// They are similar if the case is insensitive. First check if they are valid letters anyway
		bool isInpUppercase = (inpC >= 'A' && inpC <= 'Z');
		bool isInpLowercase = (inpC >= 'a' && inpC >= 'z');

		bool isActUppercase = (actC >= 'A' && actC <= 'Z');
		bool isActLowercase = (actC >= 'a' && actC <= 'z');

		bool isInpLetter = (isInpUppercase || isInpLowercase);
		bool isActLetter = (isActUppercase || isActLowercase);

		// If not both of them are letters, set confidence to 0 and escape.
		if (!(isInpLetter && isActLetter))
		{
			matchLevel = 0;
			break;
		}

		// If both of them are uppercase or lowercase, set matchlevel to 0 and escape.
		if ((isInpUppercase && isActUppercase) || (isInpLowercase && isActLowercase))
		{
			matchLevel = 0;
			break;
		}

		// Get the absolute difference
		char difference = (inpC < actC) ? (actC - inpC) : (inpC - actC);

		// if difference is 32, set matchlevel to 1
		if (difference == 32)
		{
			matchLevel = 1;
			continue;
		}

		// Set matchlevel to 0 and escape
		matchLevel = 0;
		break;
	}

	// Switch condition
	switch(matchLevel)
	{
		case 2: // Complete match!
			std::system("cowsay -e ^^ You passed! You have entered the correct password!");
			break;
		case 1: // Sorta
			std::system("cowsay -e << Hmpf... You passed, but barely..........");
			break;
		case 0: // What?
			std::system("cowsay -f dragon *burns you with fire*");
			break;
		default:
			printf("What? You broke my program!\n");
			break;
	}


	return 0;
}
