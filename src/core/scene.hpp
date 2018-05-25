#ifndef IFRIT_SCENE_HPP
#define IFRIT_SCENE_HPP

#include <vector>
#include "model.hpp"

/*
	Note: most of this needs recoding
*/

namespace ifrit
{
	//The whole scene meant for drawing
	class Scene
	{
		protected:
			std::vector <ifrit::Model*> models;

		public:
			void add( ifrit::Model *model );
			void draw( );

			//Constructor and destructor
			Scene( );
			~Scene( );
	};
};

#endif