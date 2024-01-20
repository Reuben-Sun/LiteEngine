#pragma once

#include "Marco.h"
#include "Core/Event/Event.h"

namespace ToolEngine
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& event){}

		std::string getName() const { return m_name; }
	protected:
		std::string m_name;
	};
}