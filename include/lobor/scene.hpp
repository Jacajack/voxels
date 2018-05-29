#ifndef LOBOR_SCENE_HPP
#define LOBOR_SCENE_HPP

#include <vector>
#include "model.hpp"
#include "lobor.hpp"

/*
	Note: most of this needs recoding
*/

namespace lobor
{
	//The whole scene meant for drawing
	class Scene
	{
		protected:
			std::vector <lobor::Model*> models;

		public:
			void add( lobor::Model *model );
			void draw( );

			//Constructor and destructor
			Scene( );
			~Scene( );
	};
};

#endif