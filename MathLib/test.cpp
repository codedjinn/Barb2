/**
* This work is licensed under the Creative Commons Attribution 3.0 Unported License.
* To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons,
* 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#include <stdio.h>
#include "MathLib.h"
#include "Profiler.h"

const uint32_t iterations	= 100000;

namespace VectorTests
{
const uint32_t numVectors	= 10000;
MathLib::vector4 vectorArray1[numVectors];
MathLib::vector4 vectorArray2[numVectors];
MathLib::vector4 vectorArrayResults[numVectors];
float floatArray[numVectors];

void initializeVectors()
{
	for (uint32_t i = 0; i < numVectors; i++)
	{
		float multiplierX	= rand() > (RAND_MAX / 2) ? 1.0f : -1.0f;
		float multiplierY	= rand() > (RAND_MAX / 2) ? 1.0f : -1.0f;
		float multiplierZ	= rand() > (RAND_MAX / 2) ? 1.0f : -1.0f;
		float multiplierW	= rand() > (RAND_MAX / 2) ? 1.0f : -1.0f;

		vectorArray1[i].setXYZW((float)rand() * multiplierX,
							    (float)rand() * multiplierY,
							    (float)rand() * multiplierZ,
							    (float)rand() * multiplierW);
	}

	for (uint32_t i = 0; i < numVectors; i++)
	{
		float multiplierX	= rand() > (RAND_MAX / 2) ? 1.0f : -1.0f;
		float multiplierY	= rand() > (RAND_MAX / 2) ? 1.0f : -1.0f;
		float multiplierZ	= rand() > (RAND_MAX / 2) ? 1.0f : -1.0f;
		float multiplierW	= rand() > (RAND_MAX / 2) ? 1.0f : -1.0f;

		vectorArray2[i].setXYZW((float)rand() * multiplierX,
							    (float)rand() * multiplierY,
							    (float)rand() * multiplierZ,
							    (float)rand() * multiplierW);
	}
}

void vector4_print(const MathLib::vector4& vector)
{
	printf("vector at [%p]:\n", &vector);
	printf("x : %.4f\n", vector.extractX());
	printf("y : %.4f\n", vector.extractY());
	printf("z : %.4f\n", vector.extractZ());
	printf("w : %.4f\n", vector.extractW());
	printf("\n");
}

void vector4_addTest(const MathLib::vector4& vector1, const MathLib::vector4& vector2, MathLib::vector4& result)
{
	MathLib::vector4_add(vector1, vector2, result);
}

void vector4_subTest(const MathLib::vector4& vector1, const MathLib::vector4& vector2, MathLib::vector4& result)
{
	MathLib::vector4_sub(vector1, vector2, result);
}

void vector4_copyTest(MathLib::vector4& dest, const MathLib::vector4& src)
{
	MathLib::vector4_copy(dest, src);
}

void vector4_setToVectorTest(MathLib::vector4& vector)
{
	MathLib::vector4_setToVector(vector);
}

void vector4_setToPointTest(MathLib::vector4& vector)
{
	MathLib::vector4_setToPoint(vector);
}

void vector4_scaleTest(const MathLib::vector4& vector, float scaleFactor, MathLib::vector4& result)
{
	MathLib::vector4_scale(vector, scaleFactor, result);
}

void vector4_addScaledVectorTest(const MathLib::vector4& vector1, const MathLib::vector4& vector2, float mulFactor, MathLib::vector4& result)
{
	MathLib::vector4_addScaledVector(vector1, vector2, mulFactor, result);
}

float vector4_magnitudeTest(const MathLib::vector4& vector)
{
	return (MathLib::vector4_magnitude(vector));
}

void vector4_componentProductTest(const MathLib::vector4& v1, const MathLib::vector4& v2, MathLib::vector4& r)
{
	MathLib::vector4_componentProduct(v1, v2, r);
}

void vector4_crossProductTest(const MathLib::vector4& v1, const MathLib::vector4& v2, MathLib::vector4& r)
{
	MathLib::vector4_crossProduct(v1, v2, r);
}

void vector4_normalizeTest(MathLib::vector4& vector)
{
	MathLib::vector4_normalize(vector);
}

float vector4_distanceTest(const MathLib::vector4& vector1, const MathLib::vector4& vector2)
{
	return (MathLib::vector4_distance(vector1, vector2));
}

void vector4_negateTest(MathLib::vector4& vector)
{
	return (MathLib::vector4_negate(vector, vector));
}

void vector4_addBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		MathLib::vector4_add(vectorArray1[i], vectorArray2[i], vectorArrayResults[i]);
}

void vector4_subBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		MathLib::vector4_sub(vectorArray1[i], vectorArray2[i], vectorArrayResults[i]);
}

void vector4_scaleBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		MathLib::vector4_scale(vectorArray1[i], floatArray[i], vectorArrayResults[i]);
}

void vector4_addScaledVectorBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		MathLib::vector4_addScaledVector(vectorArray1[i], vectorArray2[i], floatArray[i], vectorArrayResults[i]);
}

void vector4_magnitudeBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		floatArray[i]	= MathLib::vector4_magnitude(vectorArray1[i]);
}

void vector4_componentProductBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		MathLib::vector4_componentProduct(vectorArray1[i], vectorArray2[i], vectorArrayResults[i]);
}

void vector4_dotProductBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		floatArray[i]	= MathLib::vector4_dotProduct(vectorArray1[i], vectorArray2[i]);
}

void vector4_crossProductBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		MathLib::vector4_crossProduct(vectorArray1[i], vectorArray2[i], vectorArrayResults[i]);
}

void vector4_normalizeBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		MathLib::vector4_normalize(vectorArrayResults[i]);
}

void vector4_distanceBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		floatArray[i]	= MathLib::vector4_distance(vectorArray1[i], vectorArray2[i]);
}

void vector4_negateBenchmark()
{
	for (uint32_t i = 0; i < numVectors; i++)
		MathLib::vector4_negate(vectorArray1[i], vectorArrayResults[i]);
}

void vectorTests()
{
	printf("Running benchmark!\n");
	printf("Vector array sizes\t: %u\n", numVectors);
	printf("Number of iterations\t: %u\n", iterations);
	printf("----------------------------------------\n");

	// Run addition benchmark:
	{
		printf("Addition benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_addBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run subtraction benchmark:
	{
		printf("Subtraction benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_subBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run scale benchmark:
	{
		printf("Scale benchmark\t\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_scaleBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run addScaledVector benchmark:
	{
		printf("Add scaled vector benchmark\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_addScaledVectorBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run magnitude benchmark:
	{
		printf("Magnitude benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_magnitudeBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run component product benchmark:
	{
		printf("Component product benchmark\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_componentProductBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run dot product benchmark:
	{
		printf("Dot product benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_dotProductBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run cross product benchmark:
	{
		printf("Cross product benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_crossProductBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run normalize benchmark:
	{
		printf("Normalize benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_normalizeBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run distance benchmark:
	{
		printf("Distance benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_distanceBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run negate benchmark:
	{
		printf("Negate benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			vector4_negateBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}
}

}

namespace MatrixTests
{

const uint32_t numMatrices	= 10000;
MathLib::matrix4x4 matrixArray1[numMatrices];
MathLib::matrix4x4 matrixArray2[numMatrices];
MathLib::matrix4x4 matrixArrayResults[numMatrices];

float floatArray[numMatrices];

MathLib::vector4 vectorArray[numMatrices];
MathLib::vector4 vectorResultArray[numMatrices];

void matrix4x4_print(const MathLib::matrix4x4& matrix)
{
	printf("matrix at [%p]:\n", &matrix);
	printf("[%.4f][%.4f][%.4f][%.4f]\n", matrix._00, matrix._01, matrix._02, matrix._03);
	printf("[%.4f][%.4f][%.4f][%.4f]\n", matrix._10, matrix._11, matrix._12, matrix._13);
	printf("[%.4f][%.4f][%.4f][%.4f]\n", matrix._20, matrix._21, matrix._22, matrix._23);
	printf("[%.4f][%.4f][%.4f][%.4f]\n", matrix._30, matrix._31, matrix._32, matrix._33);
	printf("\n");
}

void matrix4x4_addTest(const MathLib::matrix4x4& matrix1, const MathLib::matrix4x4& matrix2, MathLib::matrix4x4& result)
{
	MathLib::matrix4x4_add(matrix1, matrix2, result);
}

void matrix4x4_subTest(const MathLib::matrix4x4& matrix1, const MathLib::matrix4x4& matrix2, MathLib::matrix4x4& result)
{
	MathLib::matrix4x4_sub(matrix1, matrix2, result);
}

void matrix4x4_scalarMulTest(const MathLib::matrix4x4& matrix, float scalar, MathLib::matrix4x4& result)
{
	MathLib::matrix4x4_scalarMul(matrix, scalar, result);
}

void matrix4x4_mulTest(const MathLib::matrix4x4& matrix1, const MathLib::matrix4x4& matrix2, MathLib::matrix4x4& result)
{
	MathLib::matrix4x4_mul(matrix1, matrix2, result);
}

void matrix4x4_transposeTest(MathLib::matrix4x4& matrix)
{
	MathLib::matrix4x4_transpose(matrix);
}

void matrix4x4_vectorMulTest(const MathLib::matrix4x4& matrix, const MathLib::vector4& vector, MathLib::vector4& result)
{
	MathLib::matrix4x4_vectorMul(matrix, vector, result);
}

void matrix4x4_vectorBatchMulTest(const MathLib::matrix4x4& matrix, MathLib::vector4 const * const vectorArray, uint32_t numVectors, MathLib::vector4* resultArray)
{
	MathLib::matrix4x4_vectorBatchMul(matrix, vectorArray, numVectors, resultArray);
}

void matrix4x4_addBenchmark()
{
	for (uint32_t i = 0; i < numMatrices; i++)
		MathLib::matrix4x4_add(matrixArray1[i], matrixArray2[i], matrixArrayResults[i]);
}

void matrix4x4_subBenchmark()
{
	for (uint32_t i = 0 ; i < numMatrices; i++)
		MathLib::matrix4x4_sub(matrixArray1[i], matrixArray2[i], matrixArrayResults[i]);
}

void matrix4x4_transposeBenchmark()
{
	for (uint32_t i = 0; i < numMatrices; i++)
		MathLib::matrix4x4_transpose(matrixArray1[i]);
}

void matrix4x4_scalarMulBenchmark()
{
	for (uint32_t i = 0 ; i < numMatrices; i++)
		MathLib::matrix4x4_scalarMul(matrixArray1[i], floatArray[i], matrixArrayResults[i]);
}

void matrix4x4_matrixMulBenchmark()
{
	for (uint32_t i = 0; i < numMatrices; i++)
		MathLib::matrix4x4_mul(matrixArray1[i], matrixArray2[i], matrixArrayResults[i]);
}

void matrix4x4_vectorMulBenchmark()
{
	for (uint32_t i = 0; i < numMatrices; i++)
		MathLib::matrix4x4_vectorMul(matrixArray1[i], vectorArray[i], vectorResultArray[i]);
}

void matrix4x4_vectorBatchMulBenchmark()
{
	MathLib::matrix4x4_vectorBatchMul(matrixArray1[0], vectorArray, numMatrices, vectorResultArray);
}

void matrixTests()
{
	printf("Running benchmark!\n");
	printf("Matrix array sizes\t: %u\n", numMatrices);
	printf("Number of iterations\t: %u\n", iterations);
	printf("----------------------------------------\n");

	// Run addition benchmark:
	{
		printf("Addition benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			matrix4x4_addBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run subtraction benchmark:
	{
		printf("Subtraction benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			matrix4x4_subBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run transposition benchmark:
	{
		printf("Transposition benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			matrix4x4_transposeBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run scalar mul benchmark:
	{
		printf("Scalar mul benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			matrix4x4_scalarMulBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run matrix mul benchmark:
	{
		printf("Matrix mul benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			matrix4x4_matrixMulBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run vector mul benchmark:
	{
		printf("Vector mul benchmark\t\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			matrix4x4_vectorMulBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}

	// Run vector batch mul benchmark:
	{
		printf("Vector batch mul benchmark\t: ");

		float totalTime	= 0.0f;
		Profiler profiler;

		for (uint32_t i = 0; i < iterations; i++)
		{
			profiler.start();

			matrix4x4_vectorBatchMulBenchmark();

			profiler.stop();

			totalTime += profiler.getTimeMilliseconds();
		}

		printf("average time (msecs) [%4.6f]\n", totalTime / (float)iterations);
	}
}

}

int main()
{
	Profiler::initializePerformanceCounters();

	VectorTests::initializeVectors();

    printf("MathLib test program.\n");

    MathLib::initMathLib();
    printf("%s\n", MathLib::getInfoString());
    printf("\n");

	//vectorTests();

	MatrixTests::matrixTests();

    return (0);
}
