#include <Pentagram.hpp>
#include <iostream>

PNT::image image;

void eventCallback(PNT::Window* window, PNT::windowEvent event) {
    if(event.type == PNT_EVENT_TYPE_DROP) {
        image.load(event.dropFiles.paths[0]);
        window->setDimentions(image.getWidth(), image.getHeight());
        image.loadOnGPU();
    }
}

int main(int argc, char *argv[]) {
    if(!PNT::init()) {
        return 1;
    }

    image.load("res\\textures\\logo\\ghoul.png");
    PNT::Window window("Drag'n Drop", image.getWidth(), image.getHeight(), 500, 500, ImGuiConfigFlags_None);
    image.loadOnGPU();
    window.setEventCallback(eventCallback);

    // Vertex shader.
    PNT::file vertexFile("res\\shaders\\vertex.glsl");
    PNT::shader vertexShader(vertexFile.getContents().c_str(), GL_VERTEX_SHADER);
    vertexShader.compile();

    // Fragment shader.
    PNT::file fragmentFile("res\\shaders\\fragment.glsl");
    PNT::shader fragmentShader(fragmentFile.getContents().c_str(), GL_FRAGMENT_SHADER);
    fragmentShader.compile();

    // Shader program.
    PNT::program shader({&vertexShader, &fragmentShader});
    shader.link();
    std::cout << shader.getError(600) << '\n';

    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniform1i(glGetUniformLocation(shader.getID(), "texture"), image.getID());

    while(!window.shouldClose()) {
        PNT::processEvents();

        window.startFrame();

        ImGui::SetNextWindowSizeConstraints(ImVec2(image.getWidth() + 2.0f, image.getHeight() + 35.0f), ImVec2(image.getWidth() + 2.0f, image.getHeight() + 35.0f));
        ImGui::Begin("Image");
        image.ImGuiDraw(ImVec2(image.getWidth(), image.getHeight()));
        ImGui::End();

        window.endFrame();
    }

    PNT::deinit();
    return 0;
}
