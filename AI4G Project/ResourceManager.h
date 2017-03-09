
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

/* A utility class for retrieving resources */
class ResourceManager
{
	typedef unsigned __int64				Hash;

	typedef std::map <Hash, sf::Image *>	ImageStore;
	typedef std::map <Hash, sf::Sound *>	SoundStore;
	typedef std::map <Hash, sf::Music *>	MusicStore;


								// Image used if an image resource could not be found
	sf::Image					m_NotFoundImage;

								// Internal record of all resources loaded so far (resources that have already
								//	been loaded are returned from here instead of loading them from disk again)
	ImageStore					m_ImageCache;
	SoundStore					m_SoundCache;
	MusicStore					m_MusicCache;

	std::string					TranslateResourceName (std::string oldName);

	Hash						GetResourceHash (std::string resourceName);

public:
								// The public instance of the ResourceManager that all components of the game
								//	can access
	static ResourceManager *	Context;

								ResourceManager ();

	bool						ResourceExists (std::string resourceName);

	void						GetResource (std::string resourceName, sf::Image *& target);
};