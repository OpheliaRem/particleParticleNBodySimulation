#include "Particle.h"
#include "Vector.h"
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

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


Particle* InitializeNBodySystem(const std::string path, int& n);

double Cube(double number);

Vector Sum(Vector* sequence, int size);

int main()
{
	int n;
	double timeStep = 0.01;

	Particle* particles = InitializeNBodySystem("Particles.txt", n);

	Vector** force = new Vector*[n];
	for (int i = 0; i < n; ++i)
		force[i] = new Vector[n];


	std::filesystem::path path = L"coordinates";
	if (std::filesystem::exists(path)) 
	{
		std::filesystem::remove_all(path);
	}

	if (!std::filesystem::create_directory(path))
	{
		std::cout << "Error making a directory\n";
		return 1;
	}

	double time = 0.0;
	for (;;)
	{
		std::ofstream fileCoordinates;
		std::string timeStr = std::to_string(time);
		fileCoordinates.open("coordinates\\" + timeStr + ".csv");
		fileCoordinates << "x;y;z\n";


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
			fileCoordinates << particles[i].position << std::endl;

			particles[i].acceleration = Sum(force[i], n) / particles[i].mass;

			particles[i].velocity = particles[i].velocity + particles[i].acceleration * timeStep;

			particles[i].position = particles[i].position + particles[i].velocity * timeStep;
		}

		time += timeStep;
		fileCoordinates.close();
	}


	for (int i = 0; i < n; ++i)
		delete[] force[i];
	delete[] force;
	delete[] particles;
	return 0;
}

Particle* InitializeNBodySystem(const std::string path, int& n)
{
	std::ifstream fileParticles;
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
	sum.z = .0;

	for (int i = 0; i < size; ++i)
	{
		sum = sum + sequence[i];
	}

	return sum;
}
