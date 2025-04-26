#include "triangle_mesh.h"
#include "config.h"

TriangleMesh::TriangleMesh() {

    // Triangle vertices
    std::vector<float> positions = {
        -1.0f, -1.0f, 0.0f, 
         1.0f, -1.0f, 0.0f, 
        -1.0f,  1.0f, 0.0f,  
         1.0f,  1.0f, 0.0f, 
    };
    std::vector<int> colorIndices = {
        0, 1, 2, 3
    };
    std::vector<int> elementIndices = {
        0, 1, 2, 2, 1, 3
    };
    vertex_count = 6; // vertices

    // Generate VAO
    glGenVertexArrays(1, &VAO); // Generate VAO
    glBindVertexArray(VAO); // Bind VAO

    VBOs.resize(2);

    // position
    glGenBuffers(2, VBOs.data()); // Generate VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), 
        positions.data(), GL_STATIC_DRAW); // Buffer data

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);

    // Color indices
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, colorIndices.size() * sizeof(float), 
        colorIndices.data(), GL_STATIC_DRAW); // Buffer data
    glVertexAttribIPointer(1, 1, GL_INT, 4, (void*)0);
    glEnableVertexAttribArray(1);

    // element buffer
    glGenBuffers(1, &EBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementIndices.size() * sizeof(int), 
        elementIndices.data(), GL_STATIC_DRAW);

}

void TriangleMesh::draw() {
    glBindVertexArray(VAO); // Bind VAO
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0); // Draw triangles
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO); // Delete VAO
    glDeleteBuffers(2, VBOs.data()); // Delete VBO
    glDeleteBuffers(1, &EBO); // Delete EBO
}