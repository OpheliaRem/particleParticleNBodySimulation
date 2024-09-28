#include "Particle.h"
#include "Vector.h"
#include <fstream>
#include <vector>
using namespace std;

/*Решение задачи N-тел методом Particle-Particle (прямого интегрирования).
моделируется трёхмерное пространство,
обезразмеривание осуществлено с соображением G = 1,
уравнения движения решаются методом Эйлера.
используется симметрично-противоположная матрица гравитационных взаимодействий force,
благодаря которой количество необходимых вычислений уменьшается в два раза,
в соответствии с третьим законом Ньютона (Fij = -Fji)*/

/*Solution of N-body problem with Particle-Particle (direct sum) method.
This program calculates in 3D space,
with nondimensialization (G = 1),
equations of motion are solved with Euler's method.
Forces Fij and Fji are treated as equal with different signs, due to Newton's 3rd law. That halves the amount of calculations.*/


Particle* InitializeNBodySystem(const string path, int& n);

double Cube(double number);

Vector Sum(Vector* sequence, int size);

int main()
{
	ofstream fileCoordinates;
	fileCoordinates.open("Coordinates.txt");

	int n;
	//double timeStep = 3600 * 24;
	double timeStep = 0.01;

	bool isConvertingFileNeeded = true;

	Particle* particles = InitializeNBodySystem("Particles.txt", n);

	Vector** force = new Vector * [n];
	for (int i = 0; i < n; ++i)
		force[i] = new Vector[n];

	double time = 0.0;
	for (;;)
	{
		fileCoordinates << "Moment: " << time << endl;


		for (int i = 0; i < n; ++i)
		{
			force[i][i].SetZeroVector();
		}

		for (int i = 0; i < n; ++i)
		{
			for (int j = i + 1; j < n; ++j)
			{
				Vector distance = particles[j].position - particles[i].position;
				force[i][j] = distance * particles[i].mass * particles[j].mass / Cube(distance.Abs());
				force[j][i] = -force[i][j];
			}
		}

		
		for (int i = 0; i < n; ++i)
		{
			fileCoordinates << "Position of " << i << ": ";
			fileCoordinates << particles[i].position << endl;

			particles[i].acceleration = Sum(force[i], n) / particles[i].mass;

			particles[i].velocity = particles[i].velocity + particles[i].acceleration * timeStep;

			particles[i].position = particles[i].position + particles[i].velocity * timeStep;
		}
		fileCoordinates << endl;
		fileCoordinates << endl;
		fileCoordinates << endl;
		fileCoordinates << endl;

		time += timeStep;
	}


	for (int i = 0; i < n; ++i)
		delete[] force[i];
	delete[] force;
	delete[] particles;
	fileCoordinates.close();
	return 0;
}

Particle* InitializeNBodySystem(const string path, int& n)
{
	ifstream fileParticles;
	fileParticles.open(path);

	char tempString[256];
	fileParticles.getline(tempString, 256, ':');

	fileParticles >> n;
	Particle* particles = new Particle[n];

	fileParticles.get();
	fileParticles.get();

	fileParticles.getline(tempString, 256);

	for (int i = 0; i < n; ++i)
	{
		fileParticles >> particles[i].mass;
		fileParticles.get();
		fileParticles >> particles[i].velocity.x >> particles[i].velocity.y >> particles[i].velocity.z;
		fileParticles.get();
		fileParticles >> particles[i].position.x >> particles[i].position.y >> particles[i].position.z;
	}

	fileParticles.close();
	return particles;
}

double Cube(double number)
{
	return number * number * number;
}

Vector Sum(Vector* sequence, int size)
{
	Vector sum;
	sum.x = .0;
	sum.y = .0;

	for (int i = 0; i < size; ++i)
	{
		sum = sum + sequence[i];
	}

	return sum;
}
