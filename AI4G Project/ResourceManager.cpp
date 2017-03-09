
#include "ResourceManager.h"
#include "Debug.h"

ResourceManager * ResourceManager::Context;

/*
=================
ResourceManager::TranslateResourceName

Converts the resource name to use all lower-case letters and remove whitespace, minimizing the chance
that a misspelling will causing the system to not be able to find a resource.
=================
 */
std::string ResourceManager::TranslateResourceName (std::string oldName)
{
	std::string result;

	//	Just removes spaces and converts all text to lower-case
	for (unsigned int i = 0; i < oldName.size (); i++)
	{
		if (oldName [i] != ' ')
			result += tolower (oldName [i]);
	}

	return result;
}


/*
=================
ResourceManager::GetResourceHash

Returns a quick 64-bit hash of the resource name.
=================
 */
ResourceManager::Hash ResourceManager::GetResourceHash (std::string resourceName)
{
	Hash result = 0;

	for (unsigned int i = 0; i < resourceName.size (); i++)
	{
		result ^= ((Hash)resourceName[i]) << ((i % sizeof (Hash)) * 8);
	}

	return result;
}

/*
=================
ResourceManager::ResourceManager
=================
 */
ResourceManager::ResourceManager ()
{
	m_NotFoundImage.Create (64, 64, sf::Color::Magenta);
}

/*
=================
ResourceManager::ResourceExists
=================
 */
bool ResourceManager::ResourceExists (std::string resourceName)
{
	Hash resourceHash = GetResourceHash (resourceName);

	if (m_ImageCache.find (resourceHash) != m_ImageCache.end ())
		return true;
	if (m_SoundCache.find (resourceHash) != m_SoundCache.end ())
		return true;
	if (m_MusicCache.find (resourceHash) != m_MusicCache.end ())
		return true;

	return false;
}

/*
=================
ResourceManager::GetResource
=================
 */
void ResourceManager::GetResource (std::string resourceName, sf::Image *& target)
{
	Hash resourceHash = GetResourceHash (resourceName);

	if (!ResourceExists (resourceName))
	{
		sf::Image * newImage = new sf::Image ();
		if (newImage->LoadFromFile (resourceName))
		{
			m_ImageCache [resourceHash] = newImage;
		}
		else
		{
			m_ImageCache [resourceHash] = &m_NotFoundImage;
		}

		Debug::Warning("Cache miss on resource %s with hash %llu ", resourceName.c_str(), resourceHash);
	}

	target = m_ImageCache [resourceHash];
}