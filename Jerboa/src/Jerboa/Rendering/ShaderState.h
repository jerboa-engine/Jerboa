#pragma once

#include "Jerboa/Core/Singleton.h"
#include "Jerboa/Rendering/Resource/Internal/GPUResource.h"

#include <glm/glm.hpp>
#include <string>

namespace Jerboa
{
    class ShaderState : Singleton<ShaderState>
    {
    public:
        virtual void SetBool(const std::string& name, bool value) = 0;
        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetVec2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetVec2(const std::string& name, float x, float y) = 0;
        virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetVec3(const std::string& name, float x, float y, float z) = 0;
        virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetVec4(const std::string& name, float x, float y, float z, float w) = 0;
        virtual void SetMat2(const std::string& name, const glm::mat2& mat) = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& mat) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& mat) = 0;
    };
}