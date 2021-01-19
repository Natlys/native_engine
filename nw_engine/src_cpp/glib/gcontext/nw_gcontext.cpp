#include <nw_pch.hpp>
#include "nw_gcontext.h"

#include <sys/nw_mem_sys.h>

#if (defined NW_GRAPHICS)
#include <glib/gcontext/nw_framebuf.h>
#include <glib/glib_tools.h>
#endif	// NW_WINDOW
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace NW
{
	static bool s_GLinit = false;

	GContextOgl::GContextOgl(GLFWwindow* pNativeWindow) :
		m_pNativeWindow(pNativeWindow) { }
	GContextOgl::~GContextOgl() { }

	// --==<core_methods>==--
	bool GContextOgl::OnInit()
	{
		glfwMakeContextCurrent(m_pNativeWindow);
		if (!s_GLinit) { s_GLinit = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); }
		m_GContextInfo = GContextInfo(glGetString(GL_RENDERER), glGetString(GL_VERSION),
			glGetString(GL_VENDOR), glGetString(GL_SHADING_LANGUAGE_VERSION));
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_GContextInfo.nMaxTextures);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_GContextInfo.nMaxVertexAttribs);
		std::cout << std::endl << "Graphics Context: GLFW_" << glfwGetVersionString() << std::endl <<
			"OpenGL Renderer: " << m_GContextInfo.strRenderer << std::endl <<
			"OpenGL Version: " << m_GContextInfo.strVersion << std::endl <<
			"OpenGL Vendor: " << m_GContextInfo.strVendor << std::endl <<
			"OpenGL Shading Language: " << m_GContextInfo.strShadingLanguage << "\n\n";
		
		return true;
	}
	void GContextOgl::OnQuit()
	{
		if (glfwWindowShouldClose(m_pNativeWindow) == GLFW_TRUE) { glfwTerminate(); }
	}

	void GContextOgl::SwapBuffers()
	{
		glfwSwapBuffers(m_pNativeWindow);
	}
	// --==</core_methods>==--
}
#endif	// NW_GRAPHICS