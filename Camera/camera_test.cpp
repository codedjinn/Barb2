#include <iostream>
#include <MathLib.h>
#include "Camera.h"

using namespace std;

MathLib::vector4 vector(1, 2, 3, 4);

int main(int argc, char** argv)
{
	CameraLib::Camera cam;

	cam.RotateXAxis(180.0f);
	const MathLib::vector4& ref	= cam.GetYAxis();

	cout << "ref  : " << ref.extractX() << ", "
					  << ref.extractY() << ", "
					  << ref.extractZ() << ", "
					  << ref.extractW() << endl;
	cout << "size : " << MathLib::vector4_magnitude(ref) << endl;
}
