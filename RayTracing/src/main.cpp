#include <iostream>
#include <fstream>
#include <string>
#include "vec3.h"
#include "ray.h"


float HitSphere(const vec3& center, float radius, const Ray& r) 
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) 
	{
		return -1.0;
	}
	else
	{
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

vec3 color(const Ray& r) 
{
	float t = HitSphere(vec3(0, 0, -1), 0.5, r);
	if (t > 0.0) 
	{
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
		return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() 
{
	const int nx = 200;
	const int ny = 100;

	//Create ppm file
	std::fstream output_file;
	output_file.open("result.ppm", std::fstream::in | std::fstream::out | std::fstream::trunc);

	//Setup ppm header is necessary
	std::string ppm_header = "P3\n" + std::to_string(nx) + " " + std::to_string(ny) + "\n255\n";
	output_file << ppm_header;

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	for (int j = ny - 1; j >= 0; j--) 
	{
		for (int i = 0; i < nx; i++) 
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color(r);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";

			//Write color to ppm file
			std::string ppm_px = std::to_string(ir) + " " + std::to_string(ig) + " " + std::to_string(ib) + "\n";
			output_file << ppm_px;
		}
	}
	output_file.close();
}
