#include "Game.h"
#include "Box2DHelper.h"

Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		CheckCollitions();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close();
			break;
		
		}
	}
}

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// MyContactListener* l= new MyContactListener();
	// phyWorld->SetContactListener(l);
	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// Creamos la cabeza
	b2Body* cabeza = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	cabeza->SetTransform(b2Vec2(50.0f, 40.0f), 0.0f);
	//Creamos el cuerpo
	b2Body* cuerpo = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 8, 17, 10.0f, 0.5, 0.1f);
	cuerpo->SetTransform(b2Vec2(50.0f, 55.0f), 0.0f);
	//Conectamos la cabeza con el cuerpo
	Box2DHelper::CreateDistanceJoint(phyWorld, cabeza, cabeza->GetWorldCenter() + b2Vec2(0.0f, 5.0f),
		cuerpo, cuerpo->GetWorldCenter() - b2Vec2(0.0f, 5.0f), 0.0f, 10.0f, 0.1f);

	//Creamos el femur izquierdo y lo conectamos al cuerpo
	b2Body* femurIzq = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4, 9, 0.7f, 0.5, 0.1f);
	femurIzq->SetTransform(b2Vec2(46.0f, 69.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, cuerpo, cuerpo->GetWorldCenter() + b2Vec2(-4.0f, 8.0f),
		femurIzq, femurIzq->GetWorldCenter() - b2Vec2(0.0f, 5.0f), 0.0f, 10.0f, 0.1f);

	//Creamos el femur derecho y lo conectamos al cuerpo
	b2Body* femurDer = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4, 9, 0.7f, 0.5, 0.1f);
	femurDer->SetTransform(b2Vec2(54.0f, 69.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, cuerpo, cuerpo->GetWorldCenter() + b2Vec2(4.0f, 8.0f),
		femurDer, femurDer->GetWorldCenter() - b2Vec2(0.0f, 5.0f), 0.0f, 10.0f, 0.1f);

	//Creamos la pierna izquierda y la conectamos al femur izquierdo
	b2Body* piernaIzq = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4, 8, 0.6f, 0.5, 0.1f);
	piernaIzq->SetTransform(b2Vec2(46.0f, 79.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, femurIzq, femurIzq->GetWorldCenter() + b2Vec2(0.0f, 5.0f),
		piernaIzq, piernaIzq->GetWorldCenter() - b2Vec2(0.0f, 3.0f), 0.0f, 10.0f, 0.1f);

	//Creamos la pierna derecha y la conectamos al femur derecho
	b2Body* piernaDer = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4, 8, 0.6f, 0.5, 0.1f);
	piernaDer->SetTransform(b2Vec2(54.0f, 79.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, femurDer, femurDer->GetWorldCenter() + b2Vec2(0.0f, 5.0f),
		piernaDer, piernaDer->GetWorldCenter() - b2Vec2(0.0f, 3.0f), 0.0f, 10.0f, 0.1f);

	//Creamos el antebrazo izquierdo y lo conectamos al cuerpo
	b2Body* anteIzq = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 7, 0.5f, 0.5f, 0.1f);
	anteIzq->SetTransform(b2Vec2(44.0f, 50.0f), 10.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, cuerpo, cuerpo->GetWorldCenter() - b2Vec2(4.0f, 8.0f),
		anteIzq, anteIzq->GetWorldCenter() + b2Vec2(3.0f, -4.0f), 0.0f, 10.0f, 0.1f);

	//Creamos el antebrazo derecho y lo conectamos al cuerpo
	b2Body* anteDer = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 7, 0.5f, 0.5f, 0.1f);
	anteDer->SetTransform(b2Vec2(56.0f, 50.0f), -10.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, cuerpo, cuerpo->GetWorldCenter() + b2Vec2(4.0f, -8.0f),
		anteDer, anteDer->GetWorldCenter() + b2Vec2(-3.0f, -4.0f), 0.0f, 10.0f, 0.1f);

	//Creamos el brazo izquierdo y lo conectamos al antebrazo izquierdo
	b2Body* brazoIzq = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 8, 0.3f, 0.5f, 0.1f);
	brazoIzq->SetTransform(b2Vec2(44.0f, 57.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, anteIzq, anteIzq->GetWorldCenter() + b2Vec2(0.0f, 4.0f),
		brazoIzq, brazoIzq->GetWorldCenter() + b2Vec2(0.0f, -3.0f), 0.0f, 10.0f, 0.1f);

	//Creamos el brazo derecho y lo conectamos al antebrazo derecho
	b2Body* brazoDer = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 8, 0.3f, 0.5f, 0.1f);
	brazoDer->SetTransform(b2Vec2(56.0f, 57.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, anteDer, anteDer->GetWorldCenter() + b2Vec2(0.0f, 4.0f),
		brazoDer, brazoDer->GetWorldCenter() + b2Vec2(0.0f, -3.0f), 0.0f, 10.0f, 0.1f);

	//Creamos la mano izquierda y la conectamos al brazo izquierdo
	b2Body* manoIzq = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2, 1.0f, 0.1f, 0.1f);
	manoIzq->SetTransform(b2Vec2(44.0f, 64.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, brazoIzq, brazoIzq->GetWorldCenter() + b2Vec2(0.0f, 3.0f),
		manoIzq, manoIzq->GetWorldCenter() + b2Vec2(0.0f, -2.0f), 0.0f, 10.0f, 0.1f);

	//Creamos la mano derecha y la conectamos al brazo derecho
	b2Body* manoDer = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2, 0.1f, 0.5f, 0.1f);
	manoDer->SetTransform(b2Vec2(56.0f, 64.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, brazoDer, brazoDer->GetWorldCenter() + b2Vec2(0.0f, 3.0f),
		manoDer, manoDer->GetWorldCenter() + b2Vec2(0.0f, -2.0f), 0.0f, 10.0f, 0.1f);

	//Creamos el pie izquierdo y lo conectamos a la pierna izquierda
	b2Body* pieIzq = Box2DHelper::CreateTriangularDynamicBody(phyWorld, b2Vec2(0.0f, 0.0f), 5.0f, 0.1f, 0.5f, 0.1f);
	pieIzq->SetTransform(b2Vec2(46.0f, 84.0f), 30.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, piernaIzq, piernaIzq->GetWorldCenter() + b2Vec2(0.0f, 4.0f),
		pieIzq, pieIzq->GetWorldCenter() + b2Vec2(1.0f, 0.0f), 0.0f, 10.0f, 0.1f);

	//Creamos el pie derecho y lo conectamos a la pierna derecha
	b2Body* pieDer = Box2DHelper::CreateTriangularDynamicBody(phyWorld, b2Vec2(0.0f, 0.0f), 5.0f, 0.1f, 0.5f, 0.1f);
	pieDer->SetTransform(b2Vec2(54.0f, 84.0f), -30.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, piernaDer, piernaDer->GetWorldCenter() + b2Vec2(0.0f, 4.0f),
		pieDer, pieDer->GetWorldCenter() + b2Vec2(1.0f, 0.0f), 0.0f, 10.0f, 0.1f);
	
}

Game::~Game(void)
{
}