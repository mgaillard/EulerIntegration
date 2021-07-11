#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

// Time step is one hour, it's voluntarily coarse 
const double delta_time = 3600.0;
// Simulation for one year
const int number_steps = 24*365;
const double gravity_constant = 6.674e-11;

using vec2 = glm::highp_dvec2;

struct Object
{
    std::string name;
    double mass;
    vec2 position;
    vec2 velocity;
    vec2 force;
};

const std::string METHOD_NAIVE = "naive";
const std::string METHOD_SYMPLECTIC = "symplectic";

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Give as argument to the program the integration method: naive or symplectic" << std::endl;
        return 1;
    }

    const std::string method = argv[1];

    if (method != METHOD_NAIVE && method != METHOD_SYMPLECTIC)
    {
        std::cerr << "The integration method can only be: naive or symplectic" << std::endl;
        return 1;
    }

    std::vector<Object> objects = {
        {"Earth", 5.9722e24, {0.0, 0.0}, {0.0, -12.5}},
        {"Moon", 7.342e22, {384405000.0, 0.0}, {0.0, 1022.0}},
    };
    
    for (int step = 0; step < number_steps; step++)
    {
        // Compute the gravity vector
        for (int i = 0; i < objects.size(); i++)
        {
            for (int j = i + 1; j < objects.size(); j++)
            {
                const auto vector = objects[j].position - objects[i].position;
                // Direction from i to j
                const auto direction = glm::normalize(vector);
                // Distance squared between i and j
                const auto distance_sq = glm::length2(vector);
                // Force
                const auto force = gravity_constant * (objects[i].mass * objects[j].mass) / distance_sq;
                
                objects[i].force = direction * force;
                objects[j].force = -objects[i].force;
            }
        }

        // Integration of the force for each object
        for (int i = 0; i < objects.size(); i++)
        {
            const auto acceleration = objects[i].force / objects[i].mass;
            
            const auto& old_velocity = objects[i].velocity;
            const auto new_velocity = old_velocity + acceleration * delta_time;

            // Integrate with different method
            vec2 new_position;
            if (method == METHOD_NAIVE)
            {
                new_position = objects[i].position + old_velocity * delta_time;
            }
            else if (method == METHOD_SYMPLECTIC)
            {
                new_position = objects[i].position + new_velocity * delta_time;
            }

            objects[i].velocity = new_velocity;
            objects[i].position = new_position;
        }

        // Show time, x, y for each object, distance earth-moon
        std::cout << step * delta_time << "\t"
                  << objects[0].position.x << "\t" << objects[0].position.y << "\t"
                  << objects[1].position.x << "\t" << objects[1].position.y << "\t"
                  << glm::distance(objects[0].position, objects[1].position) << std::endl;
    }

    return 0;
}