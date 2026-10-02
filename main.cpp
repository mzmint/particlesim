#include <SFML/Graphics.hpp>
#include <random>

struct Particle {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float lt;
};

int main() {

    int partType = 0;

    sf::Clock spawnClock;
    sf::Clock deltaClock;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist1(-20, 20);
    std::uniform_int_distribution<int> dist2(0, 255);
    std::uniform_int_distribution<int> dist3(0, 64);
    std::uniform_int_distribution<int> dist4(-50, 0);
    std::uniform_int_distribution<int> dist5(20, 100);
    std::uniform_int_distribution<int> dist6(-15, 15);

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

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButton->button == sf::Mouse::Button::Right)
                {
                    partType++;

                    if (partType > 1)
                        partType = 0;
                }
            }
        }

        if (partType == 0) {
            st = 40;
        } else if (partType == 1) {
            st = 20;
        }

        float dt = deltaClock.restart().asSeconds();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (spawnClock.getElapsedTime().asMilliseconds() >= st) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                Particle particle;

                particle.shape.setSize({5.f, 5.f});
                if (partType == 0) {
                    particle.shape.setFillColor(sf::Color(255, dist2(gen) , 0));
                } else if (partType == 1) {
                    particle.shape.setFillColor(sf::Color(dist3(gen), 0 , 255));
                }

                int randomX = dist1(gen);
                int randomY = dist1(gen);

                particle.shape.setPosition({
                    mousePos.x + static_cast<float>(randomX),
                    mousePos.y + static_cast<float>(randomY) - 10.f
                });

                if (partType == 0) {
                    particle.velocity = {
                        static_cast<float>(dist6(gen)),
                        static_cast<float>(dist4(gen))
                    };
                } else if (partType == 1) {
                    particle.velocity = {
                        static_cast<float>(dist6(gen)),
                        static_cast<float>(dist5(gen))
                    };
                }

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