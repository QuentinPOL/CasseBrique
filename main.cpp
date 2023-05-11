#include <SFML/Graphics.hpp>
#include "Ball.h"

int main(int argc, char **argv)
{
	// On créer les briques, une platerforme, une balle
	std::deque<Brick*> bricks;
	Player platform(550, 100, 11);
	Ball ball(200, 250, 10, 600);

	// On créer la fenêtre
	sf::RenderWindow window(sf::VideoMode(800, 600), "Casse Brique BY MCG");
	
	// initialiser les briques
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int x, y;

			// Pour la première brique de chaque ligne
			if (j == 0)
			{
				x = 0;
			}
			else
			{
				x = j * (80 + 2);
			}

			// Pour la première ligne de briques
			if (i == 0)
			{
				y = 50;
			}
			else
			{
				y = i * (25 + 2) + 50;
			}

			bricks.push_back(new Brick(x, y, 80, 25, 3));
		}
	}

	// On va définir "un rectangle" qui sera la fênetre
	sf::RectangleShape rdr2;
	rdr2.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	
	// On charge la police et le texte pour la vie
	sf::Text lifeText; // Déclaration de l'objet Texte
	sf::Font font; // Déclaration de l'objet police

	if (!font.loadFromFile("arial.ttf")) // chargement de la police
	{
		// gestion de l'erreur en cas de problème de chargement de police
		return EXIT_FAILURE;
	}

	lifeText.setFont(font); // On définit la police du texte
	lifeText.setCharacterSize(24); // On définit la taille du texte
	lifeText.setPosition(10, 10); // On définit la position du texte
	lifeText.setFillColor(sf::Color::White); // On définit la couleur du texte

	// Menu d'accueil
	sf::Text title("Casse Brique BY MCG", font, 64);
	title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);
	title.setPosition(window.getSize().x / 2.f, 100.f);
	sf::Text start("Cliquez ici pour jouer !", font, 32);
	start.setOrigin(start.getLocalBounds().width / 2.f, start.getLocalBounds().height / 2.f);
	start.setPosition(window.getSize().x / 2.f, 400.f);
	bool menuActive = true;

	// Menu de Loose
	sf::Text titleLoose("Vous avez perdu !!!", font, 64);
	titleLoose.setOrigin(titleLoose.getLocalBounds().width / 2.f, titleLoose.getLocalBounds().height / 2.f);
	titleLoose.setPosition(window.getSize().x / 2.f, 100.f);
	sf::Text end("Dommage il faut relancer le jeu ! :/", font, 32);
	end.setOrigin(end.getLocalBounds().width / 2.f, end.getLocalBounds().height / 2.f);
	end.setPosition(window.getSize().x / 2.f, 400.f);
	bool menuLoose = false;

	// Menu de Win
	sf::Text titleWin("Vous avez Gagner !!!", font, 64);
	titleWin.setOrigin(titleWin.getLocalBounds().width / 2.f, titleWin.getLocalBounds().height / 2.f);
	titleWin.setPosition(window.getSize().x / 2.f, 100.f);
	sf::Text endWin("Il faut relancer le jeu si vous souhaitez rejouer ! :/", font, 32);
	endWin.setOrigin(endWin.getLocalBounds().width / 2.f, endWin.getLocalBounds().height / 2.f);
	endWin.setPosition(window.getSize().x / 2.f, 400.f);
	bool menuWin = false;

	// On définie le temps, la positon souris et le temps écouler et la pos Y à ne pas dépasser par la balle par rapport à la platerforme
	sf::Clock clock;
	sf::Vector2i mousePos;
	float ellapsedTime = 0, posLineLoose = 550.f;
	int healthPlayer = 5;
	bool ballHasCrossedLine = false;

	lifeText.setString(std::string("Vies restantes : " + std::to_string(healthPlayer)));

	// on fait tourner le programme jusqu'à ce que la fenêtre soit fermée
	while (window.isOpen())
	{
		// On donne une valeur au temps écouler
		ellapsedTime = clock.restart().asSeconds();
		
		// on inspecte tous les évènements de la fenêtre qui ont été émis depuis la précédente itération
		sf::Event event;
		while (window.pollEvent(event))
		{
			// évènement "fermeture demandée" : on ferme la fenêtre
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left) 
				{
					if (menuActive) 
					{
						menuActive = false;
						// Démarrer le jeu
					}
				}
			}

			// Vérifiez si la souris est dans la fenêtre
			if (event.type == sf::Event::MouseMoved)
			{
				// Récupére la pos de la souris et on vérifie si elle est dans la fênetre
				mousePos = sf::Mouse::getPosition(window);
				if (mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < window.getSize().x && mousePos.y < window.getSize().y)
				{
					// si oui, on appel de la méthode handleMouse avec la position de la souris en paramètre
					platform.handleMouse(mousePos);
				}
			}
		}

		// Si le menu principal et plus activer et qu'il n'a pas perdu ou gagner
		if (menuActive != true && menuLoose != true && menuWin != true)
		{
			// On fait bouger la balle en fonction du temps et gérer les collisions avec les bords de la fênetre et la plateforme
			ball.move(ellapsedTime);
			ball.manageCollisionWith(window);
			ball.manageCollisionWithPlayerOrBrick(&platform, nullptr);

			// On vérifie si la balle dépasse la position Y à ne pas dépasser par rapport à la ligne
			if (ball.getPosition().y > posLineLoose)
			{
				if (!ballHasCrossedLine) // Si la balle vient de franchir la ligne, on enlève une vie
				{
					// Si la balle dépasse la position Y à ne pas dépasser, le joueur perd une vie s'il en reste
					if (healthPlayer > 0)
					{
						healthPlayer--;
						std::string lifeStr = "Vies restantes : " + std::to_string(healthPlayer);
						lifeText.setString(lifeStr); // On met à jour le texte du compteur de vies
						ballHasCrossedLine = true; // On indique que la balle a franchi la ligne
					}
					else
					{
						menuLoose = true;
						// Arrêter le jeu
					}
				}
			}
			else
			{
				ballHasCrossedLine = false; // Si la balle repasse au-dessus de la ligne, on remet ballHasCrossedLine à false
			}
		}
		
		// On va mettre à jour et donc clear
		window.clear();

		// On va joueur avec le fond et changer sa position
		rdr2.setFillColor(sf::Color(0, 0, 0));
		rdr2.setPosition(0, 0);

		// Ensuite Dessiner la fênêtre et le texte de vie
		window.draw(rdr2);

		// Si le menu accueil est actif
		if (menuActive == true)
		{
			window.draw(title);
			window.draw(start);
		}
		else if (menuLoose == true) // Sinon si le menu looser est actif
		{
			window.draw(titleLoose);
			window.draw(end);
		}
		else if (menuWin == true) // Sinon si le menu win est actif
		{
			window.draw(titleWin);
			window.draw(endWin);
		}
		else if (menuActive != true && menuLoose != true && menuWin != true) // Sinon si le menu d'accueil et menu de loose et menu win pas actif
		{
			// On affiche le compteur de vies
			window.draw(lifeText);

			// Dessiner et regarder si y'a collision avec les briques ainsi si elle sont en vies pour les détruires
			for (auto brick : bricks)
			{
				brick->draw(window);
				ball.manageCollisionWithPlayerOrBrick(nullptr, brick);

				if (!brick->isAlive()) // Si la brique n'a plus de vie
				{
					brick->destroyAndDelete(bricks);

					if (bricks.size() == 0) // Si il n'y à plus de brique
					{
						menuWin = true; // Win
						// Arrêter le jeu
					}
				}
			}

			// Dessiner la plateforme et la balle
			platform.draw(window);
			ball.draw(window);
		}

		// Et les afficher
		window.display();
	}
	return 0;
}