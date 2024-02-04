#include "GPContext.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
    GPContext::GPContext()
    {
        m_scene_manager = std::make_unique<SceneManager>();
    }
    GPContext::~GPContext()
    {
    }
}