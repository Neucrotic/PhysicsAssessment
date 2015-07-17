#include <iostream> 
#include "TutNewtonsThirdLaw.h"
#include "TutCollisionDetection.h"

int main(int argc, char** argv)
{
	App* app = new TutCollisionDetection();
	app->Run();

	return 0;
}