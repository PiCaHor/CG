#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class sShader
{
public:
    sShader(const char* vertexPath, const char* franmentPath);
    std::string vertexString;
    std::string fragmentString;
    const char* vertexSource;
    const char* fragmentSource;
    unsigned int ID;

    enum Slot
    {
        DIFFUSE,
        SPECULAR,
        EMMISION
    };

    void use();
    void SetUniform3f(const char* paramNameString, glm::vec3 param);
    void SetUniform1f(const char* paramNameString, float param);
    void SetUniform1i(const char* paramNameString, int slot);
    void SetUniformMat4(const char* paramNameString, glm::mat4 mat);
private:
    void checkCompileErrors(unsigned int ID, std::string type);
};

#endif