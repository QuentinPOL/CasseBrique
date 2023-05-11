#include <SFML/Graphics.hpp>
#include "Ball.h"

int main(int argc, char **argv)
{
	// On cr�er les briques, une platerforme, une balle
	std::deque<Brick*> bricks;
	Player platform(550, 100, 11);
	Ball ball(200, 250, 10, 600);

	// On cr�er la fen�tre
	sf::RenderWindow window(sf::VideoMode(800, 600), "Casse Brique BY MCG");
	
	// initialiser les briques
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int x, y;

			// Pour la premi�re brique de chaque ligne
			if (j == 0)
			{
				x = 0;
			}
			else
			{
				x = j * (80 + 2);
			}

			// Pour la premi�re ligne de briques
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

	// On va d�finir "un rectangle" qui sera la f�netre
	sf::RectangleShape rdr2;
	rdr2.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	
	// On charge la police et le texte pour la vie
	sf::Text lifeText; // D�claration de l'objet Texte
	sf::Font font; // D�claration de l'objet police

	if (!font.loadFromFile("arial.ttf")) // chargement de la police
	{
		// gestion de l'erreur en cas de probl�me de chargement de police
		return EXIT_FAILURE;
	}

	lifeText.setFont(font); // On d�finit la police du texte
	lifeText.setCharacterSize(24); // On d�finit la taille du texte
	lifeText.setPosition(10, 10); // On d�finit la position du texte
	lifeText.setFillColor(sf::Color::White); // On d�finit la couleur du texte

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

	// On d�finie le temps, la positon souris et le temps �couler et la pos Y � ne pas d�passer par la balle par rapport � la platerforme
	sf::Clock clock;
	sf::Vector2i mousePos;
	float ellapsedTime = 0, posLineLoose = 550.f;
	int healthPlayer = 5;
	bool ballHasCrossedLine = false;

	lifeText.setString(std::string("Vies restantes : " + std::to_string(healthPlayer)));

	// on fait tourner le programme jusqu'� ce que la fen�tre soit ferm�e
	while (window.isOpen())
	{
		// On donne une valeur au temps �couler
		ellapsedTime = clock.restart().asSeconds();
		
		// on inspecte tous les �v�nements de la fen�tre qui ont �t� �mis depuis la pr�c�dente it�ration
		sf::Event event;
		while (window.pollEvent(event))
		{
			// �v�nement "fermeture demand�e" : on ferme la fen�tre
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left) 
				{
					if (menuActive) 
					{
						menuActive = false;
						// D�marrer le jeu
					}
				}
			}

			// V�rifiez si la souris est dans la fen�tre
			if (event.type == sf::Event::MouseMoved)
			{
				// R�cup�re la pos de la souris et on v�rifie si elle est dans la f�netre
				mousePos = sf::Mouse::getPosition(window);
				if (mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < window.getSize().x && mousePos.y < window.getSize().y)
				{
					// si oui, on appel de la m�thode handleMouse avec la position de la souris en param�tre
					platform.handleMouse(mousePos);
				}
			}
		}

		// Si le menu principal et plus activer et qu'il n'a pas perdu ou gagner
		if (menuActive != true && menuLoose != true && menuWin != true)
		{
			// On fait bouger la balle en fonction du temps et g�rer les collisions avec les bords de la f�netre et la plateforme
			ball.move(ellapsedTime);
			ball.manageCollisionWith(window);
			ball.manageCollisionWithPlayerOrBrick(&platform, nullptr);

			// On v�rifie si la balle d�passe la position Y � ne pas d�passer par rapport � la ligne
			if (ball.getPosition().y > posLineLoose)
			{
				if (!ballHasCrossedLine) // Si la balle vient de franchir la ligne, on enl�ve une vie
				{
					// Si la balle d�passe la position Y � ne pas d�passer, le joueur perd une vie s'il en reste
					if (healthPlayer > 0)
					{
						healthPlayer--;
						std::string lifeStr = "Vies restantes : " + std::to_string(healthPlayer);
						lifeText.setString(lifeStr); // On met � jour le texte du compteur de vies
						ballHasCrossedLine = true; // On indique que la balle a franchi la ligne
					}
					else
					{
						menuLoose = true;
						// Arr�ter le jeu
					}
				}
			}
			else
			{
				ballHasCrossedLine = false; // Si la balle repasse au-dessus de la ligne, on remet ballHasCrossedLine � false
			}
		}
		
		// On va mettre � jour et donc clear
		window.clear();

		// On va joueur avec le fond et changer sa position
		rdr2.setFillColor(sf::Color(0, 0, 0));
		rdr2.setPosition(0, 0);

		// Ensuite Dessiner la f�n�tre et le texte de vie
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

			// Dessiner et regarder si y'a collision avec les briques ainsi si elle sont en vies pour les d�truires
			for (auto brick : bricks)
			{
				brick->draw(window);
				ball.manageCollisionWithPlayerOrBrick(nullptr, brick);

				if (!brick->isAlive()) // Si la brique n'a plus de vie
				{
					brick->destroyAndDelete(bricks);

					if (bricks.size() == 0) // Si il n'y � plus de brique
					{
						menuWin = true; // Win
						// Arr�ter le jeu
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