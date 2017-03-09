
#pragma once
#pragma warning (disable: 4503)

#include <unordered_map>
#include <memory>

#include "Debug.h"

typedef long ArbitraryType;

class PropertyStore
{
public:

	template <typename T>
	bool PropertyExists( const std::string & propertyName ) const
	{
		std::string translatedPropertyName = TranslatePropertyName(propertyName);

		return m_PropertyMappings.find(translatedPropertyName) != m_PropertyMappings.end();
	}

	template <typename T>
	void SetProperty( const std::string & propertyName, const T& value )
	{
		std::string translatedPropertyName = TranslatePropertyName(propertyName);

		T * valueReference = new T ( value );

		m_PropertyMappings[translatedPropertyName] = std::unique_ptr<ArbitraryType>((ArbitraryType *)valueReference);
	}

	template <typename T>
	T GetProperty( const std::string & propertyName ) const
	{
		std::string translatedPropertyName = TranslatePropertyName(propertyName);

		if (!PropertyExists<T>(translatedPropertyName))
		{
			Debug::Warning("Unable to find property %s", propertyName.c_str());
			return T();
		}

		return *((T *) (m_PropertyMappings.find(translatedPropertyName)->second.get()));
	}

private:

	typedef std::unordered_map< std::string, std::unique_ptr<ArbitraryType> > Dictionary;

	Dictionary m_PropertyMappings;

	std::string TranslatePropertyName(const std::string & string) const
	{
		std::string result;
		for (char c : string)
			result += tolower(c);
		return result;
	}
};