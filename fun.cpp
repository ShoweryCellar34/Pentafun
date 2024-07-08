#include <Pentagram.hpp>

PNT::image image;

void eventCallback(PNT::Window* window, PNT::windowEvent event) {
    switch(event.type) {
    case PNT_EVENT_TYPE_DROP:
        image.load(event.dropFiles.paths[0]);
        window->setDimentions(image.getWidth(), image.getHeight());
        image.loadOnGPU();
        break;
    case PNT_EVENT_TYPE_KEYBOARD:
        if(event.keyboard.key == GLFW_KEY_R) {
            window->setDimentions(image.getWidth(), image.getHeight());
        }
    }
}

int main(int argc, char *argv[]) {
    stbi_set_flip_vertically_on_load(true);

    if(!PNT::init()) {
        return 1;
    }

    image.load("res\\textures\\logo\\ghoul.png");
    PNT::Window window("Drag'n Drop", image.getWidth(), image.getHeight(), 500, 500, ImGuiConfigFlags_None);
    image.loadOnGPU();
    window.setEventCallback(eventCallback);

    // Vertex shader.
    PNT::file file("res\\shaders\\vertex.glsl");
    PNT::shader vertexShader(file.getContents().c_str(), GL_VERTEX_SHADER);
    vertexShader.compile();

    // Fragment shader.
    file.close();
    file.open("res\\shaders\\fragment.glsl");
    PNT::shader fragmentShader(file.getContents().c_str(), GL_FRAGMENT_SHADER);
    fragmentShader.compile();

    // Shader program.
    PNT::program shader({&vertexShader, &fragmentShader});
    shader.link();

    float vertices[] = {
         1.0f,  1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,
        -1.0f,  1.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniform1i(glGetUniformLocation(shader.getID(), "texture"), image.getID());

    while(!window.shouldClose()) {
        PNT::processEvents();

        window.startFrame();

        shader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.endFrame();
    }

    PNT::deinit();
    return 0;
}
