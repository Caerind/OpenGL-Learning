#include "Application.hpp"

#include <cstdio>
#include <cstdlib>

int main()
{
	Application app;

	if (!app.init())
	{
		getchar();
		app.clear();
		return EXIT_FAILURE;
	}

	if (!app.run())
	{
		getchar();
		app.clear();
		return EXIT_FAILURE;
	}

	app.clear();

	return EXIT_SUCCESS;
}