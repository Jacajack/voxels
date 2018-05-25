#include "scene.hpp"
#include "../lobor.hpp"

//Add new object to the scene
void lobor::Scene::add( lobor::Model *model )
{
	this->models.push_back( model );
}

//Whole scene render
void lobor::Scene::draw( )
{
	for ( lobor::Model *model : this->models )
	{
		model->draw( );
	}
}

//Scene constructor
lobor::Scene::Scene( )
{

}

//Scene destructor
lobor::Scene::~Scene( )
{

}