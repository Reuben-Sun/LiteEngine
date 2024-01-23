#include "GlfwWindow.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"

namespace ToolEngine
{
	static void onErrorCallback(int error, const char* description)
	{
		LOG_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	GlfwWindow::GlfwWindow(const WindowProps& props) : Window(props)
	{
		m_data.m_width = props.m_width;
		m_data.m_height = props.m_height;
		m_data.m_title = props.m_title;
		if (!glfwInit())
		{
			LOG_ERROR("Failed to initialize GLFW!");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// TODO: resize window
		m_window = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_window, &m_data);

		glfwSetErrorCallback(onErrorCallback);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent event(width, height);
				window_data.m_event_callback(event);
			});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				window_data.m_event_callback(event);
			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					window_data.m_event_callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					window_data.m_event_callback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					window_data.m_event_callback(event);
					break;
				}
				}
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					window_data.m_event_callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					window_data.m_event_callback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x_offset, double y_offset)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)x_offset, (float)y_offset);
				window_data.m_event_callback(event);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x_pos, double y_pos)
			{
				WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)x_pos, (float)y_pos);
				window_data.m_event_callback(event);
			});
	}
	GlfwWindow::~GlfwWindow()
	{
		glfwTerminate();
	}

	void GlfwWindow::tick()
	{
		glfwPollEvents();
	}
	void GlfwWindow::setEventCallback(const EventCallbackFunc& callback)
	{
		m_data.m_event_callback = callback;
	}
	std::vector<const char*> GlfwWindow::getRequiredSurfaceExtensions() const
	{
		uint32_t glfw_extension_count{ 0 };
		const char** glfw_extensions{ glfwGetRequiredInstanceExtensions(&glfw_extension_count) };
		std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
		return extensions;
	}
}