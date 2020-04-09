#pragma once

#include "engine/prehistoric/core/util/Includes.hpp"
#include "engine/CoreEngine.h"
#include "engine/prehistoric/core/util/time/Timer.h"

int main()
{
	int N = pow(10, 1);

	{
		float* a = new float[N * 4];
		float* b = new float[N * 4];
		float* c = new float[N * 4];

		memset(a, 1, N * 4 * sizeof(float));
		memset(b, 1, N * 4 * sizeof(float));

		float maxError = 0.0f;

		Timer t("No SIMD");

		for (int i = 0; i < N; i++)
		{
			c[i * 4 + 0] = a[i * 4 + 0] + b[i * 4 + 0];
			c[i * 4 + 1] = a[i * 4 + 1] + b[i * 4 + 1];
			c[i * 4 + 2] = a[i * 4 + 2] + b[i * 4 + 2];
			c[i * 4 + 3] = a[i * 4 + 3] + b[i * 4 + 3];
		}

		/*for (int i = 0; i < N * 4; i++)
		{
			float err = c[i] - 3.0f;
			if (err > maxError)
			{
				maxError = err;
			}
		}

		PR_LOG_MESSAGE("Max error: %f\n", maxError);*/

		delete[] a;
		delete[] b;
		delete[] c;
	}

	{
		__m128* a = new __m128[N];
		__m128* b = new __m128[N];
		__m128* c = new __m128[N];

		memset(a, 1, N * sizeof(__m128));
		memset(b, 1, N * sizeof(__m128));

		__m128 maxError = { 0.0f };

		Timer t("SIMD");

		for (int i = 0; i < N; i++)
		{
			c[i] = _mm_add_ps(a[i], b[i]);
		}

		/*for (int i = 0; i < N; i++)
		{
			__m128 err = _mm_cmpeq_ps(c[i], { 3.0f });
			maxError = _mm_cmpgt_ps(c[i], maxError);
		}

		PR_LOG_MESSAGE("Max error: %f\n", maxError);*/

		delete[] a;
		delete[] b;
		delete[] c;
	}
	
	//CoreEngine engine;
	//engine.Start();

	return 0;
}