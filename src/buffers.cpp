#include "buffers.h"

using namespace amk;

EBO::EBO() {
    glGenBuffers(1, &ebo_id);
}

EBO::EBO(const std::vector<GLuint> &indices) : indices(indices) {
    glGenBuffers(1, &ebo_id);
    set_data(indices);
}

void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::set_data(const std::vector<GLuint> &indices) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * indices.size(),
                 indices.data(),
                 GL_STATIC_DRAW);
    unbind();
}

size_t EBO::nr_of_indices() const {
    return indices.size();
}

VBO::VBO() {
    glGenBuffers(1, &vbo_id);
}

VBO::VBO(const std::vector<vertex> &vertices) : vertices(vertices) {
    glGenBuffers(1, &vbo_id);
    set_data(vertices);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::set_data(const std::vector<vertex> &vertices) {
    bind();
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertex) * vertices.size(),
                 vertices.data(),
                 GL_STATIC_DRAW);
    unbind();
}

VAO::VAO() {
    glGenVertexArrays(1, &vao_id);
}

VAO::VAO(VBO &vbo, EBO &ebo) {
    glGenVertexArrays(1, &vao_id);
    add_vbo(vbo);
    add_ebo(ebo);
}

void VAO::bind() {
    glBindVertexArray(vao_id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

/* VBO must have data inside of it */
void VAO::add_vbo(VBO &vbo) {
    constexpr GLuint vec3size = 3;
    constexpr GLuint vec2size = 2;
    constexpr GLuint coordIdx = 0;
    constexpr GLuint normalIdx = 1;
    constexpr GLuint texIdx = 2;

    /* bind VAO first */
    bind();
    vbo.bind();

    glVertexAttribPointer(
        coordIdx, vec3size, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
    glEnableVertexAttribArray(coordIdx);
    glVertexAttribPointer(normalIdx,
                          vec3size,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(vertex),
                          (void *)(offsetof(vertex, normal)));
    glEnableVertexAttribArray(normalIdx);
    glVertexAttribPointer(texIdx,
                          vec2size,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(vertex),
                          (void *)(offsetof(vertex, texCoord)));
    glEnableVertexAttribArray(texIdx);

    unbind();
    vbo.unbind();
}

/* EBO must have data inside of it */
void VAO::add_ebo(EBO &ebo) {
    bind();
    ebo.bind();

    unbind();
    ebo.unbind();
}
