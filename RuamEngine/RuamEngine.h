#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Component.hpp"
#include "Entity.hpp"

#include "Input.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <assert.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"

#include "RendererCore.h"
#include <filesystem>

#include "Vec3.h"

#include "RuamUtils.h"
#include "AudioSystem.hpp"
