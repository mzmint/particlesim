#include <SFML/Graphics.hpp>
#include <random>

struct Particle {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float lt;
};

int main() {

    sf::Clock spawnClock;
    sf::Clock deltaClock;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist1(-20, 20);
    std::uniform_int_distribution<int> dist2(0, 255);

    sf::RenderWindow window(
        sf::VideoMode({1920, 1080}),
        "ParticleSim"
    );

    std::vector<Particle> particles;

    int st = 40;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = deltaClock.restart().asSeconds();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (spawnClock.getElapsedTime().asMilliseconds() >= st) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                Particle particle;

                particle.shape.setSize({5.f, 5.f});
                particle.shape.setFillColor(sf::Color(255, dist2(gen) , 0));

                int randomX = dist1(gen);
                int randomY = dist1(gen);

                particle.shape.setPosition({
                    mousePos.x + static_cast<float>(randomX),
                    mousePos.y + static_cast<float>(randomY) - 10.f
                });

                particle.velocity = {
                    static_cast<float>(dist1(gen)),
                    static_cast<float>(dist1(gen))
                };

                particle.lt = 1.f;
                particles.push_back(particle);

                spawnClock.restart();
            }
        }

        for (auto& particle : particles) {
            particle.shape.move(particle.velocity * dt);
            particle.lt -= dt;
        }

        std::erase_if(particles, [](const Particle& particle) {
            return particle.lt <= 0;
        });



        window.clear();

        for (auto& particle : particles) {
            window.draw(particle.shape);
        }
        window.display();
    }

    return 0;
}