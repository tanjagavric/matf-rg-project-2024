
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>
#include <glad/glad.h>

namespace engine::resources {
    void Model::draw(const Shader *shader) {
        shader->use();
        for (auto &mesh: m_meshes) {
            mesh.draw(shader);
        }
    }

    void Model::destroy() {
        for (auto &mesh: m_meshes) {
            mesh.destroy();
        }
    }

    void Model::setup_instancing(const std::vector<glm::mat4> &model_matrices) {
        m_instance_amount = model_matrices.size();
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, m_instance_amount * sizeof(glm::mat4), model_matrices.data(), GL_STATIC_DRAW);
        for (auto mesh: m_meshes) {
            mesh.setup_instancing();
        }

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Model::draw_instanced(Shader *shader) const {
        shader->use();
        for (auto mesh: m_meshes) {
            mesh.draw_instanced(shader, m_instance_amount);
        }
    }
}
