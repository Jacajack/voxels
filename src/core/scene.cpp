#include "scene.hpp"
#include "../ifrit.hpp"

//Add new object to the scene
void ifrit::Scene::addObject( ifrit::Model *model )
{
	this->models.push_back( model );
}

//Whole scene render
void ifrit::Scene::draw( )
{
	for ( ifrit::Model *model : this->models )
	{
		model->draw( );
	}
}

//Scene constructor
ifrit::Scene::Scene( )
{

}

//Scene destructor
ifrit::Scene::~Scene( )
{

}